#include "egl_config.h"
#include "egl_typedef.h"

int DecodeUTF8(const char *encodedBuffer, unsigned int *outLength)
{
	int n ;
	const unsigned char *buf = (const unsigned char*)encodedBuffer;
	int value = 0;
	int length = -1;
	unsigned char byte = buf[0];
	if( (byte & 0x80) == 0 )
	{
		// This is the only byte
		if( outLength ) *outLength = 1;
		return byte;
	}
	else if( (byte & 0xE0) == 0xC0 )
	{
		// There is one more byte
		value = (int)(byte & 0x1F);
		length = 2;

		// The value at this moment must not be less than 2, because 
		// that should have been encoded with one byte only.
		if( value < 2 )
			length = -1;
	}
	else if( (byte & 0xF0) == 0xE0 )
	{
		// There are two more bytes
		value = (int)(byte & 0x0F);
		length = 3;
	}
	else if( (byte & 0xF8) == 0xF0 )
	{
		// There are three more bytes
		value = (int)(byte & 0x07);
		length = 4;
	}

	n = 1;
	for( ; n < length; n++ )
	{
		byte = buf[n];
		if( (byte & 0xC0) == 0x80 )
			value = (value << 6) + (int)(byte & 0x3F);
		else 
			break;
	}

	if( n == length )
	{
		if( outLength ) *outLength = (unsigned)length;
		return value;
	}

	// The byte sequence isn't a valid UTF-8 byte sequence.
	return -1;
}

int EncodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outLength)
{
	unsigned char *buf = (unsigned char*)outEncodedBuffer;
	int n;
	int length = -1;

	if( value <= 0x7F )
	{
		buf[0] = value;
		if( outLength ) *outLength = 1;
		return 1;
	}
	else if( value >= 0x80 && value <= 0x7FF )
	{
		// Encode it with 2 characters
		buf[0] = 0xC0 + (value >> 6);
		length = 2;
	}
	else if( ((value >= 0x800) && (value <= 0xD7FF)) || ((value >= 0xE000) && (value <= 0xFFFF)))
	{
		// Note: Values 0xD800 to 0xDFFF are not valid unicode characters
		buf[0] = 0xE0 + (value >> 12);
		length = 3;
	}
	else if( value >= 0x10000 && value <= 0x10FFFF )
	{
		buf[0] = 0xF0 + (value >> 18);
		length = 4;
	}

	n = length-1;
	for( ; n > 0; n-- )
	{
		buf[n] = 0x80 + (value & 0x3F);
		value >>= 6;
	}

	if( outLength ) *outLength = length;
	return length;
}

int DecodeUTF16(const char *encodedBuffer, unsigned int *outLength)
{
	const unsigned char *buf = (const unsigned char *)encodedBuffer;
	int value = 0;
	value += buf[0];
	value += (unsigned int)(buf[1]) << 8; 
	
	if( value < 0xD800 || value > 0xDFFF )
	{
		if( outLength ) *outLength = 2;
		return value;
	}
	else if( value < 0xDC00 )
	{
		int value2 = 0;
		// We've found the first surrogate word
		value = ((value & 0x3FF)<<10);

		// Read the second surrogate word
		value2 += buf[2];
		value2 += (unsigned int)(buf[3]) << 8; 

		// The second surrogate word must be in the 0xDC00 - 0xDFFF range
		if( value2 < 0xDC00 || value2 > 0xDFFF )
			return -1;

		value = value + (value2 & 0x3FF) + 0x10000;
		if( outLength ) *outLength = 4;
		return value;
	}

	// It is an illegal sequence if a character in the 0xDC00-0xDFFF range comes first
	return -1;
}

int EncodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength)
{
	if( value < 0x10000 )
	{
		outEncodedBuffer[0] = (value & 0xFF);
		outEncodedBuffer[1] = ((value >> 8) & 0xFF);
		if( outCharLength ) *outCharLength = 2;
		return 2;
	}
	else
	{
		int surrogate1 ;
		int surrogate2 ;
		value -= 0x10000;
		surrogate1 = ((value >> 10) & 0x3FF) + 0xD800;
		surrogate2 = (value & 0x3FF) + 0xDC00;

		outEncodedBuffer[0] = (surrogate1 & 0xFF);
		outEncodedBuffer[1] = ((surrogate1 >> 8) & 0xFF);
		outEncodedBuffer[2] = (surrogate2 & 0xFF);
		outEncodedBuffer[3] = ((surrogate2 >> 8) & 0xFF);
		if( outCharLength ) *outCharLength = 4;
		return 4;
	}
}
#ifndef _CODE_PAGE
#define _CODE_PAGE	CONFIG_CODE_PAGE
#endif
#if CONFIG_CODE_PAGE == 932
#include "cp/cc932.c"

#elif CONFIG_CODE_PAGE == 936
#include "cp/cc936.c"

#elif CONFIG_CODE_PAGE == 949
#include "cp/cc949.c"

#elif CONFIG_CODE_PAGE == 950
#include "cp/cc950.c"

#else 
#include "cp/ccsbcs.c"
#endif


int oem_to_unicode(unsigned short src)
{
	return ff_convert(src,1);
}
int unicode_to_oem(unsigned short src)
{
	return ff_convert(src,0);
}
