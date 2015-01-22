/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

typedef enum 
{
	NONE,
	UTF8,
	UTF16
}EFontTextEncoding;

typedef struct 
{
	int id;
	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;
}SCharDescr;



typedef struct  _tagFont
{
	short fontHeight; // total height of the font
	short base;       // y of base line
	short scaleW;
	short scaleH;
	SCharDescr defChar;
	int Outlinesize;
	int pagecnt;

	int alphaChnl;
	int redChnl;
	int greenChnl;
	int blueChnl;

	EFontTextEncoding encoding;

	srRGB font_clr;///not used yet
	srRGB outline_clr;//not used yet

	int charcount;
	SCharDescr* chars;
	SURFACE** surfs;

	int kerning;
	short fontsize;
}BMFont;

BMFont* bmfont_init(const char *fontFile);
bool bmfont_setfont(BMFont* pFont);
int bmfont_close(BMFont* pFont);
bool bmfont_setrgb(BMFont* pFont,U8 r, U8 g, U8 b);//only for 8bit
bool bmfont_setautokerning(bool b);
int bmfont_gettextwidth(const char *text, int count);//return text width in pixels

int bmfont_write(int x, int y, const char *text);//return text width in pixels
int bmfont_write_vleft( int x, int y, const char *text );
int bmfont_write_vright( int x, int y, const char *text );
void bmfont_settextencoding(EFontTextEncoding encoding);
bool bmfont_setkerning(BMFont* pFont,int k);
SURFACE* bmfont_makesurface(char* text);

