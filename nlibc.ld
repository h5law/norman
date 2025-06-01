/* nlibc.ld: linker script for norman libc for the RP2350
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
__RAM_SIZE = 512K;

__SCRATCH_X_BASE = 0x20080000;
__SCRATCH_Y_BASE = 0x20081000;
__SCRATCH_SIZE = 4K;

/* heap just after bss section */
__HEAP_SIZE  = 4K;

/* stack at the end of memory, growing down */
__STACK_SIZE = 16K;

/* Entry point defaults to start of .text section(__FLASH_BASE)
 * Defines the following symbols for use by code:
 *  __exidx_start
 *  __exidx_end
 *  __etext
 *  __data_start__
 *  __preinit_array_start
 *  __preinit_array_end
 *  __init_array_start
 *  __init_array_end
 *  __fini_array_start
 *  __fini_array_end
 *  __data_end__
 *  __bss_start__
 *  __bss_end__
 *  __end__
 *  end
 *  __HeapLimit
 *  __StackLimit
 *  __StackTop
 *  __stack(== StackTop)
 */
ENTRY(_start)

MEMORY
{
  FLASH(rx)  : ORIGIN = __FLASH_BASE, LENGTH = __FLASH_SIZE
  RAM(rwx) : ORIGIN = __RAM_BASE,   LENGTH = __RAM_SIZE
}

PHDRS
{
  text_fvector PT_LOAD;
  text PT_LOAD;
  ram_init PT_LOAD;
  ram PT_LOAD;
  tls_init PT_TLS;
  tls PT_TLS;
}

SECTIONS
{
 .flash_begin :
  {
    __flash_binary_start = .;
  } >FLASH

  .vectors :
  {
    KEEP(*(.vectors))
  } >FLASH AT>FLASH :text

  .text :
  {
    __logical_binary_start = .;
    KEEP(*(.vectors))
    KEEP(*(.reset))

    *(.text*)
    *(.gnu.linkonce.t.*)
    KEEP(*(.init .fini.*))
    __text_end = .;

    PROVIDE(__etext = __text_end);
    PROVIDE(__etext = __text_end);
    PROVIDE(_etext = __text_end);

    . = ALIGN(4);
    PROVIDE(__preinit_array_start = .);
    KEEP(*(SORT(.preinit_array.*)))
    KEEP(*(.preinit_array))
    PROVIDE(__preinit_array_end = .);

    . = ALIGN(4);
    PROVIDE(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE(__init_array_end = .);

    . = ALIGN(4);
    PROVIDE(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE(__fini_array_end = .);
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
  PROVIDE(__plt_start = ADDR(.plt));
  PROVIDE(__plt_size = SIZEOF(.plt));

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
    __data_start = .;
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    PROVIDE(__global_pointer$ = . + 0x800);
    PROVIDE(_gp = . + 0x8000);
    *(.sdata .sdata.* .sdata2.*)
    *(.gnu.linkonce.s.*)
    PROVIDE(__stack_chk_start = .);
    KEEP(*(.stack_chk .stack_chk.*))
    PROVIDE(__stack_chk_end = .);
  } >RAM AT>FLASH :ram_init
  PROVIDE(__data_start = ADDR(.data));
  PROVIDE(__data_source = LOADADDR(.data));

  .tdata :
  {
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    PROVIDE(__data_end = .);
    PROVIDE(__tdata_end = .);
  } >RAM AT>FLASH :ram_init
  PROVIDE(__tdata_start = ADDR(.tdata));
  PROVIDE(__tdata_source = LOADADDR(.tdata));
  PROVIDE(__tdata_source_end = LOADADDR(.tdata) + SIZEOF(.tdata));
  PROVIDE(__data_source_end = __tdata_source_end);
  PROVIDE(__tdata_size = SIZEOF(.tdata));
  PROVIDE(__data_start = ADDR(.data));
  PROVIDE(__data_source = LOADADDR(.data));

  PROVIDE(__edata = __data_end);
  PROVIDE(__edata = __data_end);
  PROVIDE(_edata = __data_end);
  PROVIDE(__data_size = __data_end - __data_start);
  PROVIDE(__data_source_size = __data_source_end - __data_source);

  .tbss(NOLOAD) :
  {
    . = ALIGN(4);
    *(.tbss .tbss.* .gnu.linkonce.tb.*)
    *(.tcommon)
    PROVIDE(__tls_end = .);
    PROVIDE(__tbss_end = .);
  } >RAM AT>RAM :ram
  PROVIDE(__bss_start = ADDR(.tbss));
  PROVIDE(__tbss_start = ADDR(.tbss));
  PROVIDE(__tbss_offset = ADDR(.tbss) - ADDR(.tdata));
  PROVIDE(__tbss_size = SIZEOF(.tbss));
  PROVIDE(__tls_size = __tls_end - __tls_base );
  PROVIDE(__tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)));
  PROVIDE(__tls_size_align =(__tls_size + __tls_align - 1) & ~(__tls_align - 1));
  PROVIDE(__arm32_tls_tcb_offset = MAX(8, __tls_align));
  PROVIDE(__arm64_tls_tcb_offset = MAX(16, __tls_align));

  .tbss_space(NOLOAD) :
  {
    . = ADDR(.tbss);
    . = . + SIZEOF(.tbss);
  } >RAM AT>RAM :ram

  .bss(NOLOAD) :
  {
    . = ALIGN(__tls_align);
    PROVIDE(__tls_tail_extra_start = .);
    KEEP(*(.tls_tail_extra .tls_tail_extra.*))
    PROVIDE(__tls_tail_extra_end = .);
    PROVIDE(__non_tls_bss_start = .);
    *(.sbss*)
   *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  } >RAM AT>RAM :ram
  PROVIDE(__non_tls_bss_start = ADDR(.bss));
  PROVIDE(__end = __bss_end);
  __end = __bss_end;
  PROVIDE(_end = __bss_end);
  PROVIDE(__bss_size = __bss_end - __bss_start);

  .heap(NOLOAD) :
  {
    . +=(DEFINED(__heap_size) ? __heap_size : __HEAP_SIZE);
  } >RAM :ram
  PROVIDE(__HeapLimit = ORIGIN(RAM) + LENGTH(RAM));
  PROVIDE(__heap_start = ADDR(.heap));
  PROVIDE(__heap_end = ADDR(.heap) + SIZEOF(.heap));
  PROVIDE(__heap_size = __heap_end - __heap_start);

  .stack(NOLOAD) :
  {
	. +=(DEFINED(__stack_size) ? __stack_size : __STACK_SIZE);
  } >RAM :ram
  PROVIDE(__StackLimit = ORIGIN(RAM) + LENGTH(RAM));
  PROVIDE(__max_heap = __StackLimit);
  PROVIDE(__StackTop = ADDR(.stack) + SIZEOF(.stack));
  PROVIDE(__StackBottom = ADDR(.stack));
  PROVIDE(__stack_limit = __StackBottom);
  PROVIDE(__stack = __StackTop);

  .flash_end :
  {
    PROVIDE(__flash_binary_end = .);
  } >FLASH AT>FLASH =0xaa

  /* Discard debugging and exception sections */
  /DISCARD/ :
  {
    *(.note .note.*)
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
}

ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed");
ASSERT(__data_size == __data_source_size,
  "ERROR: .data/.tdata flash size does not match RAM size");
