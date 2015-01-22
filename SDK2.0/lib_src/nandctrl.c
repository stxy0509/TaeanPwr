/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"

/* 
parity size : 
4bit ecc,7byte/512byte
24bit ecc, 336bit(42byte)/1 kbyte

ECC code offset : 8Kpage(spare436):100byte,2kpage(spare 64byte):36byte
*/
#define PARITY_OFFSET_4 20
#define PARITY_OFFSET_24 100 


//#define NAND_USE_DMA
//#define NAND_DEBUG
#ifdef NAND_DEBUG
#define NAND_DEBUGPRINTF	debugprintf
#else
#define NAND_DEBUGPRINTF(...)
#endif
// Device Code Define
#define K9F5608U	0x75EC
#define K9F1208U	0x76EC
#define K9F1G08U	0xF1EC
#define K9F2G08U	0xDAEC
#define K9F4G08U	0xDCEC
#define K9F8G08U	0xD3EC
#define K9GAG08U	0xD5EC

#define S34ML01G	0xF101
#define S34ML02G	0xDA01
#define S34ML04G	0xDC01


#define BLOCK2PAGE(BlockNum) (BlockNum * NandInfo.pageperblock)

volatile U32 g_dummy;//for dummy read

typedef struct _tagNand
{
	U16 id;
	U32 pagesize;
	U32 blocksize;
	U32 pageperblock;
	U32 blockcnt;
	U32 sparesize;
	U32 maxbadblockcnt;
	U32 rowaddrlen;
	U32 datastartblock; //file system area
	U32 datablockcnt;
	U8 eccbit;
}NANDTYPE;

NANDTYPE nand_db[]=
{
	{
		.id=K9F5608U,
		.pagesize=512,
		.blocksize=512*32,
		.pageperblock=32,
		.blockcnt=2048,
		.sparesize=16,
		.maxbadblockcnt=35,
		.rowaddrlen=16,
		.datastartblock=64,//1Mbyte
		.datablockcnt = 4096-64-35,
		.eccbit=0,
	},
	{
		.id=K9F1208U,
		.pagesize=512,
		.blocksize=512*32,
		.pageperblock=32,
		.blockcnt=4096,
		.sparesize=16,
		.maxbadblockcnt=70,
		.rowaddrlen=17,
		.datastartblock=64,//1Mbyte
		.datablockcnt = 4096-64-70,
		.eccbit=0,
	},
	{
		.id=K9F1G08U,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=1024,
		.sparesize=64,
		.maxbadblockcnt=20,
		.rowaddrlen=16,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 1024-8-20,
		.eccbit=0,
	},
	{
		.id=K9F2G08U,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=2048,
		.sparesize=64,
		.maxbadblockcnt=40,
		.rowaddrlen=17,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 2048-8-40,
		.eccbit=4,
	},
	{
		.id=K9F4G08U,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=4096,
		.sparesize=64,
		.maxbadblockcnt=70,
		.rowaddrlen=18,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 4096-8-70,
		.eccbit=4,
	},
	{
		.id=K9F8G08U,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=8192,
		.sparesize=64,
		.maxbadblockcnt=160,
		.rowaddrlen=19,
		.datastartblock=8,
		.datablockcnt = 8192-8-160,
		.eccbit=4,
		},
	{
		.id=K9GAG08U,
		.pagesize=(8*1024),
		.blocksize=(8*1024*128),
		.pageperblock=128,
		.blockcnt=2048,
		.sparesize=436,
		.maxbadblockcnt=58,
		.rowaddrlen=19,
		.datastartblock=1,//1Mbyte
		.datablockcnt = 2048-1-58,
		.eccbit=24,
	},
	{
		.id=S34ML01G,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=1024,
		.sparesize=64,
		.maxbadblockcnt=20,
		.rowaddrlen=16,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 1024-8-20,
		.eccbit=0,
	},
	{
		.id=S34ML02G,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=2048,
		.sparesize=64,
		.maxbadblockcnt=40,
		.rowaddrlen=17,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 2048-8-40,
		.eccbit=4,
	},
	{
		.id=S34ML04G,
		.pagesize=2048,
		.blocksize=(2048*64),
		.pageperblock=64,
		.blockcnt=4096,
		.sparesize=64,
		.maxbadblockcnt=70,
		.rowaddrlen=18,
		.datastartblock=8,//1Mbyte
		.datablockcnt = 4096-8-70,
		.eccbit=4,
	},	
	{0,},
};

static NANDTYPE NandInfo=
{
	.id=K9F1208U,
	.pagesize=512,
	.blocksize=512*32,
	.pageperblock=32,
	.blockcnt=4096-4,//-4(for copyback)
	.sparesize=16,
	.maxbadblockcnt=33,
	.rowaddrlen=9,
	.datastartblock=64//-4(for copyback)
};

#ifdef NAND_USE_DMA

// supports only 512,1024,2048,8192byte
static BOOL dma_read_nand(void* dest,U32 bytelen)
{
	U32 conval=0;
	int dmach = get_free_dma();
	if(dmach<0)
		return FALSE;

	dcache_invalidate_way();
	*R_NFMCON |= (1<< 8);//dma request

	U32 transfer_size=bytelen>>2;
	conval |= DMA_CON_SRC_32BIT;//src 32bit

	if(((U32)dest & 0x3)==0)
		conval |= DMA_CON_DST_32BIT;
	else if(((U32)dest & 0x1)==0)
		conval |= DMA_CON_DST_16BIT;
	else
		conval |= DMA_CON_DST_8BIT;


	conval |= DMA_CON_SRC_BST_4;
	
	if(((U32)dest & 0xf ) == 0)
		conval |= DMA_CON_DST_BST_256;
	else
		conval |= DMA_CON_DST_BST_1;

	conval |= DMA_CON_DST_INCR; //dest address increment

	*R_DMASGA(dmach) = 0;
	*R_DMADSA(dmach) = 0;
	*R_DMAARCNT(dmach) = 0;
	*R_DMASRC(dmach) = (U32)R_NFMDATA;
	*R_DMADST(dmach) = (U32)dest;
	conval |= (transfer_size&0xfff);
	*R_DMACON(dmach) = conval;
	*R_DMACFG(dmach) = (DMA_PERI_NAND_RX<<4)|1;
	
	while((*R_DMACFG(dmach))&1);
	set_free_dma(dmach);
	return TRUE;
}

