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

    .section .vectors, "ax"
    .align 11  /* 2KB alignment */
    .globl vector_table
vector_table:
    /* Current EL with SP0 */
    b sync_invalid_el1t   /* Synchronous EL1t */
    b irq_invalid_el1t    /* IRQ EL1t */
    b fiq_invalid_el1t    /* FIQ EL1t */
    b error_invalid_el1t  /* Error EL1t */
    /* Current EL with SPx */
    b sync_invalid_el1h   /* Synchronous EL1h */
    b irq_invalid_el1h    /* IRQ EL1h */
    b fiq_invalid_el1h    /* FIQ EL1h */
    b error_invalid_el1h  /* Error EL1h */
    /* Lower EL using AArch64 */
    b sync_invalid_el0_64 /* Synchronous EL0_64 */
    b irq_invalid_el0_64  /* IRQ EL0_64 */
    b fiq_invalid_el0_64  /* FIQ EL0_64 */
    b error_invalid_el0_64/* Error EL0_64 */
    /* Lower EL using AArch32 */
    b sync_invalid_el0_32 /* Synchronous EL0_32 */
    b irq_invalid_el0_32  /* IRQ EL0_32 */
    b fiq_invalid_el0_32  /* FIQ EL0_32 */
    b error_invalid_el0_32/* Error EL0_32 */

sync_invalid_el1t:
irq_invalid_el1t:
fiq_invalid_el1t:
error_invalid_el1t:
sync_invalid_el1h:
irq_invalid_el1h:
fiq_invalid_el1h:
error_invalid_el1h:
sync_invalid_el0_64:
irq_invalid_el0_64:
fiq_invalid_el0_64:
error_invalid_el0_64:
sync_invalid_el0_32:
irq_invalid_el0_32:
fiq_invalid_el0_32:
error_invalid_el0_32:
    /* Basic exception handler rint ESR and ELR, then hang */
    ldr x0, =exception_msg
    bl print_string

    mrs x0, ESR_EL1       /* Read Exception Syndrome Register */
    mrs x1, ELR_EL1       /* Read Exception Link Register */

    /* Print ESR */
    mov x3, #0x09000000   /* UART base address */
    strb w0, [x3]
    /* Print ELR */
    strb w1, [x3]
    b hang

exception_msg:
    .ascii "[exception] Undefined Instruction occurred\n\0"

hang:
    b hang

    .text
    .align 0
    .globl _start
    .type _start,%function
_start:
    ldr x0, =vector_table
    msr VBAR_EL1, x0

    ldr x0, =stack_top
    mov sp, x0

    ldr x0, =boot_msg
    bl print_string

boot_kernel:
    ldr x0, =jump_to_kernel_msg
    bl print_string

    ldr x0, =kernel_entry
    br x0

end:
    b end  /* Infinite loop to halt */

print_string:
    mov x3, #0x09000000   /* UART base address */
print_loop:
    ldrb w2, [x0], #1     /* Load a byte from the string */
    cmp w2, #0            /* Check if null byte */
    b.eq print_done       /* If null byte, finish */
    strb w2, [x3]         /* Write the byte to UART */
    b print_loop          /* Loop to next character */
print_done:
    ret

    .section .data
boot_msg:
    .ascii "[boot] Boot init completed\n\0"
jump_to_kernel_msg:
    .ascii "[boot] Jumping to ROLLO kernel...\n\0"

    .section .bss
    .align 16
.stack:
    .skip 0x1000          /* 4KB stack */
stack_top:
