/* linker.ld: linker script for RP2350
Copyright (c) 2025 h5law <dev@h5law.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */

__FLASH_BASE = 0x08000000;
__FLASH_SIZE = 2048K;

__RAM_BASE = 0x20000000;
__RAM_SIZE = 640K;

/* heap just after bss section */
__HEAP_SIZE  = 1K;

/* stack at the end of memory, growing down */
__STACK_SIZE = 1K;

/* Entry point defaults to start of .text section (__FLASH_BASE) */
/*ENTRY(Reset_Handler)*/

MEMORY
{
  RAM   (rwx) : ORIGIN = __RAM_BASE,   LENGTH = __RAM_SIZE
  FLASH (rx)  : ORIGIN = __FLASH_BASE, LENGTH = __FLASH_SIZE
}

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
    . = ALIGN(8);
  } >FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(8);
    _sdata = .;
    *(.data*)
    . = ALIGN(8);
    _edata = .;

  } >RAM AT> FLASH

  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    *(.bss*)
    . = ALIGN(8);
    _ebss = .;
    end = .; /* end is used by _sbrk call */
  } >RAM

  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } >RAM

  .stack (COPY) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
  } >RAM

}


/* vim: ft=ld ts=4 sts=4 sw=4 et ai cin */
