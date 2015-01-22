/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "adstar.h"
#include "fatfs/diskio.h"

static U32 org_int_mask[2];
static U32 fs_int_disable[2]={0,};

/*
if you use file-io function at ISR, you should register interrupt-number
*/
void fs_enable_support_interrupt(int intnum,BOOL b)
{
	int n=0;
	if(intnum>31)
	{
		n=1;
		intnum-=32;
	}
	if(b)
		fs_int_disable[n]|=1<<intnum;
	else
		fs_int_disable[n]&=~(1<<intnum);

}

#if _FS_REENTRANT
/*
These functions is used for only firmware.
if you use FreeRTOS, Use semaphore
*/

/* Create a sync object */
int  __attribute__((weak)) ff_cre_syncobj  (BYTE b, _SYNC_t* t)
{
	*t = 1;
	return 1;
}

/* Lock sync object */
int __attribute__((weak)) ff_req_grant (_SYNC_t t)
{
	/*register Interrupt Number what used filesystem
	DMA(Sound) ISR is use FS
	*/

	org_int_mask[0]= *R_INTMASK(0);
	org_int_mask[1]= *R_INTMASK(1);
	
	*R_INTMASKCLR(0) = fs_int_disable[0];
	*R_INTMASKCLR(1) = fs_int_disable[1];

	return 1;
}
/* Unlock sync object */
void __attribute__((weak)) ff_rel_grant (_SYNC_t t)		
{
	*R_INTMASK(0) = org_int_mask[0];
	*R_INTMASK(1) = org_int_mask[1];
}
/* Delete a sync object */
int __attribute__((weak))  ff_del_syncobj (_SYNC_t t)
{
	return 1;
}
#endif


#if CONFIG_NAND == 0
#define nand_init(...)	0
#define nand_readsects(...)	0
#define nand_updatesects(...)	0
#define nand_get_sectorcount()	0
#define nand_getsectorsize() 0
#define SET_NAND_PINMUX
#else
#if (CONFIG_SDCARD != 0) && (SDCARD_PIN_USE_67 == 0)
#define SET_NAND_PINMUX do{\
	U32 val = *R_PAF1;\
	val &= 0xf;\
	*R_PAF1 = val;\
	*R_PAF2 = 0;\
	}while(0)
#else
#define SET_NAND_PINMUX
#endif

#endif

//avoid SDCARD api linking 
#if CONFIG_SDCARD == 0
#define sdhc_init(...)	0
#define sdhc_read_sect(...)	0
#define sdhc_write_sect(...)	0
#define sdhc_getsectorcount()	0
#define sdhc_getsectorsize() 0
#define SET_SDCARD_PINMUX 
#else
#if (CONFIG_NAND != 0) && (SDCARD_PIN_USE_67 == 0)
#define SET_SDCARD_PINMUX do{\
	U32 val = *R_PAF1;\
	val &=  ~(0xf<<6);\
	val |= (1<<6)|(1<<8);\
	*R_PAF1 = val;\
	val = *R_PAF2;\
	val &=0xff;\
	*R_PAF2 = val | 0x5500;\
	}while(0)
#else
#define SET_SDCARD_PINMUX 
#endif
#endif
	



DSTATUS disk_initialize ( BYTE drv)
{
	if(drv == DRIVE_NAND )
	{
		SET_NAND_PINMUX;
		if(nand_init())
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD)
	{
		SET_SDCARD_PINMUX;
		if(sdhc_init())
			return RES_OK;
	}	
	else
		debugprintf("%d driver not supported\r\n",drv);
	return RES_ERROR;
}


/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
					 BYTE drv		/* Physical drive nmuber (0..) */
					 )
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
				   BYTE drv,		/* Physical drive nmuber (0..) */
				   BYTE *buff,		/* Data buffer to store read data */
				   DWORD sector,	/* Sector address (LBA) */
				   BYTE count		/* Number of sectors to read (1..255) */
				   )
{
	if(drv == DRIVE_NAND )
	{
		SET_NAND_PINMUX;
		if(nand_readsects(sector,buff,count))
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD )
	{
		SET_SDCARD_PINMUX;
		if(sdhc_read_sect(buff,sector,count))
		return RES_OK;
	}
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

DRESULT disk_write (
					BYTE drv,			/* Physical drive nmuber (0..) */
					const BYTE *buff,	/* Data to be written */
					DWORD sector,		/* Sector address (LBA) */
					BYTE count			/* Number of sectors to write (1..255) */
					)
{
	if(drv == DRIVE_NAND)
	{
		SET_NAND_PINMUX;
		if(nand_updatesects(sector,(void*)buff,count))
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD)
	{
		SET_SDCARD_PINMUX;
		if( sdhc_write_sect((void*)buff,sector,count) )
			return RES_OK;
	}
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	DRESULT res =RES_PARERR ;

	switch (ctrl) {
	case CTRL_SYNC:
		if(drv==DRIVE_NAND)
		{
			SET_NAND_PINMUX;
#if CONFIG_NAND == 1			
			nand_flushdata();
#endif		
		}
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		if(drv==DRIVE_NAND)
		{
			SET_NAND_PINMUX;
			*(DWORD*)buff = nand_get_sectorcount();
		}
		else if(drv==DRIVE_SDCARD)
		{
			SET_SDCARD_PINMUX;
			*(DWORD*)buff = sdhc_getsectorcount();
		}
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:
		if(drv==DRIVE_NAND)
		{
			SET_NAND_PINMUX;
			*(WORD*)buff = nand_get_sectorsize();
		}
		else if(drv==DRIVE_SDCARD)
		{
			SET_SDCARD_PINMUX;
			*(WORD*)buff = sdhc_getsectorsize();
		}
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
		*(DWORD*)buff = 1;
		res = RES_OK;
		break;
	}

	return res;
}
/*
Currnet time is returned with packed into a DWORD value. The bit field is as follows:

bit31:25 
Year from 1980 (0..127) 
bit24:21 
Month (1..12) 
bit20:16 
Day in month(1..31) 
bit15:11 
Hour (0..23) 
bit10:5 
Minute (0..59) 
bit4:0 
Second / 2 (0..29) 
*/
// DWORD get_fattime (void)
// {
	
// 	return 0;
// }
