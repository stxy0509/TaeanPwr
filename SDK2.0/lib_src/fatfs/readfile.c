#include "adstar.h"
char* readfile(char* fname,int* len)
{
	FIL fp;
	int size;
	char* pngbuf;
	U32 nRead;
	FRESULT res = f_open(&fp,fname,FA_READ|FA_OPEN_EXISTING);
	if( res != FR_OK )
	{
		DEBUGPRINTF("Cannot open : %s\r\n", fname);
		return 0;
	}
	size = f_size(&fp);
	pngbuf=malloc(size);
	if(pngbuf==0)
		return 0;
	f_read(&fp,pngbuf,size,&nRead);
	f_close(&fp);
	*len=size;
	return pngbuf;
}

