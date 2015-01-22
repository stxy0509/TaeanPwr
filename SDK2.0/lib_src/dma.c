/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"
static U32 DSPM sval;
BOOL dma_blockmemset(void* dest,int val,int w,int h, int lpitch,int bitwidth  )
{
	U32 conval=0;
	int dmach = get_free_dma();
	if(dmach<0)
		return FALSE;
	sval=val;//define for memory allocated.
	U32 destoffset = lpitch - (w*(bitwidth/8));
	U32 transfer_width = ((bitwidth/8)>>1);
	conval |= transfer_width<<24;
	conval |= transfer_width<<20;

	conval |= (1<<29); //destination address increment
	conval |= w;//transfer size
	
	dcache_invalidate_way(); //for savl
	*R_DMASRC(dmach) = (U32)&sval;
	*R_DMADST(dmach) = (U32)dest;

	//use auto reload
	*R_DMASGA(dmach) = 0;
	*R_DMADSA(dmach) = destoffset;
	*R_DMAARCNT(dmach) = h-1;
	*R_DMACON(dmach) = conval;
	*R_DMACFG(dmach) =1;
	while((*R_DMACFG(dmach))&1);
	set_free_dma(dmach);
	return TRUE;
}

BOOL dma_memcpy(void* dest,void* src,U32 bytelen)
{
	U32 conval=0;
	int dmach = get_free_dma();
	if(dmach<0)
		return FALSE;
	U32 bitwidth=8;
	U32 transfer_size=bytelen;
	dcache_invalidate_way();
	if(bytelen>4)
	{
		if(((U32)dest & 0x3)==0) //32bit
		{
			conval |= 1<<25;
		}
		else if(((U32)dest & 0x1)==0)//16bit
		{
			conval |= 1<<24;
		}

		if(((U32)src & 0x3)==0) //32bit
		{
			conval |= 1<<21;
			transfer_size = bytelen>>2;
			bitwidth = 32;
		}
		else if(((U32)src & 0x1)==0)//16bit
		{
			conval |= 1<<20;
			transfer_size = bytelen>>1;
			bitwidth=16;
		}
	}
	/*
	if(((U32)src & 0xf ) == 0)
		conval |= DMA_CON_SRC_BST_256;
	else
		conval |= DMA_CON_SRC_BST_1;
		*/
	conval |= DMA_CON_SRC_BST_256;

	if(((U32)dest & 0xf ) == 0)
		conval |= DMA_CON_DST_BST_256;
	else
		conval |= DMA_CON_DST_BST_1;


	conval |= (3<<28); //address increment
	
	*(R_DMASRC0+dmach*8) = (U32)src;
	*(R_DMADST0+dmach*8) = (U32)dest;
	
	U32 reloadcnt = transfer_size>>11;// devide by 0x800
	if(reloadcnt)
	{
		//use auto reload
		conval |= 0x800;
		*(R_DMASGA0+dmach*8) = 0;
		*(R_DMADSA0+dmach*8) = 0;
		*(R_DMAARCNT0+dmach*8) = reloadcnt-1;
		*(R_DMACON0+dmach*8) = conval;
		*(R_DMACFG0+dmach*8) =1;
		while((*R_DMACFG(dmach))&1);
		U32 totaltx = (bitwidth/8)*0x800*reloadcnt;
		U32 remain = bytelen - totaltx;
		if(remain)
		{
			dma_memcpy(dest+totaltx,src+totaltx,remain);
			//memcpy(dest+totaltx,src+totaltx,remain);
		}
	}
	else
	{
		conval |= transfer_size;
		*(R_DMACON0+dmach*8) = conval;
		*(R_DMACFG0+dmach*8) =1;
		while((*R_DMACFG(dmach))&1);
		U32 totaltx = (bitwidth/8)*transfer_size;
		U32 remain = bytelen - totaltx;
		if(remain)
		{
			dma_memcpy(dest+totaltx,src+totaltx,remain);
			//memcpy(dest+totaltx,src+totaltx,remain);
		}
	}
	set_free_dma(dmach);
	return TRUE;
}

BOOL dma_blockcpy(void* dest,void* src,U32 destpitch,U32 srcpitch,U32 bytewidth,U32 h)
{
	U32 conval=0;
	U32 transfer_size = bytewidth;
	U32 srcoffset;
	U32 destoffset;
	U32 bitwidth =8;
	
	int dmach = get_free_dma();
	if(dmach<0)
	{
		debugstring("All DMA is busy\r\n");
		return FALSE;
	}
	srcoffset = srcpitch -bytewidth;
	destoffset = destpitch -bytewidth;
	if((bytewidth & 3)==0)
	{
		if(((U32)dest & 0x3)==0) //32bit
		{
			conval |= 1<<25;
		}
		else if(((U32)dest & 0x1)==0)//16bit
		{
			conval |= 1<<24;
		}

		if(((U32)src & 0x3)==0) //32bit
		{
			conval |= 1<<21;
			transfer_size = bytewidth>>2;
			bitwidth = 32;
		}
		else if(((U32)src & 0x1)==0)//16bit
		{
			conval |= 1<<20;
			transfer_size = bytewidth>>1;
			bitwidth=16;
		}
	}
	else if((bytewidth&1)==0)
	{
		if(((U32)dest & 0x1)==0)//16bit
		{
			conval |= 1<<24;
		}
		if(((U32)src & 0x1)==0)//16bit
		{
			conval |= 1<<20;
			transfer_size = bytewidth>>1;
			bitwidth=16;
		}
	}
	/*
	if(((U32)src & 0xf ) == 0)
		conval |= DMA_CON_SRC_BST_256;
	else
		conval |= DMA_CON_SRC_BST_1;
		*/
	conval |= DMA_CON_SRC_BST_256;

	if(((U32)dest & 0xf ) == 0)
		conval |= DMA_CON_DST_BST_256;
	else
		conval |= DMA_CON_DST_BST_1;

	conval |= (3<<28);
	conval |= (transfer_size &0xfff);

	*(R_DMASRC0+dmach*8) = (U32)src;
	*(R_DMADST0+dmach*8) = (U32)dest;
	*(R_DMASGA0+dmach*8) = srcoffset;
	*(R_DMADSA0+dmach*8) = destoffset;
	*(R_DMAARCNT0+dmach*8) = h-1;
	*(R_DMACON0+dmach*8) = conval;
	*(R_DMACFG0+dmach*8) =1;
	while((*R_DMACFG(dmach))&1);
	set_free_dma(dmach);
	return TRUE;
}
static bool dma_used[8]={false,false,false,false,false,false,false,false};
int get_free_dma()
{
	int i;
	CRITICAL_ENTER();
	for(i=0;i<8;i++)
	{
		if(dma_used[i]==false)
		{
			dma_used[i]=true;
			CRITICAL_EXIT();
			return i;
		}
	}
	CRITICAL_EXIT();
	return -1;
}
void set_free_dma(int ch)
{
	CRITICAL_ENTER();
	if((ch>=0)&&(ch<8))
	{
		*R_DMACFG(ch)=0xff<<4; //set dest, src as reserved
		dma_used[ch]=false;
	}
	CRITICAL_EXIT();
}
