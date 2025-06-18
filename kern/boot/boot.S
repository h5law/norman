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
    .globl _start
    .type _start,%function
/* Start of the bootloader */
_start:
    /* Set up the stack pointer */
    ldr x0, =stack_top
    mov sp, x0

    /* Load the base address of the string into x0 */
    ldr x0, =boot_msg

    /* Write the string to the UART (serial output) */
print_boot_msg:
    ldrb w2, [x0], #1          /* Load a byte from the string */
    cmp w2, #0                 /* Check if null byte */
    b.eq boot_kernel           /* If null byte, finish and jump to kernel */

    /* Print bootloader message */
    bl print_string            /* Branch Link UART address to x3*/
    strb w2, [x3]              /* Write the byte to UART */
    b print_boot_msg           /* Loop until string is printed */

boot_kernel:
    /* Print a message before jumping to kernel */
    ldr x0, =jump_to_kernel_msg
    bl print_string

    /* Jump to the kernel entry point */
    ldr x0, =kernel_entry      /* Address of the kernel entry */
    br x0                      /* Branch to the kernel */

    /* Infinite loop to halt */
end:
    b end                      /* Hang system */

/* Function to print a string to UART */
print_string:
    mov x3, #0x09000000        /* UART base address for QEMU virt machine */
print_loop:
    ldrb w2, [x0], #1          /* Load a byte from the string */
    cmp w2, #0                 /* Check if null byte */
    b.eq print_done            /* If null byte, finish */
    strb w2, [x3]              /* Write the byte to UART */
    b print_loop               /* Loop to next character */
print_done:
    ret

/* Data section */
.section .data
boot_msg:
    .ascii "[bootloader] Boot init completed\n"      /* The string to print */

jump_to_kernel_msg:
    .ascii "[bootloader] Kernel init starting...\n"  /* Message before jumping to kernel */

/* Stack */
.section .bss
.align 16
.stack:
    .skip 0x1000               /* 4KB stack */
stack_top:
