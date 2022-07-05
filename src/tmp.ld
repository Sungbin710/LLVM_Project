ENTRY(Reset_Handler)

MEMORY
{
  FLASH(rx):ORIGIN =0x08000000,LENGTH =1024K
  SRAM(rwx):ORIGIN =0x20000000,LENGTH =128K
}


SECTIONS
{

  .text :
  {
    *(.isr_vector)
    *(.text)
    *(EXCLUDE_FILE(*output_opt5.o).text.*)
    *(.init)
    *(.fini)
    *(.rodata)
    *(.rodata.*)
	*(.MPU)
    . = ALIGN(4);
    _etext = .;
  }> FLASH


  .function 0x08004000 : 
  {
	*(.f1)
	. = ALIGN(512);
	*(.text.*)
	. = ALIGN(256);
	*(.f2)
	. = ALIGN(256);
	*(.f3)
	. = ALIGN(128);
	*(.f4)
	. = ALIGN(128);
	*(.f5)
	. = ALIGN(128);
	*(.f6)
	. = ALIGN(128);
	*(.f7)
	. = ALIGN(128);
	*(.f8)
	. = ALIGN(128);
	*(.f9)
	. = ALIGN(64);
  }

  _la_data = LOADADDR(.data);

  .data :
  {
    _sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
  }> SRAM AT> FLASH

  .bss :
  {
    _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
           . = ALIGN(4);
        end = .;
        __end__ = .;
  }> SRAM
}