// supports only 512,1024,2048,8192byte

static BOOL dma_write_nand(void* src,U32 bytelen)
{
	U32 conval=0;
	int dmach = get_free_dma();
	if(dmach<0)
		return FALSE;

	dcache_invalidate_way();
	*R_NFMCON |= (1<< 8);//dma request

	U32 transfer_size;
	U32 loopcnt=1;
	U32 onetimesize = bytelen;
	conval |= DMA_CON_DST_32BIT;//register

	if(((U32)src & 0x3)==0)
	{
		conval |= DMA_CON_SRC_32BIT;
		transfer_size = bytelen>>2;
	}
	else if(((U32)src & 0x1)==0)
	{
		conval |= DMA_CON_SRC_16BIT;
		if(bytelen==8192)
		{
			onetimesize = 4096;
			transfer_size=2048;
			loopcnt = 2;
		}
		else
			transfer_size = bytelen>>1;
	}
	else
	{
		conval |= DMA_CON_SRC_8BIT;
		if(bytelen==8192)
		{
			transfer_size =2048;
			onetimesize = 2048;
			loopcnt = 4;
		}
		else
			transfer_size = bytelen;
	}

	if(((U32)src & 0xf ) == 0)
		conval |= DMA_CON_SRC_BST_256;
	else
		conval |= DMA_CON_SRC_BST_1;

	conval |= DMA_CON_DST_BST_256;


	conval |= DMA_CON_SRC_INCR; //src address increment
	conval |= (transfer_size&0xfff);

 	*R_DMASRC(dmach) = (U32)src;
	*R_DMADST(dmach) = (U32)R_NFMDATA;
	*R_DMASGA(dmach) = 0;
	*R_DMADSA(dmach) = 0;
	*R_DMAARCNT(dmach) = loopcnt -1;
	*R_DMACON(dmach) = conval;
	*R_DMACFG(dmach) = (DMA_PERI_NAND_TX<<8)|1;

	while((*R_DMACFG(dmach))&1);
	set_free_dma(dmach);
	return TRUE;
}

#endif

static void WaitBusy()
{
	// Check Busy End
	while ( !(*(volatile U8*)R_NFMSTAT & 0x01) );
	while ( !(*(volatile U8*)R_NFMSTAT & 0x02) ); // for "trr" timing,see Nand-Flash data sheet
	int i;
	for( i=0;i<1;i++);
}

__inline__ void nand_reset()
{
	*(volatile char *)R_NFMCMD  = NAND_COMM_RESET ;  // Read 
	WaitBusy();
}
U16 nand_id_read(void)
{
	nand_reset();
	/*
	U16 id ;
	U16 id_dummy;
	*(volatile char *)R_NFMCMD  = NAND_COMM_RID ;  // Read 

	*(volatile char *)R_NFMADDR  = 0x00 ;  // ADDRESS 

	id = (U16)*(volatile U8*)R_NFMDATA ;
	id += ((U16)*(volatile U8*)R_NFMDATA)<<8 ;

	id_dummy = (U16)*(volatile U8*)R_NFMDATA ;
	id_dummy += ((U16)*(volatile U8*)R_NFMDATA)<<8 ;
	*/
	U32 id = *R_NFMID;
	return (U16)id;
}


__inline__ static void setpageaddress(U32 pagenum,U32 offset)
{
	int i;
	int k;
	U8 mask;
	U8 validbit;
	*(volatile U8 *)R_NFMADDR  = offset&0xff ;  // A0-A7
	if(NandInfo.pagesize!=512)
		*(volatile U8 *)R_NFMADDR  = offset>>8 ;  

	for(i=0;i<NandInfo.rowaddrlen;i+=8)
	{
		mask=0;
		validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
		for(k=0;k<validbit;k++)
			mask |= (1<<k);
		*(volatile U8 *)R_NFMADDR  = (U8)(pagenum  & mask) ;  // Row Address
		pagenum = pagenum>>8;
	}
}
__inline__ static void seteraseblockaddr(U32 blockaddr)
{
	int i;
	int k;
	U8 mask;
	U8 validbit;
	for(i=0;i<NandInfo.rowaddrlen;i+=8)
	{
		mask=0;
		validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
		for(k=0;k<validbit;k++)
			mask |= (1<<k);
		*(volatile U8 *)R_NFMADDR  = (U8)(blockaddr  & mask) ;  // Row Address
		blockaddr = blockaddr>>8;
	}
}
__inline__ static void setspareaddr(U32 pagenum)
{
	if(NandInfo.pagesize==512)
		setpageaddress(pagenum,0);
	else
	{
		int i;
		int k;
		U8 mask;
		U8 validbit;
		*(volatile U8 *)R_NFMADDR  = 0 ;  
		if(NandInfo.pagesize!=512)
			*(volatile U8 *)R_NFMADDR  =  NandInfo.pagesize>>8;  

		for(i=0;i<NandInfo.rowaddrlen;i+=8)
		{
			mask=0;
			validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
			for(k=0;k<validbit;k++)
				mask |= (1<<k);
			*(volatile U8 *)R_NFMADDR  = (U8)(pagenum  & mask) ;  // Row Address
			pagenum = pagenum>>8;
		}
	}
}


