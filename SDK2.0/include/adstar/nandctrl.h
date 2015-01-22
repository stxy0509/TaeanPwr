/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#pragma once

/***********************************************
* NAND FLASH CONTROLLER
***********************************************/
#define NAND_FLASH_CONTROL_BASE 0xA00000C00
#define R_NFMCON		((volatile unsigned int*)0xA0000c00)#define R_NFMCMD		((volatile unsigned int*)0xA0000c04)
#define R_NFMADDR		((volatile unsigned int*)0xA0000c08)
#define R_NFMDATA		((volatile unsigned int*)0xA0000c0c)
#define R_NFMSTAT		((volatile unsigned int*)0xA0000c14)
#define R_NFMECC		((volatile unsigned int*)0xA0000c18)
#define R_NFMCFG		((volatile unsigned int*)0xA0000c1c)
#define R_NFMECCLSN		((volatile unsigned int*)0xA0000c20)
#define R_NFMECD		((volatile unsigned int*)0xA0000c24)
#define R_NFMSPADDR		((volatile unsigned int*)0xA0000c28)
#define R_NFMMLCECC(n)		((volatile unsigned int*)(0xA0000c2c+(n*4)))
#define R_NFMELOC(n)	((volatile unsigned int*)(0xA0000c58+(n*4)))
#define R_NFMEPTN(n)	((volatile unsigned int*)(0xA0000cB8+(n*4)))
#define R_NFMID			((volatile unsigned int*)0xA0000d18)



// NAND Command Define
#define NAND_COMM_READ1	0x00	// NAND Read 1st Cycle
#define NAND_COMM_READ1_2	0x01	// NAND Read 2nd harf register
#define NAND_COMM_READ2	0x30	// NAND Read 2st Cycle
#define NAND_COMM_SPAREREAD	0x50 // NAND Read spare area
#define NAND_COMM_RID	0x90	// NAND Read ID
#define NAND_COMM_RESET	0xff	// NAND Reset
#define NAND_COMM_PPRO1	0x80	// NAND Page Program 1st Cycle
#define NAND_COMM_PPRO2 0x10	// NAND Page Program 2st Cycle
#define NAND_COMM_BERS1	0x60	// NAND Block Erase 1st Cycle
#define NAND_COMM_BERS2	0xd0	// NAND Block Erase 2st Cycle
#define NAND_COMM_RANDI	0x85	// NAND Random Data Input
#define NAND_COMM_RANDO1	0x05	// NAND Random Data Output 1st Cycle
#define NAND_COMM_RANDO2	0xe0	// NAND Random Data Output 2st Cycle
#define NAND_STATUS		0x70	// NAND Status Read


//sector means 512 byte memory
BOOL nand_phy_readpage(U32 pagenum, void* buf);
BOOL nand_phy_writepage(U32 pagenum, void* buf);
BOOL nand_phy_eraseblock(U32 blocknum);
BOOL nand_phy_readspare(U32 pagenum,void *buffer,U32 len);
BOOL nand_phy_writespare(U32 pagenum,void* buffer,U32 len);
BOOL nand_phy_readblock(U32 blocknum,void* buffer);
BOOL nand_phy_writeblock(U32 blocknum,void* buffer);
BOOL nand_phy_isbadblock(U32 blocknum);

BOOL nand_set_sparedata(U32 lognum);
void nand_set_cfg(U32 cfg);
BOOL nand_init();
U32 nand_get_pagesize();
U32 nand_get_blocksize();
U32 nand_get_pageperblock();
U32 nand_get_blockcnt();
U32 nand_get_memsize_kbyte();
U32 nand_get_phy_block(U32 lognum);

BOOL nand_eraseblock(U32 logblocknum);
BOOL nand_readpage(U32 pagenum,void* buf);
BOOL nand_writepage(U32 pagenum,void* buf);
void nand_flushdata();

// for file-system
BOOL nand_phy_readsect(U32 sector,void* buf);//read only 512 byte
U32 nand_get_sectorcount();//sector is 512
DWORD nand_get_sectorsize();
BOOL nand_readsects(U32 sector,void* buf,U32 cnt);
BOOL nand_updatesects(U32 sector,void* buf,U32 cnt);


