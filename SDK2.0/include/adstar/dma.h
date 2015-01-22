/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

#define _DMA_BASE_	0x80001400

#define R_DMAIS		((volatile U32 *)(_DMA_BASE_+0))
#define R_DMATCIS	((volatile U32 *)(_DMA_BASE_+4))
#define R_DMATCIC	((volatile U32 *)(_DMA_BASE_+8))
#define R_DMAEIS	((volatile U32 *)(_DMA_BASE_+0xc))
#define R_DMAEIC	((volatile U32 *)(_DMA_BASE_+0x10))
#define R_DMABIS	((volatile U32 *)(_DMA_BASE_+0x14))
#define R_DMABIC	((volatile U32 *)(_DMA_BASE_+0x18))

#define R_DMARTCIS	((volatile U32 *)(_DMA_BASE_+0x1c))
#define R_DMACEIS	((volatile U32 *)(_DMA_BASE_+0x20))
#define R_DMAECS	((volatile U32 *)(_DMA_BASE_+0x24))
#define R_DMASBR	((volatile U32 *)(_DMA_BASE_+0x28))
#define R_DMASSR	((volatile U32 *)(_DMA_BASE_+0x2c))
#define R_DMASLBR	((volatile U32 *)(_DMA_BASE_+0x30))
#define R_DMASLSR	((volatile U32 *)(_DMA_BASE_+0x34))

#define R_DMASRC(ch)	((volatile U32 *)(_DMA_BASE_+0x100+(0x20*(ch))))
#define R_DMASRC0	((volatile U32 *)(_DMA_BASE_+0x100))
#define R_DMASRC1	((volatile U32 *)(_DMA_BASE_+0x120))
#define R_DMASRC2	((volatile U32 *)(_DMA_BASE_+0x140))
#define R_DMASRC3	((volatile U32 *)(_DMA_BASE_+0x160))
#define R_DMASRC4	((volatile U32 *)(_DMA_BASE_+0x180))
#define R_DMASRC5	((volatile U32 *)(_DMA_BASE_+0x1a0))
#define R_DMASRC6	((volatile U32 *)(_DMA_BASE_+0x1c0))
#define R_DMASRC7	((volatile U32 *)(_DMA_BASE_+0x1e0))

#define R_DMADST(ch)	((volatile U32 *)(_DMA_BASE_+0x104+(0x20*(ch))))
#define R_DMADST0	((volatile U32 *)(_DMA_BASE_+0x104))
#define R_DMADST1	((volatile U32 *)(_DMA_BASE_+0x124))
#define R_DMADST2	((volatile U32 *)(_DMA_BASE_+0x144))
#define R_DMADST3	((volatile U32 *)(_DMA_BASE_+0x164))
#define R_DMADST4	((volatile U32 *)(_DMA_BASE_+0x184))
#define R_DMADST5	((volatile U32 *)(_DMA_BASE_+0x1a4))
#define R_DMADST6	((volatile U32 *)(_DMA_BASE_+0x1c4))
#define R_DMADST7	((volatile U32 *)(_DMA_BASE_+0x1e4))


#define R_DMACON(ch)	((volatile U32 *)(_DMA_BASE_+0x10c+(0x20*(ch))))
#define R_DMACON0	((volatile U32 *)(_DMA_BASE_+0x10c))
#define R_DMACON1	((volatile U32 *)(_DMA_BASE_+0x12c))
#define R_DMACON2	((volatile U32 *)(_DMA_BASE_+0x14c))
#define R_DMACON3	((volatile U32 *)(_DMA_BASE_+0x16c))
#define R_DMACON4	((volatile U32 *)(_DMA_BASE_+0x18c))
#define R_DMACON5	((volatile U32 *)(_DMA_BASE_+0x1ac))
#define R_DMACON6	((volatile U32 *)(_DMA_BASE_+0x1cc))
#define R_DMACON7	((volatile U32 *)(_DMA_BASE_+0x1ec))

#define R_DMACFG(ch)	((volatile U32 *)(_DMA_BASE_+0x110+(0x20*(ch))))
#define R_DMACFG0	((volatile U32 *)(_DMA_BASE_+0x110))
#define R_DMACFG1	((volatile U32 *)(_DMA_BASE_+0x130))
#define R_DMACFG2	((volatile U32 *)(_DMA_BASE_+0x150))
#define R_DMACFG3	((volatile U32 *)(_DMA_BASE_+0x170))
#define R_DMACFG4	((volatile U32 *)(_DMA_BASE_+0x190))
#define R_DMACFG5	((volatile U32 *)(_DMA_BASE_+0x1b0))
#define R_DMACFG6	((volatile U32 *)(_DMA_BASE_+0x1d0))
#define R_DMACFG7	((volatile U32 *)(_DMA_BASE_+0x1f0))

