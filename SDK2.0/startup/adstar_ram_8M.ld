OUTPUT_FORMAT("elf32-ae32000-little", "elf32-ae32000-big",
			"elf32-ae32000-little")
OUTPUT_ARCH(ae32000)
ENTRY(_Reset_Start)
/*GROUP(-lc -lgcc -lm)*/
/*SEARCH_DIR(.);*/

MEMORY
{
    rom (rx)	: ORIGIN = 0x00000000,	LENGTH = 512K
	ispm(!rx)	: ORIGIN = 0x10000000,	LENGTH = 2K
	/* vector table will stored at 0x18000000 by InitInterrupt()*/
	dspm(rx)	: ORIGIN = 0x18000150,	LENGTH = (30*1024-0x150)
	ram (!rx)	: ORIGIN = 0x20000000,	LENGTH = 8M
}

SECTIONS
{
  .text : {
    *(.vects)
    *(.text)
    . = ALIGN(4);
    *(.stub)
    *(.rodata)
    *(.rodata.str1.*)
    *(.rodata1)
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    ___dtors_end = . ;
	*(.gnu.linkonce.*) 
	*(.gcc_except_table) 
	CONSTRUCTORS

  } > ram
  _etext = . ;
  PROVIDE (etext = .);
  .data ALIGN(0x4) :
  {
    . = ALIGN(4);
    __ram_data_start = . ;
    /*PROVIDE (___ram_data_start = .);*/
    *(.data)
    *(.sdata)
    . = ALIGN(4);
    __ram_data_end = .;
    /*PROVIDE (___ram_data_end = .);*/
  } > ram
  __rom_data_start  =  LOADADDR (.data);
  /*PROVIDE (___rom_data_start = .);*/

   .bss ALIGN(0x4) :
	{
    __bss_start = .;
    /*PROVIDE (__bss_start = .);*/
    *(.dynbss)
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
    /*PROVIDE (___bss_end = .);*/
  } > ram
	_end = . ;
  PROVIDE (end = .);


  .ispm : AT ( (LOADADDR (.data) + SIZEOF(.data) + 3) & ~ (4 - 1))
  {
    . = ALIGN(4);
    __ram_ispm_start = . ;
    *(.ispm)
    . = ALIGN(4);
    __ram_ispm_end = .;
  } > ispm
  __rom_ispm_start  =  LOADADDR (.ispm);

  .dspm : AT ( (LOADADDR (.ispm) + SIZEOF(.ispm) + 3) & ~ (4 - 1)) 
  {
    . = ALIGN(4);
    __ram_dspm_start = . ;
    *(.dspm)
    . = ALIGN(4);
    __ram_dspm_end = .;
  } > dspm
  __rom_dspm_start  =  LOADADDR (.dspm);



  ._stack 0x207FFFFC : 
  /*._stack 0x180077FC: *//*internal DSPM*/
  {
    _stack = .;
    /*PROVIDE (__stack = .);*/
    *(._stack)
  } 
}
PROVIDE(___data_size = SIZEOF(.data));
PROVIDE(___bss_size = SIZEOF(.bss));


