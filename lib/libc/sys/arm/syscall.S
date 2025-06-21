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
    .syntax unified

    .text
    .align 4
    .palign 4,,15
    .globl syscall
    .type syscall,%function
#ifdef __thumb__
    .thumb
#endif
syscall:
    /* Move syscall number into r0 from r7 */
    mov     r7, r0
    /* Shift syscall arguments down one register from r1... to r6... */
    mov     r0, r1
    mov     r1, r2
    mov     r2, r3
    svc     #0

    /* Check if r0 is negative (r0 < 0) */
    cmn     r0, #4096
    blt     .Lcall_set_errno
    bx      lr

.Lcall_set_errno:
    /* Preserve r0 (the error code) by moving it to r1 */
    mov     r1, r0
    /* Load the address of set_errno into a register */
    ldr     r12, =set_errno
    /* Call set_errno(r1) via the register */
    blx     r12
    /* Set return value to 1 */
    mov     r0, #1
    bx      lr
    .size syscall, . - syscall

// vim: ft=asm ts=4 sts=4 sw=4 et ai cin
