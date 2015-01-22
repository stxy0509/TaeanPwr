/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"

/*******************************************/
/* Define values for Microsoft WAVE format */
/*******************************************/
#define RIFF		0x46464952		/* "RIFF" */
#define WAVEMAGIC		0x45564157		/* "WAVE" */
#define FACT		0x74636166		/* "fact" */
#define LIST		0x5453494c		/* "LIST" */
#define FMT		0x20746D66		/* "fmt " */
#define DATA		0x61746164		/* "data" */
#define PCM_CODE	0x0001
#define MS_ADPCM_CODE	0x0002
#define IMA_ADPCM_CODE	0x0011
#define MP3_CODE	0x0055
#define WAVE_MONO	1
#define WAVE_STEREO	2

typedef struct _tagWaveFMT
{
/* Not saved in the chunk we read:
	Uint32	FMTchunk;
	Uint32	fmtlen;
*/
    U16 encoding;
    U16 channels;            /* 1 = mono, 2 = stereo */
    U32 frequency;           /* One of 11025, 22050, or 44100 Hz */
    U32 byterate;            /* Average bytes per second */
    U16 blockalign;          /* Bytes per sample block */
    U16 bitspersample;       /* One of 8, 12, 16, or 4 for ADPCM */
} WaveFMT;
typedef struct Chunk
{
	U32 magic;
	U32 length;
	U8 *data;
} Chunk;


struct wavhdr_t {
	//RIFF header
	U32 ID; // 0x46464952 "RIFF"
	U32 chunksize;
	U32 format;//0x45564157 "WAVE"

	U32 fmtID; // 0x20746d66 "fmt "
	U32 subchunk1size; 

	U16 wFormatTag; // PCM = 1 (i.e. Linear quantization)	Values other than 1 indicate some form of compression.
	U16 nChannels; // mono = 1, stereo = 2
	U32 nSamplesPerSec; // frequency
	U32 nAvgBytesPerSec;
	U16 nBlockAlign;
	U16 wBitsPerSample;
};

static WAVE* snd_dma_ch[8]={0,};
static const INTERRUPT_TYPE dma_intnum[]={INTNUM_DMA0,INTNUM_DMA1,INTNUM_DMA2,INTNUM_DMA3,INTNUM_DMA4,INTNUM_DMA5,INTNUM_DMA6,INTNUM_DMA7};

static bool s_snd_channel_used[SOUND_MAX_CHANNEL]={0,};
static int get_snd_channel()
{
	CRITICAL_ENTER();
	int i;
	for(i=0;i<SOUND_MAX_CHANNEL;i++)
	{
		U32 stat = *R_SNDCON(i);
		if((stat & 1)==0)
		{
			if(s_snd_channel_used[i]==false)
			{
				s_snd_channel_used[i] = true;
				CRITICAL_EXIT();
				return i;
			}
		}
	}
	CRITICAL_EXIT();
	debugstring("All channel is busy\r\n");
	return -1;
}
static void release_snd_channel(int i)
{
	if( (i<0) || (i>= SOUND_MAX_CHANNEL) )
		return;
	s_snd_channel_used[i]=false;
}

void snd_dmatransfer(int dmach,int sndch,void* src, int len)
{
	dcache_invalidate_way();
	*R_DMASRC(dmach) = (U32) src;
	*R_DMADST(dmach) = (U32) R_SNDDAT(sndch);

	U32 conval=0;
	conval |= DMA_CON_SRC_32BIT;//dest 32bit
	conval |= DMA_CON_DST_32BIT;//src 32bit

	if(((U32)src & 0xf ) == 0)
		conval |= DMA_CON_SRC_BST_256;
	else
		conval |= DMA_CON_SRC_BST_1;

	conval |= DMA_CON_DST_BST_8;//burst 8
	conval |= DMA_CON_SRC_INCR;

	//must be 32
	U32 transfersize=32;

	if((len>>2)<0xfff)
	{
		conval |= (len>>2);
	}
	else
	{
		U32 reloadcnt  = ((len>>2)/transfersize)-1;
		conval |= transfersize;

		*R_DMASGA(dmach)=0;
		*R_DMADSA(dmach)=0;
		*R_DMAARCNT(dmach) = reloadcnt;
	}

	*R_DMACON(dmach) = conval;
	*R_DMACFG(dmach) =(1<<17)|(1<<12)|((DMA_PERI_SNDCH0+sndch)<<8)|1;
}