BOOL nand_phy_readpage(U32 pageaddr, void* buffer)
{
	U8* buf = (U8*)buffer;
	if(NandInfo.eccbit==24) //ECC 
	{
		int j;
		for(j=0;j<NandInfo.pagesize/1024;j++,buf+=1024)
		{
			g_dummy = *R_NFMSTAT; //clear busy
			*(volatile char *)R_NFMCMD  = NAND_COMM_READ1 ;  // Read 
			setpageaddress(pageaddr,j*1024);
			*(volatile char *)R_NFMCMD  = NAND_COMM_READ2 ;  // Read 
			g_dummy = *R_NFMMLCECC(10);
			WaitBusy();
			//set  ecc location
			*R_NFMSPADDR = NandInfo.pagesize+(j*4*11)+PARITY_OFFSET_24;
			*R_NFMCON |= (1<<16);
			
			while(1)
			{
				U32 stat = *R_NFMSTAT;
				if( stat & (1<<2) )//wait for auto decoding 
				{
					if (( stat & (1<<3) )==0)
					{
						debugprintf("ECC Decoding error(NFSTAT:%#x)\r\n",stat);
					}
#ifdef NAND_DEBUG
					if((stat >> 12)&0x1f)
					{
					debugprintf("error bit count %d ==> corrected\r\n",(stat >> 12)&0x1f);
					}
#endif
					break;
				}
			}

			U32 i;

			if((U32)(&buf[0])&1) //1byte boundry
			{
				for(i=0;i<1024;)
				{
					buf[i] = *(volatile U8*)R_NFMECD;
					buf[i+1] = *(volatile U8*)R_NFMECD;
					buf[i+2] = *(volatile U8*)R_NFMECD;
					buf[i+3] = *(volatile U8*)R_NFMECD;
					i+=4;
				}
			}
			else if((U32)(&buf[0])&2) //2byte boundry
			{
				for(i=0;i<1024;)
				{
					*(U16*)(buf+i) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+2) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+4) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+6) = *(volatile U16*)R_NFMECD;
					i+=8;
				}
			}
			else
			{
				for(i=0;i<1024;)
				{
					*(U32*)(buf+i) = *R_NFMECD;
					*(U32*)(buf+i+4) = *R_NFMECD;
					*(U32*)(buf+i+8) = *R_NFMECD;
					*(U32*)(buf+i+12) = *R_NFMECD;
					i+=16;
				}		
			}
			while ( !(*(volatile U8*)R_NFMSTAT & 0x02) ); // for "trr" timing,see Nand-Flash data sheet
		}
		return TRUE;
	}
	if(NandInfo.eccbit==4) //ECC 
	{
		int j;
		for(j=0;j<NandInfo.pagesize/512;j++,buf+=512)
		{
			g_dummy = *R_NFMSTAT; //clear busy
			*(volatile char *)R_NFMCMD  = NAND_COMM_READ1 ;  // Read 
			setpageaddress(pageaddr,j*512);
			*(volatile char *)R_NFMCMD  = NAND_COMM_READ2 ;  // Read 
			//set  ecc location
			*R_NFMSPADDR = NandInfo.pagesize+(j*4*2)+PARITY_OFFSET_4;
			g_dummy = *R_NFMMLCECC(10);//clear
			WaitBusy();
			*R_NFMCON |= ((1<<16)|(1<<15));//4bit, AUTO ECC

			while(1)
			{
				U32 stat = *R_NFMSTAT;
				if( stat & (1<<2) )//wait for auto decoding 
				{
					if (( stat & (1<<3) )==0)
					{
						if(stat < (1<<11))
							debugprintf("ECC Decoding error(NFSTAT:%#x)\r\n",stat);
					}
#ifdef NAND_DEBUG
					if((stat >> 12)&0x1f)
					{
						debugprintf("error bit count %d ==> corrected\r\n",(stat >> 12)&0x1f);
					}
#endif
					break;
				}
			}
			U32 i;

			if((U32)(&buf[0])&1) //1byte boundry
			{
				for(i=0;i<512;)
				{
					buf[i] = *(volatile U8*)R_NFMECD;
					buf[i+1] = *(volatile U8*)R_NFMECD;
					buf[i+2] = *(volatile U8*)R_NFMECD;
					buf[i+3] = *(volatile U8*)R_NFMECD;
					i+=4;
				}
			}
			else if((U32)(&buf[0])&2) //2byte boundry
			{
				for(i=0;i<512;)
				{
					*(U16*)(buf+i) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+2) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+4) = *(volatile U16*)R_NFMECD;
					*(U16*)(buf+i+6) = *(volatile U16*)R_NFMECD;
					i+=8;
				}
			}
			else
			{
				for(i=0;i<512;)
				{
					*(U32*)(buf+i) = *R_NFMECD;
					*(U32*)(buf+i+4) = *R_NFMECD;
					*(U32*)(buf+i+8) = *R_NFMECD;
					*(U32*)(buf+i+12) = *R_NFMECD;
					i+=16;
				}		
			}
			while ( !(*(volatile U8*)R_NFMSTAT & 0x02) ); // for "trr" timing,see Nand-Flash data sheet
		}
		return TRUE;

	}
	else
	{
		g_dummy = *R_NFMSTAT; //clear busy

		*(volatile char *)R_NFMCMD  = NAND_COMM_READ1 ;  // Read 
		setpageaddress(pageaddr,0);
		if(NandInfo.pagesize != 512)
			*(volatile char *)R_NFMCMD  = NAND_COMM_READ2 ;  // Read 

		// Check Busy End
		WaitBusy();
#ifdef NAND_USE_DMA
		dma_read_nand(buf,NandInfo.pagesize);
#else
		U32 i;

		if((U32)(&buf[0])&1) //1byte boundry
		{
			for(i=0;i<NandInfo.pagesize;)
			{
				buf[i] = *(volatile U8*)R_NFMDATA;
				buf[i+1] = *(volatile U8*)R_NFMDATA;
				buf[i+2] = *(volatile U8*)R_NFMDATA;
				buf[i+3] = *(volatile U8*)R_NFMDATA;
				i+=4;
			}
		}
		else if((U32)(&buf[0])&2) //2byte boundry
		{
			for(i=0;i<NandInfo.pagesize;)
			{
				*(U16*)(buf+i) = *(volatile U16*)R_NFMDATA;
				*(U16*)(buf+i+2) = *(volatile U16*)R_NFMDATA;
				*(U16*)(buf+i+4) = *(volatile U16*)R_NFMDATA;
				*(U16*)(buf+i+6) = *(volatile U16*)R_NFMDATA;
				i+=8;
			}
		}
		else
		{
			for(i=0;i<NandInfo.pagesize;)
			{
				*(U32*)(buf+i) = *R_NFMDATA;
				*(U32*)(buf+i+4) = *R_NFMDATA;
				*(U32*)(buf+i+8) = *R_NFMDATA;
				*(U32*)(buf+i+12) = *R_NFMDATA;
				i+=16;
			}		
		}
#endif		//NAND DMA
		while ( !(*(volatile U8*)R_NFMSTAT & 0x02) ); // for "trr" timing,see Nand-Flash data sheet
		return TRUE;
	}

}
//read only 512byte for FAT
static U8 *page_cache=0;
static int page_cache_num=-1;
BOOL nand_phy_readsect(U32 sector,void* buffer)
{
	U8* buf = (U8*)buffer;
	U32 sectsize = nand_get_sectorsize();
	if(NandInfo.pagesize==sectsize)
		return nand_phy_readpage(sector,buf);
	else
	{
		U32 pagenum;
		U32 offset;
		U32 secperpage = NandInfo.pagesize/sectsize;
		pagenum = sector/secperpage;
		offset = (sector%secperpage)*sectsize;
		if(pagenum!=page_cache_num)
		{
			if(nand_phy_readpage(pagenum,page_cache)==FALSE)
				return FALSE;
		}
		page_cache_num = pagenum;
		memcpy(buffer,page_cache+offset,512);
		return TRUE;
	}
}

