/* nlibc.pico2w.ld: linker script for norman libc for the RP2350
 * Copyright(c) 2025 h5law <dev@h5law.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *  claim that you wrote the original software. If you use this software
 *  in a product, an acknowledgement in the product documentation would be
 *  appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *  misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

__FLASH_BASE = 0x10000000;
__FLASH_SIZE = 4096K;

__RAM_BASE = 0x20000000;
__RAM_SIZE = 520K;

/* heap just after bss section */
__DEFAULT_HEAP_SIZE  = 4K;

/* stack at the end of memory, growing down */
__DEFAULT_STACK_SIZE = 16K;

/* Entry point defaults to start of .text section(__FLASH_BASE)
 * Defines the following symbols for use by code:
 *  __exidx_star__t
 *  __exidx_en__d
 *  __etex__t
 *  __data_start____
 *  __preinit_array_star__t
 *  __preinit_array_en__d
 *  __init_array_star__t
 *  __init_array_en__d
 *  __fini_array_star__t
 *  __fini_array_en__d
 *  __data_end____
 *  __bss_start____
 *  __bss_end____
 *  __end____
 *  end
 *  __HeapLimit
 *  __StackLimit
 *  __StackTop
 *  __stack(__== StackTop)
 */
MEMORY
{
  FLASH(rx)  : ORIGIN = __FLASH_BASE, LENGTH = __FLASH_SIZE
  RAM(rwx)   : ORIGIN = __RAM_BASE,   LENGTH = __RAM_SIZE
}

SECTIONS
{
 .flash_begin :
  {
    __flash_binary_start__ = .;
  } >FLASH

  .vectors :
  {
    KEEP(*(.vectors))
  } >FLASH AT>FLASH :text

  .text :
  {
    __logical_binary_start__ = .;
    KEEP(*(.vectors))
    *(.embedded_block)
    KEEP(*(.embedded_block))
    KEEP(*(.reset))

    *(.text .text.*)
    *(.gnu.linkonce.t.*)
    KEEP(*(.init .fini.*))
    __text_end__ = .;

    PROVIDE(__etext__ = __text_end__);
    PROVIDE(__etext__ = __text_end__);
    PROVIDE(_etext = __text_end__);

    . = ALIGN(4);
    PROVIDE(__preinit_array_start__ = .);
    KEEP(*(SORT(.preinit_array.*)))
    KEEP(*(.preinit_array))
    PROVIDE(__preinit_array_end__ = .);

    . = ALIGN(4);
    PROVIDE(__init_array_start__ = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE(__init_array_end__ = .);

    . = ALIGN(4);
    PROVIDE(__fini_array_start__ = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE(__fini_array_end__ = .);
  } >FLASH AT>FLASH :text

  .rodata :
  {
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.srodata .srodata.*)
    . = ALIGN(4);
  } >FLASH AT>FLASH :text
  PROVIDE(_pid_base = ADDR(.rodata));

  .data.rel.ro :
  {
    *(.data.rel.ro .data.rel.ro.*)
  } >FLASH AT>FLASH :text

  .plt :
  {
    *(.rela.iplt)
    *(.rela.*)
  } >FLASH AT>FLASH :text
  PROVIDE(__plt_start__ = ADDR(.plt));
  PROVIDE(__plt_size__ = SIZEOF(.plt));

  .got :
  {
    *(.got.plt)
    *(.got)
  } >FLASH AT>FLASH :text

  .toc :
  {
    *(.toc .toc.*)
    *(.iplt .iplt.*)
  } >FLASH AT>FLASH :text

  .data :
  {
    __data_start__ = .;
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    PROVIDE(__global_pointer$__ = . + 0x800);
    PROVIDE(_gp = . + 0x8000);
    *(.sdata .sdata.* .sdata2.*)
    *(.gnu.linkonce.s.*)
    PROVIDE(__stack_chk_start__ = .);
    KEEP(*(.stack_chk .stack_chk.*))
    PROVIDE(__stack_chk_end__ = .);
  } >RAM AT>FLASH :ram_init
  PROVIDE(__data_start__ = ADDR(.data));
  PROVIDE(__data_source__ = LOADADDR(.data));

  .tdata :
  {
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    PROVIDE(__data_end__ = .);
    PROVIDE(__tdata_end__ = .);
  } >RAM AT>FLASH :ram_init
  PROVIDE(__tdata_start__ = ADDR(.tdata));
  PROVIDE(__tdata_source__ = LOADADDR(.tdata));
  PROVIDE(__tdata_source_end__ = LOADADDR(.tdata) + SIZEOF(.tdata));
  PROVIDE(__data_source_end__ = __tdata_source_end__);
  PROVIDE(__tdata_size__ = SIZEOF(.tdata));
  PROVIDE(__data_start__ = ADDR(.data));
  PROVIDE(__data_source__ = LOADADDR(.data));

  PROVIDE(__edata__ = __data_end__);
  PROVIDE(__edata__ = __data_end__);
  PROVIDE(_edata = __data_end__);
  PROVIDE(__data_size__ = __data_end__ - __data_start__);
  PROVIDE(__data_source_size__ = __data_source_end__ - __data_source__);

  .tbss(NOLOAD) :
  {
    . = ALIGN(4);
    *(.tbss .tbss.* .gnu.linkonce.tb.*)
    *(.tcommon)
    PROVIDE(__tls_end__ = .);
    PROVIDE(__tbss_end__ = .);
  } >RAM AT>RAM :ram
  PROVIDE(__bss_start__ = ADDR(.tbss));
  PROVIDE(__tbss_start__ = ADDR(.tbss));
  PROVIDE(__tbss_offset__ = ADDR(.tbss) - ADDR(.tdata));
  PROVIDE(__tbss_size__ = SIZEOF(.tbss));
  PROVIDE(__tls_size__ = __tls_end__ - __tls_base__ );
  PROVIDE(__tls_align__ = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)));
  PROVIDE(__tls_size_align__ =(__tls_size__ + __tls_align__ - 1) & ~(__tls_align__ - 1));
  PROVIDE(__arm32_tls_tcb_offset__ = MAX(8, __tls_align__));

  .tbss_space(NOLOAD) :
  {
    . = ADDR(.tbss);
    . = . + SIZEOF(.tbss);
  } >RAM AT>RAM :ram

  .bss(NOLOAD) :
  {
    . = ALIGN(__tls_align__);
    PROVIDE(__tls_tail_extra_start__ = .);
    KEEP(*(.tls_tail_extra .tls_tail_extra.*))
    PROVIDE(__tls_tail_extra_end__ = .);
    PROVIDE(__non_tls_bss_start__ = .);
    *(.sbss*)
   *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } >RAM AT>RAM :ram
  PROVIDE(__non_tls_bss_start__ = ADDR(.bss));
  PROVIDE(__end__ = __bss_end__);
  __end__ = __bss_end__;
  PROVIDE(_end = __bss_end__);
  PROVIDE(end = __bss_end__);
  PROVIDE(__bss_size__ = __bss_end__ - __bss_start__);

  .heap(NOLOAD) :
  {
    . +=(DEFINED(__HEAP_SIZE) ? __HEAP_SIZE : __DEFAULT_HEAP_SIZE);
  } >RAM :ram
  PROVIDE(__HeapLimit = ORIGIN(RAM) + LENGTH(RAM));
  PROVIDE(__heap_start__ = ADDR(.heap));
  PROVIDE(__heap_end__ = ADDR(.heap) + SIZEOF(.heap));
  PROVIDE(__heap_size__ = __heap_end__ - __heap_start__);

  .stack(NOLOAD) :
  {
	. +=(DEFINED(__STACK_SIZE) ? __STACK_SIZE : __DEFAULT_STACK_SIZE);
  } >RAM :ram
  PROVIDE(__StackLimit = ORIGIN(RAM) + LENGTH(RAM));
  PROVIDE(__max_heap = __StackLimit);
  PROVIDE(__StackTop = ADDR(.stack) + SIZEOF(.stack));
  PROVIDE(__StackBottom = ADDR(.stack));
  PROVIDE(__stack_limit = __StackBottom);
  PROVIDE(__stack = __StackTop);

  .flash_end :
  {
    PROVIDE(__flash_binary_end__ = .);
  } >FLASH AT>FLASH =0xaa

  /* Discard debugging and exception sections */
  /DISCARD/ :
  {
    *(.eh_frame .eh_frame.*)
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.ARM.exidx*)
  }

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }

  /* DWARF debug sections.
   * Symbols in the DWARF debugging sections are relative to the beginning
   * of the section so we begin them at 0.
   */
  /* DWARF 1.  */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }

  /* GNU DWARF 1 extensions.  */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }

  /* DWARF 1.1 and DWARF 2.  */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  /* DWARF 2.  */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

  /* SGI/MIPS DWARF 2 extensions.  */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

  /* DWARF 3.  */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }

  /* DWARF 5.  */
  .debug_addr     0 : { *(.debug_addr) }
  .debug_line_str 0 : { *(.debug_line_str) }
  .debug_loclists 0 : { *(.debug_loclists) }
  .debug_macro    0 : { *(.debug_macro) }
  .debug_names    0 : { *(.debug_names) }
  .debug_rnglists 0 : { *(.debug_rnglists) }
  .debug_str_offsets 0 : { *(.debug_str_offsets) }
  .debug_sup      0 : { *(.debug_sup) }
  .gnu.attributes 0 : { KEEP(*(.gnu.attributes)) }
};

ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed");
ASSERT(__data_size__ == __data_source_size__,
  "ERROR: .data/.tdata flash size does not match RAM size");
