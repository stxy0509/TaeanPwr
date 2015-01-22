#include "adstar.h"

void debugerror(SDKERROR err,void* p1)
{
	switch(err)
	{
	case SDKERROR_NOMEM:
		debugstring("insufficient invalid Memory\r\n");
		break;
	case SDKERROR_FILEREAD:
		break;
	case SDKERROR_FILEWRITE:
		break;
	case SDKERROR_FILESEEK:
		break;
	case SDKERROR_UNSUPPORTED:
		break;
	case SDKERROR_FILEOPEN:
		DEBUGPRINTF("\"%s\" not found\r\n",p1);
		break;
	case SDKERROR_LAST:
		break;
	}
}