static void refill_bigfile(int dmach,WAVE* pWave)
{
	UINT nRead;
	if(f_eof(&pWave->file)==0) 
	{
		if(pWave->cursamplesnum) //samples2 was played
		{
			snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples,pWave->bufsize);
			pWave->validsamples=false;
			pWave->cursamplesnum=0;
	
			f_read(&pWave->file,pWave->samples2,WAVE_BUF_MAX,&nRead);
			pWave->bufsize2 = nRead;
			if(nRead<WAVE_BUF_MAX)//zero padding for avoid noise
				memset(pWave->samples2+nRead,0,WAVE_BUF_MAX-nRead > 0x20 ? 0x20:WAVE_BUF_MAX-nRead );
			pWave->validsamples2=true;
		}
		else
		{
			snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples2,pWave->bufsize2);
			pWave->validsamples2=0;
			pWave->cursamplesnum=1;

			f_read(&pWave->file,pWave->samples,WAVE_BUF_MAX,&nRead);
			pWave->bufsize = nRead;
			if(nRead<WAVE_BUF_MAX)//zero padding for avoid noise
				memset(pWave->samples+nRead,0,WAVE_BUF_MAX-nRead > 0x20 ? 0x20:WAVE_BUF_MAX-nRead );
			pWave->validsamples=1;
		}
		if(pWave->bLoop)
		{
			if(f_eof(&pWave->file))
				f_lseek(&pWave->file,pWave->headersize);
		}
	}
	else
	{
		if(pWave->validsamples || pWave->validsamples2)
		{
			if(pWave->cursamplesnum) //samples2 was played
			{
				if(pWave->validsamples)
				{
					snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples,pWave->bufsize);
					pWave->validsamples=false;
					pWave->cursamplesnum=0;
				}
			}
			else //samples was played
			{
				if(pWave->validsamples2)
				{
					snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples2,pWave->bufsize2);
					pWave->validsamples2=false;
					pWave->cursamplesnum=1;
				}
			}
		}
		else
		{
			f_close(&pWave->file);
			//debugprintf("%s was stopped(ch:%d)\r\n",pWave->filename,pWave->channelNumber);
			sound_stop(pWave);
			fs_enable_support_interrupt(dma_intnum[dmach],FALSE);
		}
	}
}

static void dma_sound_isr_r(int dmach)
{
	WAVE* pWave = snd_dma_ch[dmach];
	*R_DMATCIC=(1<<dmach);
	if(pWave->TxIsr)
	{
		pWave->TxIsr(dmach,pWave);
	}
	else //for PCM
	{
		if(pWave->isbigfile) //big file
		{
			refill_bigfile(dmach,pWave);
		}
		else
		{
			if(pWave->bLoop)
			{
				//debugstring("loop\r\n");
				snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples,pWave->bufsize);
			}
			else
			{
				sound_stop(pWave);
				//debugprintf("%s was stopped(ch:%d, dmach:%d)\r\n",pWave->filename,pWave->channelNumber,dmach);
			}
		}
	}
}

#define DMA_SOUND_ISR(A)	static void dma_sound_isr##A(void)\
{dma_sound_isr_r(A);}

DMA_SOUND_ISR(0)
DMA_SOUND_ISR(1)
DMA_SOUND_ISR(2)
DMA_SOUND_ISR(3)
DMA_SOUND_ISR(4)
DMA_SOUND_ISR(5)
DMA_SOUND_ISR(6)
DMA_SOUND_ISR(7)

typedef void (*ISRFP)(void) ;
static const ISRFP isrtable[8]={dma_sound_isr0,dma_sound_isr1,dma_sound_isr2,dma_sound_isr3,dma_sound_isr4,dma_sound_isr5,dma_sound_isr6,dma_sound_isr7};
BOOL SetPlayDma(WAVE* pWave)
{
	dcache_invalidate_way();
	int dmach = get_free_dma();
	if(dmach <0)
		return FALSE;
	set_interrupt(dma_intnum[dmach],isrtable[dmach]);
	enable_interrupt(dma_intnum[dmach],TRUE);

	if(pWave->isbigfile)
		fs_enable_support_interrupt(dma_intnum[dmach],TRUE);
	snd_dmatransfer(dmach,pWave->channelNumber,pWave->samples,pWave->bufsize);
	pWave->validsamples=0;
	pWave->cursamplesnum=0;
	snd_dma_ch[dmach]=pWave;
	return TRUE;

}

