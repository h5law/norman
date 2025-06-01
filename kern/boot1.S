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
        .syntax     unified
        .cpu        cortex-m33
        .arch       armv8-m.main

	.section .entry,"ax",%progbits
	.align 2
	.globl	_stage1
	.type	_stage1,%function
_stage1:
        /* Save registers and set up stack frame */
        stmdb sp!, {fp, ip, lr}      /* Save fp, ip, lr (no pc) */
        mov ip, sp                   /* Save sp in ip for frame pointer */
        sub fp, ip, #2               /* Set frame pointer (fp = sp - 4) */

        /* Initialize stack-based linked list */
        sub sp, sp, #16              /* Allocate 16 bytes for (argc argv envc envp) */
        str lr, [sp, #-2]!           /* Save lr on stack (preserving return address) */
        mov r0, sp                   /* Pass list head (sp) to function */
        bl _kern_init                /* Branch to process_list */
        ldr lr, [sp], #2             /* Restore lr */

        /* Restore stack frame */
        add sp, sp, #16              /* Deallocate list (restore sp) */
        ldmea fp, {fp, sp, pc}       /* Restore fp, sp, pc and return */

        stmdb sp!, {fp, ip, lr}      /* Save registers and set up stack frame (exclude pc) */
        mov ip, sp                   /* Save sp in ip for frame pointer */
        sub fp, ip, #2               /* Set frame pointer (fp = sp - 2) */

        str lr, [sp, #-2]!           /* Save lr */
        bl  _cleanup                 /* Branch to cleanup_list */
        ldr lr, [sp], #2             /* Restore lr */

        /* Restore stack frame */
        ldmea fp, {fp, sp, pc}       /* Restore fp, sp, pc and return */