BOOL nand_phy_readspare(U32 pageaddr,void *buffer,U32 len)
{
	int i;
	char* buf;
	g_dummy = *R_NFMSTAT; //clear busy
	buf = (U8*)buffer;
	if(NandInfo.pagesize == 512)
	{
		*(volatile U8 *)R_NFMCMD  = NAND_COMM_SPAREREAD ;  // Read 
		setspareaddr(pageaddr);
	}
	else
	{
		*(volatile U8 *)R_NFMCMD  = NAND_COMM_READ1 ; 
		setspareaddr(pageaddr);
		*(volatile U8 *)R_NFMCMD  = NAND_COMM_READ2 ; 
	}

	// Check Busy End
	WaitBusy();
	if(len > NandInfo.sparesize)
		len = NandInfo.sparesize;
	for(i=0;i<len;i++)
		buf[i] = *(volatile U8*)R_NFMDATA;
	// Check Busy End
	while ( !(*(volatile U8*)R_NFMSTAT & 0x02) ); // for "trr" timing,see Nand-Flash data sheet
	return TRUE;
}

BOOL nand_phy_readblock(U32 blocknum,void* buffer)
{
	int i;
	U32 startpage = BLOCK2PAGE(blocknum);
	for(i=0;i<NandInfo.pageperblock;i++)
	{
		nand_phy_readpage(startpage,buffer);
		startpage++;
		buffer+=NandInfo.pagesize;
	}	
	return TRUE;
}

BOOL nand_phy_writepage(U32 pageaddr, void* buffer)
{
	U8* buf = (U8*)buffer;

	g_dummy = *R_NFMSTAT; //clear busy
	if(NandInfo.pagesize == 512)
		*(volatile U8*)R_NFMCMD  = 0x0 ; 
	*(volatile U8*)R_NFMCMD  = NAND_COMM_PPRO1 ; 

	setpageaddress(pageaddr,0);

	if(NandInfo.eccbit==24)
	{
		U32	mlc_ecc[8][11];//8K
		int i;
		int j;
		if((U32)(&buf[0])&1)//1byte align
		{
			for(j=0;j<NandInfo.pagesize/1024;j++)
			{
				g_dummy=*R_NFMMLCECC(10);//ecc clear
				*R_NFMCON |=(1<<12);//ecc gen enable
				for(i=0;i<1024;)
				{
					*(volatile U8*)R_NFMDATA = buf[i];
					*(volatile U8*)R_NFMDATA = buf[i+1];
					*(volatile U8*)R_NFMDATA = buf[i+2];
					*(volatile U8*)R_NFMDATA = buf[i+3];
					i+=4;
				}
				buf+=i;
				for(i=0;i<11;i++)
					mlc_ecc[j][i]=*(R_NFMMLCECC(0)+i);
			}
		}
		else if((U32)(&buf[0])&2)//2byte align
		{
			U16* lbuf = (U16*)&buf[0];
			for(j=0;j<NandInfo.pagesize/1024;j++)
			{
				g_dummy=*R_NFMMLCECC(10);//ecc clear
				*R_NFMCON |=(1<<12);//ecc gen enable
				for(i=0;i<512;)
				{
					*(volatile U16*)R_NFMDATA = lbuf[i];
					*(volatile U16*)R_NFMDATA = lbuf[i+1];
					*(volatile U16*)R_NFMDATA = lbuf[i+2];
					*(volatile U16*)R_NFMDATA = lbuf[i+3];
					i+=4;
				}
				lbuf +=i;
				for(i=0;i<11;i++)
					mlc_ecc[j][i]=*(R_NFMMLCECC(0)+i);
			}
		}
		else //4byte align
		{
			U32* lbuf = (U32*)&buf[0];
			for(j=0;j<NandInfo.pagesize/1024;j++)
			{
				g_dummy=*R_NFMMLCECC(10);//ecc clear
				*R_NFMCON |=(1<<12);//ecc gen enable
				for(i=0;i<256;)
				{
					*R_NFMDATA = lbuf[i];
					*R_NFMDATA = lbuf[i+1];
					*R_NFMDATA = lbuf[i+2];
					*R_NFMDATA = lbuf[i+3];
					i+=4;
				}
				lbuf+=i;
				for(i=0;i<11;i++)
					mlc_ecc[j][i]=*(R_NFMMLCECC(0)+i);
			}
		}

		//write ecc at spare
		*(volatile U8*)R_NFMCMD  = NAND_COMM_RANDI ; 
		*(volatile U8*)R_NFMADDR = (NandInfo.pagesize+PARITY_OFFSET_24);  
		*(volatile U8*)R_NFMADDR = (NandInfo.pagesize+PARITY_OFFSET_24)>>8;  
		for(j=0;j<8;j++)
		{
			for(i=0;i<11;i++)
				*R_NFMDATA = mlc_ecc[j][i];
		}
	}
	else if(NandInfo.eccbit==4) // 2048
	{
		int i;
		int j;
		U32 eccdata[4][2];
		if((U32)(&buf[0])&1)//1byte align
		{
			for(j=0;j<NandInfo.pagesize/512;j++)
			{
				g_dummy=*R_NFMMLCECC(10);//ecc clear
				*R_NFMCON |=((1<<15)|(1<<12));//4bit ecc gen enable
				for(i=0;i<512;)
				{
					*(volatile U8*)R_NFMDATA = buf[i];
					*(volatile U8*)R_NFMDATA = buf[i+1];
					*(volatile U8*)R_NFMDATA = buf[i+2];
					*(volatile U8*)R_NFMDATA = buf[i+3];
					i+=4;
				}
				buf+=i;
				eccdata[j][0]=*R_NFMMLCECC(0);
				eccdata[j][1]=*R_NFMMLCECC(1);
			}
		}
		else if((U32)(&buf[0])&2)//2byte align
		{
			U16* lbuf = (U16*)&buf[0];
			for(j=0;j<NandInfo.pagesize/512;j++)
			{
				g_dummy=*R_NFMMLCECC(1);//ecc clear
				*R_NFMCON |=((1<<15)|(1<<12));//4bit ecc gen enable
				for(i=0;i<256;)
				{
					*(volatile U16*)R_NFMDATA = lbuf[i];
					*(volatile U16*)R_NFMDATA = lbuf[i+1];
					*(volatile U16*)R_NFMDATA = lbuf[i+2];
					*(volatile U16*)R_NFMDATA = lbuf[i+3];
					i+=4;
				}
				lbuf +=i;
				eccdata[j][0]=*R_NFMMLCECC(0);
				eccdata[j][1]=*R_NFMMLCECC(1);
			}
		}
		else //4byte align
		{
			U32* lbuf = (U32*)&buf[0];
			for(j=0;j<NandInfo.pagesize/512;j++)
			{
				g_dummy=*R_NFMMLCECC(10);//ecc clear
				*R_NFMCON |=((1<<15)|(1<<12));//4bit ecc gen enable

				for(i=0;i<128;)
				{
					*R_NFMDATA = lbuf[i];
					*R_NFMDATA = lbuf[i+1];
					*R_NFMDATA = lbuf[i+2];
					*R_NFMDATA = lbuf[i+3];
					i+=4;
				}
				lbuf+=i;
				eccdata[j][0]=*R_NFMMLCECC(0);
				eccdata[j][1]=*R_NFMMLCECC(1);
			}
		}
		*R_NFMCON &=~(1<<12);//ECC disable
		//write ecc at spare
		*(volatile U8*)R_NFMCMD  = NAND_COMM_RANDI ; 
		*(volatile U8*)R_NFMADDR = (NandInfo.pagesize+PARITY_OFFSET_4);  
		*(volatile U8*)R_NFMADDR = (NandInfo.pagesize+PARITY_OFFSET_4)>>8;  
		for(j=0;j<4;j++)
		{
			*R_NFMDATA = eccdata[j][0];
			*R_NFMDATA = eccdata[j][1];
		}
	}
	else
	{

#ifdef NAND_USE_DMA
		dma_write_nand(buf,NandInfo.pagesize);
#else//not NAND_DMA
		int i;
		if((U32)(&buf[0])&3)
		{
			for(i=0;i<NandInfo.pagesize;)
			{
				*(volatile U8*)R_NFMDATA = buf[i];
				*(volatile U8*)R_NFMDATA = buf[i+1];
				*(volatile U8*)R_NFMDATA = buf[i+2];
				*(volatile U8*)R_NFMDATA = buf[i+3];
				i+=4;
			}
		}
		else
		{
			U32* lbuf = (U32*)&buf[0];
			for(i=0;i<NandInfo.pagesize/4;)
			{
				*R_NFMDATA = lbuf[i];
				*R_NFMDATA = lbuf[i+1];
				*R_NFMDATA = lbuf[i+2];
				*R_NFMDATA = lbuf[i+3];
				i+=4;
			}
		}

#endif//NAND_DMA
	}
	
	*(volatile U8*)R_NFMCMD = NAND_COMM_PPRO2 ; // Confirm
	WaitBusy();

	*(volatile U8*)R_NFMCMD  = NAND_STATUS ; // Command
	if(*(volatile U8*)R_NFMDATA & 1)
		return FALSE;
//for nand_phy_readsect
	if(	page_cache_num == pageaddr)
		memcpy(page_cache,buffer,NandInfo.pagesize);
	return TRUE;	
}

BOOL nand_phy_writespare(U32 pageaddr,void* buffer,U32 len)
{
	int i;
	U8* buf = (U8*)buffer;
	g_dummy = *R_NFMSTAT; //clear busy

	if(NandInfo.pagesize == 512)
	{
		*(volatile U8 *)R_NFMCMD  = NAND_COMM_SPAREREAD ;  
		*(volatile U8*)R_NFMCMD  = NAND_COMM_PPRO1 ; 
		setspareaddr(pageaddr);
	}
	else
	{
		*(volatile U8 *)R_NFMCMD  = NAND_COMM_PPRO1 ;  
		setspareaddr(pageaddr);
	}

	if(len > NandInfo.sparesize)
		len = NandInfo.sparesize;
	for(i=0;i<len;i++)
	{
		*(volatile U8*)R_NFMDATA = buf[i];
	}
	*(volatile U8*)R_NFMCMD = NAND_COMM_PPRO2 ; // Confirm
	// Check Busy End
	WaitBusy();

	*(volatile U8*)R_NFMCMD  = NAND_STATUS ; // Command
	if(*(volatile U8*)R_NFMDATA & 1)
		return FALSE;
	return TRUE;	
}