static WAVE* SetWaveHeader(FIL* fp)
{
	/* WAV magic header */
	U32 RIFFchunk;
	U32 wavelen = 0;
	U32 WAVEmagic;
	Chunk chunk;
	/* FMT chunk */
	WaveFMT *format = NULL;
	WAVE* pWave = NULL;
	U32 nRead;

	f_read(fp,(void*)&RIFFchunk,4,&nRead);
	f_read(fp,(void*)&wavelen,4,&nRead);
	if(nRead!=4)
		return 0;
	if (wavelen == WAVEMAGIC) {      /* The RIFFchunk has already been read */
		WAVEmagic = wavelen;
		wavelen = RIFFchunk;
		RIFFchunk = RIFF;
	} else {
		f_read(fp,(void*)&WAVEmagic ,4,&nRead);
	}
	if ((RIFFchunk != RIFF) || (WAVEmagic != WAVEMAGIC)) {
		debugstring("Unrecognized file type (not WAVE)");
		return 0;
	}
	
	/* Read the audio data format chunk */
	chunk.data = NULL;
	do {
		f_read(fp,&chunk.magic,4,&nRead);
		f_read(fp,&chunk.length,4,&nRead);
		chunk.data = (U8*) malloc(chunk.length);
		if (chunk.data == NULL) {
			debugerror(SDKERROR_NOMEM,0);
			return 0;
		}
		
		f_read(fp,chunk.data,chunk.length,&nRead);

		if (nRead!= chunk.length) {
			free(chunk.data);
			return (0);
		}
	} while ((chunk.magic == FACT) || (chunk.magic == LIST));

	format = (WaveFMT *)chunk.data;
	if (chunk.magic != FMT) {
		DEBUGPRINTF("Complex WAVE files not supported");
		free(chunk.data);
		return 0;
	}
	
	if(format->encoding != PCM_CODE)
	{
		DEBUGPRINTF("Unknown WAVE data format: 0x%.4x",format->encoding);
		free(chunk.data);
		return 0;
	}
	if(format->frequency > SND_OUTPUT_HZ)
	{
		free(chunk.data);
		return 0;
	}
	pWave = malloc(sizeof(WAVE));
	memset(pWave,0,sizeof(WAVE));
	pWave->volume = DEFAULT_VOLUME; // maximum 255
	pWave->panpot = 8; 
	pWave->channelNumber = -1;
	pWave->freq = format->frequency;
	pWave->channels = format->channels;
	int bitspersample =format->bitspersample;
	if(format->bitspersample == 16 )
	{
		if(format->channels==2)
			pWave->pcmtype = PCMTYPE_SS16;
		else
			pWave->pcmtype = PCMTYPE_SM16;

	}
	else if(format->bitspersample == 8)
	{
		if(format->channels==2)
			pWave->pcmtype = PCMTYPE_US8;
		else
			pWave->pcmtype = PCMTYPE_UM8;
	}
	else
	{
		free(chunk.data);
		free(pWave);
		return 0;
	}
	free(chunk.data);
	/* search data chunk */
	do {
		f_read(fp,&chunk.magic,4,&nRead);
		if (nRead!= 4) {
			free(pWave);
			return 0;
		}
		f_read(fp,&chunk.length,4,&nRead);
		if (nRead!= 4) {
			free(pWave);
			return 0;
		}
	} while (chunk.magic != DATA);
	
	pWave->headersize = f_tell(fp);
	pWave->datasize = chunk.length;
	
	pWave->PlaybackPitch = ((pWave->freq*256)/SND_OUTPUT_HZ) -1; 
	int duration =pWave->datasize/(pWave->freq* (bitspersample/8)*pWave->channels);
	if(duration==0)
		duration=1;
	DEBUGPRINTF("freq : %d , bits : %d , channel : %d, data-size:%d, %d sec\r\n",pWave->freq,bitspersample,pWave->channels,pWave->datasize,duration);
	return pWave;
}

