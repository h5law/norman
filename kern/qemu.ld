/*
 * Copyright (c) 2025 h5law <dev@h5law.com>
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
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

ENTRY(_start)

PHDRS
{
    text    PT_LOAD FLAGS(5); /* Read + Execute (RX) */
    rodata  PT_LOAD FLAGS(4); /* Read-only (R) */
    data    PT_LOAD FLAGS(6); /* Read + Write (RW) */
    bss     PT_LOAD FLAGS(6); /* Read + Write (RW) */
}

SECTIONS
{
    . = 0x40000000;  /* Kernel entry address */

    .vectors : {
        *(.vectors*) /* Exception vector table */
    } :text

    .text : {
        *(.text*)   /* All .text sections (code) */
    } :text

    .rodata : {
        *(.rodata*) /* Read-only data */
    } :rodata

    .data : {
        *(.data*)   /* Data sections */
    } :data

    .bss : {
        *(.bss*)    /* BSS (zero-initialized) sections */
        *(COMMON*)
    } :bss
}