#define R_DMASGA(ch)	((volatile U32 *)(_DMA_BASE_+0x114 + (0x20*(ch))))
#define R_DMASGA0	((volatile U32 *)(_DMA_BASE_+0x114))
#define R_DMASGA1	((volatile U32 *)(_DMA_BASE_+0x134))
#define R_DMASGA2	((volatile U32 *)(_DMA_BASE_+0x154))
#define R_DMASGA3	((volatile U32 *)(_DMA_BASE_+0x174))
#define R_DMASGA4	((volatile U32 *)(_DMA_BASE_+0x194))
#define R_DMASGA5	((volatile U32 *)(_DMA_BASE_+0x1b4))
#define R_DMASGA6	((volatile U32 *)(_DMA_BASE_+0x1d4))
#define R_DMASGA7	((volatile U32 *)(_DMA_BASE_+0x1f4))



#define R_DMADSA(ch)	((volatile U32 *)(_DMA_BASE_+0x118 + (0x20*(ch))))
#define R_DMADSA0	((volatile U32 *)(_DMA_BASE_+0x118))
#define R_DMADSA1	((volatile U32 *)(_DMA_BASE_+0x138))
#define R_DMADSA2	((volatile U32 *)(_DMA_BASE_+0x158))
#define R_DMADSA3	((volatile U32 *)(_DMA_BASE_+0x178))
#define R_DMADSA4	((volatile U32 *)(_DMA_BASE_+0x198))
#define R_DMADSA5	((volatile U32 *)(_DMA_BASE_+0x1b8))
#define R_DMADSA6	((volatile U32 *)(_DMA_BASE_+0x1d8))
#define R_DMADSA7	((volatile U32 *)(_DMA_BASE_+0x1f8))


#define R_DMAARCNT(ch)	((volatile U32 *)(_DMA_BASE_+0x11C+(0x20*(ch))))
#define R_DMAARCNT0	((volatile U32 *)(_DMA_BASE_+0x11C))
#define R_DMAARCNT1	((volatile U32 *)(_DMA_BASE_+0x13C))
#define R_DMAARCNT2	((volatile U32 *)(_DMA_BASE_+0x15C))
#define R_DMAARCNT3	((volatile U32 *)(_DMA_BASE_+0x17C))
#define R_DMAARCNT4	((volatile U32 *)(_DMA_BASE_+0x19C))
#define R_DMAARCNT5	((volatile U32 *)(_DMA_BASE_+0x1bC))
#define R_DMAARCNT6	((volatile U32 *)(_DMA_BASE_+0x1dC))
#define R_DMAARCNT7	((volatile U32 *)(_DMA_BASE_+0x1fC))

#define DMA_CON_SRC_INCR	(1<<28)
#define DMA_CON_DST_INCR	(1<<29)

#define DMA_CON_DST_8BIT	(0)
#define DMA_CON_DST_16BIT	(1<<24)
#define DMA_CON_DST_32BIT	(1<<25)
#define DMA_CON_SRC_8BIT	(0)
#define DMA_CON_SRC_16BIT	(1<<20)
#define DMA_CON_SRC_32BIT	(1<<21)

#define DMA_CON_DST_BST_1	(0)
#define DMA_CON_DST_BST_4	(1<<16)
#define DMA_CON_DST_BST_8	(2<<16)
#define DMA_CON_DST_BST_16	(3<<16)
#define DMA_CON_DST_BST_32	(4<<16)
#define DMA_CON_DST_BST_64	(5<<16)
#define DMA_CON_DST_BST_128	(6<<16)
#define DMA_CON_DST_BST_256	(7<<16)

#define DMA_CON_SRC_BST_1	(0)
#define DMA_CON_SRC_BST_4	(1<<12)
#define DMA_CON_SRC_BST_8	(2<<12)
#define DMA_CON_SRC_BST_16	(3<<12)
#define DMA_CON_SRC_BST_32	(4<<12)
#define DMA_CON_SRC_BST_64	(5<<12)
#define DMA_CON_SRC_BST_128	(6<<12)
#define DMA_CON_SRC_BST_256	(7<<12)


typedef enum {
	DMA_PERI_NAND_TX=0,
	DMA_PERI_SDHC,
	DMA_PERI_NAND_RX,
	DMA_PERI_USB_DEVICE_TX,
	DMA_PERI_USB_DEVICE_RX,
	DMA_PERI_SNDCH0,
	DMA_PERI_SNDCH1,
	DMA_PERI_SNDCH2,
	DMA_PERI_SNDCH3,
	DMA_PERI_SNDREC,
}DMA_DEVICE;

int get_free_dma();//when DMA is not used anymore, should be call set_free_dma().
void set_free_dma(int ch);

BOOL dma_blockmemset(void* dest,int val,int w,int h, int lpitch,int bitwidth  );
BOOL dma_memcpy(void* dest,void* src,U32 bytelen);
BOOL dma_blockcpy(void* dest,void* src,U32 destpitch,U32 srcpitch,U32 w,U32 h);

