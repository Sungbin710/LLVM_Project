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

