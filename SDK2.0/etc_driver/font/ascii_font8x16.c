#include "adstar.h"

const U8 EngFont8x16[]={	// FontSize: 8 X 16 
	
//	0x20	 
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x21	!
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x3c,	// ..QQQQ..
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x22	"
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x23	#
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x7f,	// .QQQQQQQ
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x7f,	// .QQQQQQQ
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x24	$
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x60,	// .QQ.....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	
//	0x25	%
	0x00,	// ........
	0x00,	// ........
	0x70,	// .QQQ....
	0xd8,	// QQ.QQ...
	0xda,	// QQ.QQ.Q.
	0x76,	// .QQQ.QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x6e,	// .QQ.QQQ.
	0x5b,	// .Q.QQ.QQ
	0x1b,	// ...QQ.QQ
	0x0e,	// ....QQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x26	&
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x38,	// ..QQQ...
	0x6c,	// .QQ.QQ..
	0x6c,	// .QQ.QQ..
	0x38,	// ..QQQ...
	0x60,	// .QQ.....
	0x6f,	// .QQ.QQQQ
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3b,	// ..QQQ.QQ
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x27	'
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x28	(
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x00,	// ........
	0x00,	// ........
	
//	0x29	)
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x00,	// ........
	0x00,	// ........
	
//	0x2a	*
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x36,	// ..QQ.QQ.
	0x1c,	// ...QQQ..
	0x7f,	// .QQQQQQQ
	0x1c,	// ...QQQ..
	0x36,	// ..QQ.QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x2b	+
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x7e,	// .QQQQQQ.
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x2c	,
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	
//	0x2d	-
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x2e	.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x2f	/
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x30	0
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x6e,	// .QQ.QQQ.
	0x6e,	// .QQ.QQQ.
	0x66,	// .QQ..QQ.
	0x76,	// .QQQ.QQ.
	0x76,	// .QQQ.QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x31	1
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x0c,	// ....QQ..
	0x1c,	// ...QQQ..
	0x7c,	// .QQQQQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x32	2
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x33	3
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x06,	// .....QQ.
	0x1c,	// ...QQQ..
	0x06,	// .....QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x34	4
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x66,	// .QQ..QQ.
	0x7f,	// .QQQQQQQ
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x35	5
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7c,	// .QQQQQ..
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x78,	// .QQQQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x36	6
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x37	7
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x38	8
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x76,	// .QQQ.QQ.
	0x3c,	// ..QQQQ..
	0x6e,	// .QQ.QQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x39	9
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x38,	// ..QQQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x3a	:
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x3b	;
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1c,	// ...QQQ..
	0x1c,	// ...QQQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	
//	0x3c	<
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x3d	=
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x3e	>
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x60,	// .QQ.....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x3f	?
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x40	@
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0xc3,	// QQ....QQ
	0xc3,	// QQ....QQ
	0xcf,	// QQ..QQQQ
	0xdb,	// QQ.QQ.QQ
	0xdb,	// QQ.QQ.QQ
	0xcf,	// QQ..QQQQ
	0xc0,	// QQ......
	0x7f,	// .QQQQQQQ
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x41	A
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7e,	// .QQQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x42	B
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x43	C
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x44	D
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x78,	// .QQQQ...
	0x6c,	// .QQ.QQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x6c,	// .QQ.QQ..
	0x78,	// .QQQQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x45	E
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7c,	// .QQQQQ..
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x46	F
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7c,	// .QQQQQ..
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x47	G
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x6e,	// .QQ.QQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x48	H
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7e,	// .QQQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x49	I
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4a	J
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4b	K
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x6c,	// .QQ.QQ..
	0x6c,	// .QQ.QQ..
	0x78,	// .QQQQ...
	0x6c,	// .QQ.QQ..
	0x6c,	// .QQ.QQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4c	L
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4d	M
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x77,	// .QQQ.QQQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4e	N
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x73,	// .QQQ..QQ
	0x7b,	// .QQQQ.QQ
	0x6f,	// .QQ.QQQQ
	0x67,	// .QQ..QQQ
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x4f	O
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x50	P
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x51	Q
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x00,	// ........
	0x00,	// ........
	
//	0x52	R
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x6c,	// .QQ.QQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x53	S
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x60,	// .QQ.....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x54	T
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x55	U
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x56	V
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x57	W
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x63,	// .QQ...QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x58	X
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x34,	// ..QQ.Q..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x2c,	// ..Q.QQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x59	Y
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x5a	Z
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x5b	[
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x3c,	// ..QQQQ..
	0x00,	// ........
	
//	0x5c	"\"
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0xc6,	// QQ...QQ.
	0xd6,	// QQ.Q.QQ.
	0xd6,	// QQ.Q.QQ.
	0xff,	// QQQQQQQQ
	0xd6,	// QQ.Q.QQ.
	0xd6,	// QQ.Q.QQ.
	0x6c,	// .QQ.QQ..
	0x6c,	// .QQ.QQ..
	0x6c,	// .QQ.QQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x5d	]
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x3c,	// ..QQQQ..
	0x00,	// ........
	
//	0x5e	^
	0x00,	// ........
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x5f	_
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0xff,	// QQQQQQQQ
	0x00,	// ........
	
//	0x60	`
	0x00,	// ........
	0x38,	// ..QQQ...
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x61	a
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x3e,	// ..QQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x62	b
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x63	c
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x64	d
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x3e,	// ..QQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x65	e
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7e,	// .QQQQQQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x66	f
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x1e,	// ...QQQQ.
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x7e,	// .QQQQQQ.
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x67	g
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3e,	// ..QQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x7c,	// .QQQQQ..
	0x00,	// ........
	
//	0x68	h
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x69	i
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	0x78,	// .QQQQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x6a	j
	0x00,	// ........
	0x00,	// ........
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x0c,	// ....QQ..
	0x78,	// .QQQQ...
	0x00,	// ........
	
//	0x6b	k
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x6c,	// .QQ.QQ..
	0x78,	// .QQQQ...
	0x6c,	// .QQ.QQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x6c	l
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x78,	// .QQQQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x6d	m
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x63,	// .QQ...QQ
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x6e	n
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x6f	o
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x70	p
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7c,	// .QQQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x7c,	// .QQQQQ..
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x00,	// ........
	
//	0x71	q
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3e,	// ..QQQQQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x00,	// ........
	
//	0x72	r
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x6e,	// .QQ.QQQ.
	0x70,	// .QQQ....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x73	s
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x3e,	// ..QQQQQ.
	0x60,	// .QQ.....
	0x60,	// .QQ.....
	0x3c,	// ..QQQQ..
	0x06,	// .....QQ.
	0x06,	// .....QQ.
	0x7c,	// .QQQQQ..
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x74	t
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x7e,	// .QQQQQQ.
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x30,	// ..QQ....
	0x1e,	// ...QQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x75	u
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3e,	// ..QQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x76	v
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x77	w
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x63,	// .QQ...QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x6b,	// .QQ.Q.QQ
	0x36,	// ..QQ.QQ.
	0x36,	// ..QQ.QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x78	x
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x18,	// ...QQ...
	0x3c,	// ..QQQQ..
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x79	y
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x66,	// .QQ..QQ.
	0x3c,	// ..QQQQ..
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0xf0,	// QQQQ....
	0x00,	// ........
	
//	0x7a	z
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........

//	0x7b	
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x60,	// .QQ.....
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x00,	// ........
	0x00,	// ........
	
//	0x7c	|
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x00,	// ........
	
//	0x7d	}
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x30,	// ..QQ....
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x0c,	// ....QQ..
	0x06,	// .....QQ.
	0x0c,	// ....QQ..
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x18,	// ...QQ...
	0x30,	// ..QQ....
	0x00,	// ........
	0x00,	// ........
	
//	0x7e	~
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x71,	// .QQQ...Q
	0xdb,	// QQ.QQ.QQ
	0x8e,	// Q...QQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	
//	0x7f	
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x7e,	// .QQQQQQ.
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
	0x00,	// ........
};


