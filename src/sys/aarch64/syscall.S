/*
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

    .global syscall
    .text
    .align 8
    .section .syscall,"ax",%progbits
    .type syscall,%function
syscall:
    /* Move syscall number into x0 from x8 */
    mov     x8, x0
    /* Shift syscall arguments down one register from x1... to x0... */
    mov     x0, x1
    mov     x1, x2
    mov     x2, x3
    mov     x3, x4
    mov     x4, x5
    mov     x5, x6
    svc 0x80

    /* Check is x0 negative (x0 + 4096 < 0) */
    cmn     x0, #(4095 + 1)
    cneg    x0, x0, hi
    b.hi    __set_errno_internal

    ret

__set_errno_internal:
    mov x0, x0
    bl set_errno
    ret

// vim: ft=asm ts=4 sts=4 sw=4 et ai cin