static BOOL bigfile_load(WAVE* pWave)
{
	U32 nRead;
	if(pWave->samples==0)
	{
		pWave->samples = (U8*)malloc(WAVE_BUF_MAX);
		if(pWave->samples == 0)
		{
			debugerror(SDKERROR_NOMEM,0);
			return FALSE;
		}
		pWave->samples2 = (U8*)malloc(WAVE_BUF_MAX);
		if(pWave->samples2 == 0)
		{
			debugerror(SDKERROR_NOMEM,0);
			return FALSE;
		}
		pWave->bMallocedBuf = true;
	}
	f_lseek(&pWave->file,pWave->headersize);
	f_read(&pWave->file,pWave->samples,WAVE_BUF_MAX,&nRead);
	pWave->bufsize=nRead;

	f_read(&pWave->file,pWave->samples2,WAVE_BUF_MAX,&nRead);
	pWave->bufsize2=nRead;
	int paddsize = WAVE_BUF_MAX - nRead;
	if(paddsize >0)
	{
		memset(&pWave->samples2[pWave->datasize],0,paddsize);
	}
	pWave->cursamplesnum=0;
	pWave->validsamples=1;
	pWave->validsamples2=1;
	return TRUE;
}
static inline U32 makeword(U8* buf)
{
	U32 re = buf[0] + (buf[1]<<8) + (buf[2]<<16) + (buf[3]<<24);
	return re;
}
WAVE* sound_loadwavp(U8* buf,U32 len)
{
	{
		/* WAV magic header */
		U32 RIFFchunk;
		//chunk point
		U32 Length;
		U32 Magic;
		/* FMT chunk */
		WaveFMT format ;
		WAVE* pWave = NULL;
		U32 nRead=0;
		RIFFchunk = makeword(buf);
		nRead +=4;
		Magic = makeword(buf+nRead);
		nRead +=4;
		if (Magic != WAVEMAGIC) {      /* The RIFFchunk has already been read */
			Magic = makeword(buf+nRead);
			nRead +=4;
		} 
		if ((RIFFchunk != RIFF) || (Magic != WAVEMAGIC)) {
			debugstring("Unrecognized file type (not WAVE)");
			return 0;
		}

		/* Read the audio data format chunk */
		do {
			Magic = makeword(buf+nRead);
			nRead +=4;
		} while ((Magic== FACT) || (Magic== LIST));
		Length= makeword(buf+nRead);
		nRead += 4;
		
		memcpy(&format, (buf+nRead),sizeof(WaveFMT));
		nRead += Length;

		if (Magic != FMT) {
			DEBUGPRINTF("Complex WAVE files not supported");
			return 0;
		}

		if(format.encoding != PCM_CODE)
		{
			DEBUGPRINTF("Unknown WAVE data format: 0x%.4x",format.encoding);
			return 0;
		}
		if(format.frequency > SND_OUTPUT_HZ)
		{
			DEBUGPRINTF("Wave Frequency(%d) is too bot to play\r\n",format.frequency);
			return 0;
		}
		pWave = malloc(sizeof(WAVE));
		memset(pWave,0,sizeof(WAVE));
		pWave->volume = DEFAULT_VOLUME; // maximum 255
		pWave->panpot = 8; 
		pWave->channelNumber = -1;
		pWave->freq = format.frequency;
		pWave->channels = format.channels;
		int bitspersample =format.bitspersample;
		if(format.bitspersample == 16 )
		{
			if(format.channels==2)
				pWave->pcmtype = PCMTYPE_SS16;
			else
				pWave->pcmtype = PCMTYPE_SM16;

		}
		else if(format.bitspersample == 8)
		{
			if(format.channels==2)
				pWave->pcmtype = PCMTYPE_US8;
			else
				pWave->pcmtype = PCMTYPE_UM8;
		}
		else
		{
			free(pWave);
			return 0;
		}
		/* search data chunk */
		do {
			Magic = makeword(buf+nRead);
			nRead += 1;
			if(nRead >= len)
			{
				debugstring("Unrecognized file type (not WAVE)");
				free(pWave);
				return 0;
			}
		} while (Magic != DATA);
		nRead+=3;
		Length = makeword(buf+nRead);
		nRead += 4;

		pWave->headersize = 0;
		pWave->datasize = Length;
 
		pWave->samples = buf+nRead;
		pWave->bufsize = Length;

		pWave->PlaybackPitch = ((pWave->freq*256)/SND_OUTPUT_HZ) -1; 
		int duration =pWave->datasize/(pWave->freq* (bitspersample/8)*pWave->channels);
		if(duration==0)
			duration=1;
		DEBUGPRINTF("freq : %d , bits : %d , channel : %d, data-size:%d, %d sec\r\n",pWave->freq,bitspersample,pWave->channels,pWave->datasize,duration);
		return pWave;
	}
}
WAVE* sound_loadwav(char* filename)
{
	WAVE* pWave;
	U32 nRead;
	FIL fp;
	if(f_open(&fp,filename,FA_READ|FA_OPEN_EXISTING)!=FR_OK)
	{
		DEBUGPRINTF("Failed to open %s\r\n",filename);
		return 0;
	}

	pWave = SetWaveHeader(&fp);
	if(pWave == 0)
	{
		f_close(&fp);
		return 0;
	}
	strcpy(pWave->filename,filename);

	if(pWave->datasize>WAVE_BUF_MAX)
	{
		// big-file, data will be loaded at play.
		pWave->isbigfile = true;
		pWave->file = fp;
	}
	else
	{
		pWave->isbigfile=false;
		pWave->bufsize = (pWave->datasize+0x1f)&0xffffffe0; //align at 0x20 boundry
		pWave->samples = (U8*)malloc(pWave->bufsize);
		if(pWave->samples == 0)
		{
			debugerror(SDKERROR_NOMEM,0);
			free(pWave);
			f_close(&fp);
			return 0;
		}
		pWave->bMallocedBuf = true;
		f_lseek(&fp,pWave->headersize);
		f_read(&fp,pWave->samples,pWave->bufsize,&nRead);
		f_close(&fp);
		U32 paddsize = pWave->bufsize - pWave->datasize ;
		if(paddsize >0)
		{
			memset(&pWave->samples[pWave->datasize],0,paddsize);
		}
	}
	return pWave;
}

WAVE* sound_loadrawp(void* sampels, int freq, PCMTYPE type, int bytelen)
{
	WAVE* pWave ;
	pWave = malloc(sizeof(WAVE));
	if(pWave==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return 0;
	}
	memset(pWave,0,sizeof(WAVE));
	pWave->volume = DEFAULT_VOLUME; // maximum 255
	pWave->panpot = 8; 
	pWave->channelNumber = -1;
	pWave->freq = freq;
	pWave->channels =1;
	pWave->datasize = bytelen;
	pWave->PlaybackPitch = ((pWave->freq*256)/SND_OUTPUT_HZ) -1; 
	pWave->samples = sampels;
	pWave->bufsize = bytelen;
	pWave->pcmtype  = type;
	return pWave;
}

BOOL sound_resume(WAVE* pWave)
{
	if(sound_ispause(pWave))
	{
		*R_SNDCON(pWave->channelNumber) |= 1;
		pWave->state=WAVE_STATE_PLAY;
		return TRUE;
	}
	return FALSE;
}
BOOL sound_pause(WAVE* pWave)
{
	BOOL b=FALSE;
	if(pWave==0)
		return FALSE;
	if(pWave->channelNumber<0)
		return FALSE;
	CRITICAL_ENTER();
	if(pWave->state==WAVE_STATE_PLAY)
	{
		*R_SNDCON(pWave->channelNumber) &=~1;//disable
		pWave->state=WAVE_STATE_PAUSE;
		b= TRUE;
	}
	CRITICAL_EXIT();
	return FALSE;
}

BOOL sound_stop(WAVE* pWave)
{
	BOOL b=FALSE;
	if(pWave==0)
		return FALSE;
	if(pWave->channelNumber<0)
		return FALSE;
	if(pWave->channelNumber != -1)
	{
		int i;
		CRITICAL_ENTER();
		for(i=0;i<8;i++)
		{
			if(pWave == snd_dma_ch[i])
			{
				*R_DMAARCNT(i)=0;
				*R_DMACFG(i) |= 1<<20;//ignore DMA
				while( (*R_DMACFG(i)) &(1<<21));///wait for empty
				*R_DMACFG(i) =0;
				set_free_dma(i);
				break;
			}
		}
		*R_SNDCON(pWave->channelNumber)=0;
		pWave->channelNumber =-1;
		pWave->state=WAVE_STATE_STOP;
		b=TRUE;
		release_snd_channel(i);
		CRITICAL_EXIT();
	}
	return b;
}