BOOL nand_phy_writeblock(U32 blocknum,void* buffer)
{
	int i;
	U32 startpage = BLOCK2PAGE(blocknum);
	U8* buf = (U8*)buffer;
	for(i=0;i<NandInfo.pageperblock;i++)
	{
		if(nand_phy_writepage(startpage+i,buf)==FALSE)
			return FALSE;
		buf += NandInfo.pagesize;
	}
	return TRUE;
}
BOOL nand_phy_eraseblock(U32 blocknum)
{
	U32 pagenum = BLOCK2PAGE(blocknum);
	g_dummy = *R_NFMSTAT; //clear busy
	*(volatile char *)R_NFMCMD  = NAND_COMM_BERS1 ; 
	seteraseblockaddr(pagenum);
	*(volatile U8*)R_NFMCMD  = NAND_COMM_BERS2 ; // Command	
	// Check Busy End
	WaitBusy();

	*(volatile U8*)R_NFMCMD  = NAND_STATUS ; // Command
	if(*(volatile U8*)R_NFMDATA & 1)
		return FALSE;//bad?
	return TRUE;
}



#define BI_GOOD	0xff //not used block
#define BI_USED	0x7f //allocated block
#define BI_NOTUSED	0x0f //need for erase to check bad?
#define BI_BAD 0x00 //can't use

#define MAX_BLOCK_CNT (8192)
typedef struct _tagSpare
{
	U8 LSN[3];
	U8 paritybit;
	U8 reserve; // reserve[0] is used parity
	U8 BadInfo;
	U8 Ecc[3]; //not used
	U8 SEcc[2]; //not used
	U8 reserve2[5];

}SPAREDATA; //small page


typedef struct _tagSpare2
{
	U8 BadInfo;
	U8 paritybit;
	U8 LSN[3]; // Logical Number
	U8 reserve2[3];
	U8 Ecc[3];//not used
	U8 SEcc[2];//not used
	U8 reserve3[3];
}SPAREDATA2;//large page

static U8 BlockInfoTable[MAX_BLOCK_CNT];
static U16 PhyBlockTable[MAX_BLOCK_CNT];

static BOOL nand_make_bbt()
{
	static int badclockcnt=0;
	int i;
	int pageperblock = NandInfo.pageperblock;
	U32 logicalBlocknum;
	U8 sparedatabuf[16];
	U8 sparedatabuf2[16];
	memset(PhyBlockTable,0xff,MAX_BLOCK_CNT*2);
	for(i=NandInfo.datastartblock;i<NandInfo.blockcnt;i++)
	{
		nand_phy_readspare(i*pageperblock,sparedatabuf,16);
		if(NandInfo.eccbit==24)
			nand_phy_readspare(i*pageperblock+pageperblock-1,sparedatabuf2,16);
		else
			nand_phy_readspare(i*pageperblock+1,sparedatabuf2,16);

		if(NandInfo.pagesize==512)
		{
			SPAREDATA* sparedata = (SPAREDATA*)sparedatabuf;
			SPAREDATA* sparedata2 = (SPAREDATA*)sparedatabuf2;
			if( (sparedata->BadInfo != 0xff) || (sparedata2->BadInfo != 0xff) )
			{
				BlockInfoTable[i]=BI_BAD;	
				debugprintf("%dblock is bad block\r\n",i);
				badclockcnt++;
				continue;
			}
			//make logical block number
			logicalBlocknum = (U32)sparedata->LSN[0] + ((U32)sparedata->LSN[1]<<8) + ((U32)sparedata->LSN[2]<<16);
			if(logicalBlocknum==0xffffff)
			{
				BlockInfoTable[i]=BI_GOOD;
			}
			else
			{
				U8 paritybit = 0;
				int k;
				//check parity bit
				for(k=0;k<24;k++)
				{
					if(logicalBlocknum & (1<<k))
					{
						if(paritybit)
							paritybit = 0;
						else
							paritybit = 1;
					}
				}
				if(sparedata->paritybit != paritybit)
				{
					BlockInfoTable[i]=BI_NOTUSED;					
				}
				else
				{
					PhyBlockTable[logicalBlocknum]=i;			
					BlockInfoTable[i]=BI_USED;
					//NAND_DEBUGPRINTF("P-%d : L-%d\r\n",i,logicalBlocknum);
				}
			}
		}
		else //large block
		{
			SPAREDATA2* sparedata = (SPAREDATA2*)sparedatabuf;
			SPAREDATA2* sparedata2 = (SPAREDATA2*)sparedatabuf2;
			if( (sparedata->BadInfo != 0xff) || (sparedata2->BadInfo != 0xff) )
			{
				BlockInfoTable[i]=BI_BAD;
				badclockcnt++;
				continue;
			}
			//make logical block number
			logicalBlocknum = (U32)sparedata->LSN[0] + ((U32)sparedata->LSN[1]<<8) + ((U32)sparedata->LSN[2]<<16);
			if(logicalBlocknum==0xffffff)
			{
				BlockInfoTable[i]=BI_GOOD;
			}
			else
			{
				U8 paritybit = 0;
				int k;
				//check parity bit
				for(k=0;k<24;k++)
				{
					if(logicalBlocknum & (1<<k))
					{
						if(paritybit)
							paritybit = 0;
						else
							paritybit = 1;
					}
				}
				if(sparedata->paritybit != paritybit)
				{
					BlockInfoTable[i]=BI_NOTUSED;					
					debugprintf("%d block paritybit error\r\n",i);
				}
				else
				{
					PhyBlockTable[logicalBlocknum]=i;			
					BlockInfoTable[i]=BI_USED;
					//NAND_DEBUGPRINTF("P-%d : L-%d\r\n",i,logicalBlocknum);
				}
			}
		}
	}
	debugprintf("make bad block inforamation done(bad-block(%d))\r\n",badclockcnt);
	return TRUE;
}
/*
 * find empty block and erase it
 **/
