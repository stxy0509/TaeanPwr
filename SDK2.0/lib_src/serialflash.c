#include "adstar.h"

static int flash_sector_size=4*1024;
static int flash_size=512*1024;
void flash_erase_sector(int sector,int len)
{
	int i;
	CacheDisable();
	U32 mode = *R_FLMOD;
	*R_FLMOD &= ~(1<<7);//write enable
	U32 addr = sector*flash_sector_size;
	for(i=0;i<len;i++)
	{
		*R_FLSEA=addr;
		addr += flash_sector_size;
	}
	*R_FLMOD = mode;
	CacheEnable();
}

void flash_write(U32 addr, BYTE* buf, int len)
{
	CacheDisable();
	U32 mode = *R_FLMOD;
	*R_FLMOD &= ~(1<<7);//write enable
	memcpy((void*)addr,(void*)buf,len);	
	*R_FLMOD = mode;
	CacheEnable();
}

void flash_read(U32 addr, BYTE* buf, int len)
{
	memcpy((void*)buf,(void*)addr,len);
}

int flash_get_sectorsize()
{
	return flash_sector_size;
}

int flash_get_size()
{
	return flash_size;
}