BOOL sound_play(WAVE* pWave)
{
	int i;
	if(pWave == 0)
		return FALSE;
	sound_stop(pWave);
	i = get_snd_channel();
	if(i==-1)
	{
		return FALSE;
	}
	if(pWave->preparetoplay!=0)//mp3?
	{
		if(pWave->preparetoplay(pWave)==FALSE)
			return FALSE;
	}
	else //wav
	{
		if(pWave->isbigfile)
		{
			if(bigfile_load(pWave)==FALSE)
				return FALSE;
		}
	}
	//debugprintf("%d channel allocated\r\n",i);
	pWave->channelNumber = i;
	U32 conval=0;
	conval = (1<<28)|(pWave->PlaybackPitch<<16) | (SND_OUTPUT_CHANNEL<<8)|(pWave->pcmtype<<4) |1;

	*R_SNDCON(pWave->channelNumber) = conval|(1<<3);
	*R_SNDVOL(pWave->channelNumber) = pWave->volume|pWave->volume<<8;
	if(SetPlayDma(pWave))
	{
		pWave->state=WAVE_STATE_PLAY;
		CRITICAL_EXIT();
		return TRUE;
	}
	*R_SNDCON(pWave->channelNumber) =0;
	return FALSE;
}

BOOL sound_ispause(WAVE* pWave)
{
	if(pWave==0)
		return FALSE;
	if(pWave->channelNumber<0)
		return FALSE;
	if(pWave->channelNumber>(SOUND_MAX_CHANNEL-1))//max channel 
		return FALSE;
	CRITICAL_ENTER();
	if(pWave->state==WAVE_STATE_PAUSE)
	{
		CRITICAL_EXIT();
		return TRUE;
	}
	CRITICAL_EXIT();
	return FALSE;
}
BOOL sound_isplay(WAVE* pWave)
{
	if(pWave==0)
		return 0;
	if(pWave->channelNumber<0)
		return FALSE;
	if(pWave->channelNumber>(SOUND_MAX_CHANNEL-1))//max channel 
		return FALSE;
	CRITICAL_ENTER();
	if(pWave->state==WAVE_STATE_PLAY)
	{
		CRITICAL_EXIT();
		return TRUE;
	}
	CRITICAL_EXIT();
	return FALSE;
}

void sound_clock_init();//adstarinit.c
BOOL sound_init()
{
	sound_clock_init();
	if(SND_OUTPUT_CHANNEL>1)//DIGITAL MOD
	{
		*R_SNDOUT(SND_OUTPUT_CHANNEL) = (3<<8)|(3)|(0<<2) ;
		DEBUGPRINTF("Sound Output(digital pwm) %d channel\r\n",SND_OUTPUT_CHANNEL);
	}
	else //I2S
	{
		*R_SNDOUT(SND_OUTPUT_CHANNEL) = 1;//16fs, 256fs
		DEBUGPRINTF("Sound Output(I2S) %d channel\r\n",SND_OUTPUT_CHANNEL);
	}

	*(vU32*)(_SND_BASE_+0x80+(SND_OUTPUT_CHANNEL*0x10)) = 1;
	return TRUE;
}

BOOL sound_release(WAVE* pWave)
{
	if(pWave)
	{
		if(pWave->bMallocedBuf)
		{
			if(pWave->samples)
				free(pWave->samples);

			if(pWave->samples2)
				free(pWave->samples2);
		}
		if(pWave->release !=0)
			pWave->release(pWave->user);
		if(pWave->isbigfile)
		{
			f_close(&pWave->file);
		}

		free(pWave);
		return TRUE;
	}
	return FALSE;
}

void sound_vol(U8 vol)
{
	*R_SNDVOLOUT(SND_OUTPUT_CHANNEL) = ((u32)vol<<8)+(vol);
}

void sound_vol_wav(WAVE* pWav,U8 vol)
{
	pWav->volume = vol;
}