static int nand_get_phy_freeblock(U32 logicalblocknum)
{
	int i;
	int k=logicalblocknum;
	for(i=NandInfo.datastartblock;i<NandInfo.blockcnt;i++)
	{
		if(BlockInfoTable[k]==BI_GOOD)
		{
			//if(nand_phy_eraseblock(k)==TRUE)
				return k;
		}
		else if(BlockInfoTable[i]==BI_NOTUSED)
		{
			if(nand_phy_eraseblock(k)==TRUE)
			{
				BlockInfoTable[i] = BI_GOOD;
				return k;
			}
			BlockInfoTable[k]=BI_BAD;
		}
		k++;
		if(k>=NandInfo.blockcnt)
			k=NandInfo.datastartblock;
	}
	return -1;
}

static BOOL nand_allocate_block(U32 phynum,U32 lognum)
{
	U8 sparebuf[16]={0,};
	if(phynum > NandInfo.blockcnt || lognum > NandInfo.blockcnt)
		return FALSE;

	if(NandInfo.pagesize == 512)
	{
		SPAREDATA* sparedata = (SPAREDATA*)sparebuf;
		sparedata->BadInfo=0xff;
		sparedata->LSN[0] = lognum;
		sparedata->LSN[1] = lognum>>8;
		sparedata->LSN[2] = lognum>>16;
		U8 paritybit = 0;
		int k;
		//check parity bit
		for(k=0;k<24;k++)
		{
			if(lognum & (1<<k))
			{
				if(paritybit)
					paritybit = 0;
				else
					paritybit = 1;
			}
		}
		sparedata->paritybit = paritybit;
	}
	else//2048
	{
		SPAREDATA2* sparedata = (SPAREDATA2*)sparebuf;
		sparedata->BadInfo=0xff;
		sparedata->LSN[0] = lognum;
		sparedata->LSN[1] = lognum>>8;
		sparedata->LSN[2] = lognum>>16;
		U8 paritybit = 0;
		int k;
		//check parity bit
		for(k=0;k<24;k++)
		{
			if(lognum & (1<<k))
			{
				if(paritybit)
					paritybit = 0;
				else
					paritybit = 1;
			}
		}
		sparedata->paritybit = paritybit;
	}
	U32 pagenum = phynum*NandInfo.pageperblock;
	if(nand_phy_writespare(pagenum,sparebuf,16) == FALSE)
	{
		debugprintf("pagenum:%d writespare error\r\n",pagenum);
		return FALSE;
	}
	U16 oldphynum = PhyBlockTable[lognum];
	if(oldphynum !=phynum && oldphynum <NandInfo.blockcnt)
	{
		BlockInfoTable[oldphynum]=BI_NOTUSED;
		//nand_phy_eraseblock(oldphynum);
	}
	BlockInfoTable[phynum]=BI_USED;
	PhyBlockTable[lognum]=phynum;

	return TRUE;
}
/*
static void nand_set_badblock(U32 lognum)
{
	U32 phyblocknum = PhyBlockTable[lognum];
	BlockInfoTable[phyblocknum]=BI_BAD;
	nand_phy_eraseblock(phyblocknum);
	PhyBlockTable[lognum]=0xffff;//not allocate
}
*/
U32 nand_get_phy_block(U32 lognum)
{
	U32 phyblocknum = PhyBlockTable[lognum];
	if(phyblocknum == 0xffff) //not allocate
	{
		phyblocknum = nand_get_phy_freeblock(lognum);
		if(phyblocknum ==-1)
		{
			NAND_DEBUGPRINTF("failed to get free block\r\n");
			return -1;
		}
		NAND_DEBUGPRINTF("new block allocated P-%d:L-%d\r\n",phyblocknum,lognum);
		nand_allocate_block(phyblocknum,lognum);
	}
	return phyblocknum;
}

BOOL nand_set_sparedata(U32 lognum)
{
	U32 phynum = nand_get_phy_block(lognum);
	return nand_allocate_block(phynum,lognum);
}

U32 nand_get_pagesize()
{
	return NandInfo.pagesize;
}

U32 nand_get_blocksize()
{
	return NandInfo.blocksize;
}
U32 nand_get_pageperblock()
{
	return NandInfo.blocksize/NandInfo.pagesize;
}
U32 nand_get_blockcnt()
{
	return NandInfo.blockcnt;
}
U32 nand_get_memsize_kbyte()
{
	return NandInfo.blocksize/1024 * NandInfo.datablockcnt;
}

U32 nand_get_sectorcount()
{
	return (NandInfo.blocksize * NandInfo.datablockcnt)/nand_get_sectorsize();
}


// Flash Translation Layer(FTL)

U32 nand_get_phy_page(U32 pagenum)
{
	U32 blocknum = pagenum / NandInfo.pageperblock;
	blocknum = nand_get_phy_block(blocknum);
	return (blocknum*NandInfo.pageperblock + (pagenum % NandInfo.pageperblock));
}

BOOL nand_eraseblock(U32 blocknum)
{
	U32 phyblocknum = nand_get_phy_block(blocknum);
	if(nand_phy_eraseblock(phyblocknum)==FALSE)
		return FALSE;
	nand_set_sparedata(blocknum);
	return TRUE;
}

BOOL nand_writepage(U32 pagenum,void* buf)
{
	pagenum = nand_get_phy_page(pagenum);
	return nand_phy_writepage(pagenum,buf);
}

BOOL nand_readpage(U32 pagenum,void* buf)
{
	pagenum = nand_get_phy_page(pagenum);
	return nand_phy_readpage(pagenum,buf);
}

