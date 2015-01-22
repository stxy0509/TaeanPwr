/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma  once

#define _SND_BASE_ 0xA0021C00  

#define R_SNDCON(ch) ((volatile U32*)(_SND_BASE_+ (0x10*(ch))))
#define R_SNDCON0	((volatile U32*)_SND_BASE_)
#define R_SNDCON1	((volatile U32*)(_SND_BASE_+0x10))
#define R_SNDCON2	((volatile U32*)(_SND_BASE_+0x20))
#define R_SNDCON3	((volatile U32*)(_SND_BASE_+0x30))

#define R_SNDINR	((volatile U32*)(_SND_BASE_+0x70))
#define R_SNDCONOUT0	((volatile U32*)(_SND_BASE_+0x80))
#define R_SNDCONOUT1	((volatile U32*)(_SND_BASE_+0x90))
#define R_SNDCONOUT2	((volatile U32*)(_SND_BASE_+0xa0))
#define R_SNDCONOUT3	((volatile U32*)(_SND_BASE_+0xb0))

#define R_SNDVOL(ch)	((volatile U32*)(_SND_BASE_+0x04+(0x10*(ch))))
#define R_SNDVOL0	((volatile U32*)(_SND_BASE_+0x04))
#define R_SNDVOL1	((volatile U32*)(_SND_BASE_+0x14))
#define R_SNDVOL2	((volatile U32*)(_SND_BASE_+0x24))
#define R_SNDVOL3	((volatile U32*)(_SND_BASE_+0x34))
#define R_SNDVOLINR	((volatile U32*)(_SND_BASE_+0x74))

#define R_SNDVOLOUT(ch)	((volatile U32*)(_SND_BASE_+0x84 +(0x10*(ch))))
#define R_SNDVOLOUT0	((volatile U32*)(_SND_BASE_+0x84))
#define R_SNDVOLOUT1	((volatile U32*)(_SND_BASE_+0x94))
#define R_SNDVOLOUT2	((volatile U32*)(_SND_BASE_+0xa4))
#define R_SNDVOLOUT3	((volatile U32*)(_SND_BASE_+0xb4))

#define R_SNDBST(ch)	((volatile U32*)(_SND_BASE_+0x08+(0x10*(ch))))
#define R_SNDBST0	((volatile U32*)(_SND_BASE_+0x08))
#define R_SNDBST1	((volatile U32*)(_SND_BASE_+0x18))
#define R_SNDBST2	((volatile U32*)(_SND_BASE_+0x28))
#define R_SNDBST3	((volatile U32*)(_SND_BASE_+0x38))
#define R_SNDBSTINR	((volatile U32*)(_SND_BASE_+0x78))


#define R_SNDDAT(ch)	((volatile U32*)(_SND_BASE_+0x0c+(0x10*(ch))))
#define R_SNDDAT0	((volatile U32*)(_SND_BASE_+0x0c))
#define R_SNDDAT1	((volatile U32*)(_SND_BASE_+0x1c))
#define R_SNDDAT2	((volatile U32*)(_SND_BASE_+0x2c))
#define R_SNDDAT3	((volatile U32*)(_SND_BASE_+0x3c))
#define R_SNDDATINR	((volatile U32*)(_SND_BASE_+0x7c))
											 
#define R_SNDOUT(ch)	((volatile U32*)(_SND_BASE_+0x8c+(0x10*(ch))))
#define R_SNDOUT0	((volatile U32*)(_SND_BASE_+0x8c))
#define R_SNDOUT1	((volatile U32*)(_SND_BASE_+0x8c))
#define R_SNDOUT2	((volatile U32*)(_SND_BASE_+0xac))
#define R_SNDOUT3	((volatile U32*)(_SND_BASE_+0xbc))

#define R_SNDIST	((volatile U32*)(_SND_BASE_+0xc0))

typedef enum
{
	PCMTYPE_US8=0,
	PCMTYPE_UM8,
	PCMTYPE_SS8,
	PCMTYPE_SM8,
	PCMTYPE_US16,
	PCMTYPE_UM16,
	PCMTYPE_SS16,
	PCMTYPE_SM16,
}PCMTYPE;
enum 
{
	WAVE_STATE_STOP=0,
	WAVE_STATE_PLAY,
	WAVE_STATE_PAUSE,
};
typedef struct _tagWAVE {
	U32 freq;  ///< Audio frequency in samples per second
	U8* samples; ///< 
	U32 bufsize;///< it is not buf length but valid data length in samples, should be 4*8byte align. real buffer size is WAV_BUF_MAX
	U8* samples2; ///< ping-pong buffer
	U32 bufsize2;///< it is not buf length but valid data length in samples, should be 4*8byte align. real buffer size is WAV_BUF_MAX
	int headersize;
	volatile int cursamplesnum;
	volatile bool validsamples;//for samples
	volatile bool validsamples2;//for samples2
	bool isbigfile;
	PCMTYPE pcmtype; ///< PCM8,PCM16 
	S8  channels;///< Number of channels: 1 mono, 2 stereo
	S16 PlaybackPitch;
	U8	bLoop;//0 : one-shot, 1: continuous
	S8  channelNumber;//allocated channel number
	U8  volume;
	U8	panpot;
	char filename[128];
	U32  datasize;// full data size
	volatile int state;//puase, stop play
	FIL file;
	bool bMallocedBuf;///< sound_release will check it.
	bool (*preparetoplay)(struct _tagWAVE* pWave);
	void (*TxIsr)(int dmach, struct _tagWAVE* pWave);//called by DMA-ISR when data-transfer was completed.you can use it for your decoder , see the sma_isr_r() at soundmixer.c
	void* user;//for  mp3 file
	void (*release)(void* user);//for extend, maybe used for free user-memory 
}WAVE;

BOOL sound_init();
BOOL sound_stop(WAVE* pWave);
BOOL sound_resume(WAVE* pWave);
BOOL sound_pause(WAVE* pWave);
WAVE* sound_loadwav(char* filename);
WAVE* sound_loadwavp(U8* buf,U32 len);
WAVE* sound_loadrawp(void* sampels, int freq, PCMTYPE type, int bytelen);
BOOL sound_play(WAVE* pWave);
BOOL sound_ispause(WAVE* pWave);
BOOL sound_isplay(WAVE* pWave);
BOOL sound_release(WAVE* pWave);
void sound_vol(U8 vol);
void sound_vol_wav(WAVE* pWav,U8 vol);

WAVE* sound_loadmp3(char* filename); //implemented at mp3play.c 
WAVE* sound_loadmp3p(void* buf,U32 mp3databufsize);

void snd_dmatransfer(int dmach,int sndch,void* src, int len);

#define SOUND_MAX_CHANNEL 4

