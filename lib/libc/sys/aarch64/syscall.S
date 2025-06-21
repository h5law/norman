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
    .text
    .align 0
    .globl syscall
    .type syscall,%function
syscall:
    /* Move syscall number into x0 from x8 */
    mov     x8, x0
    /* Shift syscall arguments down one register from x1... to x7... */
    mov     x0, x1
    mov     x1, x2
    mov     x2, x3
    mov     x3, x4
    mov     x4, x5
    mov     x5, x6
    svc     #0

    /* Check if x0 is negative (x0 < 0) */
    cmn     x0, #(4095 + 1)
    cneg    x0, x0, hi
    b.hi    .Lcall_set_errno

    ret

.Lcall_set_errno:
    /* Preserve x0 (the error code) by moving it to x1 */
    mov     x1, x0
    /* Load the address of set_errno into a register */
    ldr     x12, =set_errno
    /* Call set_errno(x1) via the register */
    blr     x12
    /* Set return value to 1 */
    mov     x0, #1
    ret
    .size syscall, . - syscall

// vim: ft=asm ts=4 sts=4 sw=4 et ai cin