DWORD nand_get_sectorsize()
{
	return 512;
}

typedef struct
{
	int blocknum;
	BOOL dirty; // if true, data is updated so it should be written at storage
	U8* buf;
}BLOCKBUF;
BLOCKBUF blockbuf;
/*
{
.blocknum=-1,
.dirty=0,
.buf={0,}
};
*/
static void init_blockbuf()
{
	blockbuf.blocknum=-1;
	blockbuf.dirty=FALSE;
	blockbuf.buf = malloc(nand_get_blocksize());
}
static void flushblockbuf()
{
	if(blockbuf.dirty==0)
		return;

	U32 pagesize = nand_get_pagesize();
	U32 pageperblock = nand_get_pageperblock();
	U32 startpage = blockbuf.blocknum*pageperblock;

	nand_eraseblock(blockbuf.blocknum);
	int i;
	for(i=0;i<pageperblock;i++)
	{
		nand_writepage(startpage+i,blockbuf.buf+(i*pagesize));
	}
	blockbuf.dirty=0;	
	blockbuf.blocknum = -1;
}

static void fillblockbuf(U32 blocknum)
{
	nand_phy_readblock(nand_get_phy_block(blocknum),blockbuf.buf);
	blockbuf.blocknum=blocknum;
}

void nand_flushdata()
{
	flushblockbuf();
}

static BOOL nand_updatesect(U32 sector,void* buffer)
{
	U8* buf = (U8*)buffer;
	U32 pagesize = nand_get_pagesize();
	int sectorsize=nand_get_sectorsize();
	U32 pagenum = sector*sectorsize/pagesize;
	U32 nandblocknum = pagenum/nand_get_pageperblock();

	if(blockbuf.blocknum!=nandblocknum)
	{
		flushblockbuf();
		fillblockbuf(nandblocknum);
	}
	U32 sectorperblock = NandInfo.blocksize / sectorsize;
	sector = sector % sectorperblock;
	U8* pbuf = blockbuf.buf+(sector*sectorsize);
	memcpy(pbuf,buf,sectorsize);	
	blockbuf.dirty=1;
	return TRUE;
}


BOOL nand_updatesects(U32 startsector,void* buf,U32 cnt)
{
	int i;
	int sectorsize=nand_get_sectorsize();
	U32 fatsector = NandInfo.datastartblock*nand_get_pageperblock();
	fatsector *=nand_get_pagesize()/sectorsize;
	startsector+=fatsector;
	for(i=0;i<cnt;i++)
	{
		if(nand_updatesect(startsector+i,(void*)(((U32)buf)+(i*sectorsize)))==FALSE)
			return FALSE;
	}
	return TRUE;
}
static BOOL nand_readsect(U32 sector,void* buffer)
{
	U8* buf = (U8*)buffer;
	U32 sectsize = nand_get_sectorsize();
	U32 pagesize = nand_get_pagesize();
	U32 pagenum = sector*sectsize/pagesize;
	U32 nandblocknum = pagenum/nand_get_pageperblock();
	if(blockbuf.blocknum == nandblocknum)
	{
		U32 offset = (sector*sectsize)%nand_get_blocksize();
		memcpy(buf,blockbuf.buf+offset,sectsize);
		return TRUE;
	}
	else
	{
		if(NandInfo.pagesize==sectsize)
		{
			sector = nand_get_phy_page(sector);
		}
		else
		{
			pagenum = nand_get_phy_page(pagenum);
			sector = (pagenum*(pagesize/sectsize))+(sector%(pagesize/sectsize));
		}
		BOOL re = nand_phy_readsect(sector,buf);
		return re;
	}
}


BOOL nand_readsects(U32 startsector,void* buf,U32 cnt)
{
	int i;
	U32 fatsector = NandInfo.datastartblock*nand_get_pageperblock();
	U32 sectsize = nand_get_sectorsize();
	fatsector *=nand_get_pagesize()/sectsize;
	startsector+=fatsector;
	U8* u8buf = (U8*)buf;
	for(i=0;i<cnt;i++)
	{
		if(nand_readsect(startsector+i,u8buf+(i*sectsize))==FALSE)
			return FALSE;
	}
	return TRUE;
}


static U32 nandcfg=0x2222;
void nand_set_cfg(U32 cfg)
{
	nandcfg = cfg;
}

void nand_eraseall()
{
	int i;
	for(i=0;i<NandInfo.blockcnt;i++)
	{
		if(nand_phy_eraseblock(i)==FALSE)
		{
			//debugprintf("%d block erase error\r\n",i);
		}
	}
}

BOOL nand_init()
{
	static BOOL binit=FALSE;
	if(binit)
		return TRUE;
	if(get_ahb_clock()>40000000)
		*R_NFMCFG = nandcfg|(1<<20);
	else
		*R_NFMCFG = nandcfg;

	U16 id = nand_id_read();

	int i=0;
	while(1)
	{
		if(id == nand_db[i].id)
		{
			memcpy(&NandInfo,&nand_db[i],sizeof(NandInfo));
			//NandInfo = nand_db[i];
			break;
		}
		i++;
		if(nand_db[i].id==0)
		{
			debugprintf("nand id(%x) is not supported\r\n",id);
			return FALSE;
		}
	}
	if(NandInfo.eccbit==24)
	{
		*R_NFMCON = 0;//24bit ecc mode
	}
	else
		*R_NFMCON =(1<<15);
	NandInfo.datablockcnt = NandInfo.blockcnt-NandInfo.datastartblock-NandInfo.maxbadblockcnt;
	debugprintf("Nand Flash Memory Info:%dMbyte\r\n",(NandInfo.blocksize/1024)*NandInfo.blockcnt/1024);
	init_blockbuf();
	if(page_cache==0)
	{
		page_cache = (U8*)malloc(NandInfo.pagesize);
		page_cache_num = -1;
	}
	//nand_eraseall();
	BOOL  re = nand_make_bbt();
	binit = re;
	return re;
}
