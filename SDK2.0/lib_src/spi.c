
/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"


static void inline spi_wait_finish (int ch)
{
	U8 stat;
	do {
		stat = *R_SPI0STAT(ch);
	} while (!(stat & SPISTAT_SPIF));
}

void spi_master_init(int ch)
{
	*R_SPI0CON(ch) = SPICON_MSTR |SPICON_CPOL |SPICON_CPHA| SPICON_MSBF | SPICON_EN;
}

/* 
* mode: SPI_MASTER, SPI_SLAVE
* freq: Frequency
*/
int spi_set_freq (int ch,int mode, U32 freq)
{
	U8 baudrate;

	if ((mode == SPI_MASTER) && (freq > SPI_MASTER_MAXFREQ))
		return -1;
	else if ((mode == SPI_SLAVE) && (freq > SPI_SLAVE_MAXFREQ))
		return -1;

	baudrate = (get_apb_clock() / (freq<<1)) - 1;
	*R_SPI0BAUD(ch)=baudrate;

	return baudrate;
}

void spi_master_xfer(int ch,U8 *wbuf, int wlength, U8 *rbuf,
					 int rlength, int continue_xfer)
{
	int i;
	int rcnt = 0, rlen;
	int wcnt = 0, wlen;
	U8 flush;

	if (wlength > SPI_MAX_WFIFO)
		wlen = SPI_MAX_WFIFO;
	else
		wlen = wlength;

	if (rlength > SPI_MAX_RFIFO)
		rlen = SPI_MAX_RFIFO;
	else
		rlen = rlength;

	*R_SSX0CON(ch) = 0;   // SPI nSS Low

	while (wcnt < wlength) {
		for (i = 0; i < wlen; ) {
			*R_SPI0DATA(ch) = wbuf[i];
			i++;
		}
		spi_wait_finish(ch);

		for (i = 0; i < wlen; i++)
			flush = *R_SPI0DATA(ch); // flush garbage

		wbuf += wlen;
		wcnt += wlen;
		if ((wlength - wcnt) < SPI_MAX_WFIFO)
			wlen = wlength - wcnt;
	}

	while (rcnt < rlength) {
		for (i = 0; i < rlen; ) {
			*R_SPI0DATA(ch) = 0xaa;
			spi_wait_finish(ch);
			flush = *R_SPI0DATA(ch);
			rbuf[i] = flush;
			i++;
		}

		rbuf += rlen;
		rcnt += rlen;
		if ((rlength - rcnt) < SPI_MAX_RFIFO)
			rlen = rlength - rcnt;
	}

	if (continue_xfer == 0)
		*R_SSX0CON(ch) = 1;   // SPI nSS High
}


void spi_wait_empty_fifo (int ch)
{
	U8 stat;
	do {
		stat = *R_SPI0STAT(ch);
	} while (!(stat & SPISTAT_STXE));
}
