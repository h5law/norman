    .global _start
    .text
    .align 8
    .section .text,"ax"
    ; .type __start, @function
_start:
    ldr x0, [sp]
    add x1, sp, #8

    str lr, [sp, #-16]!
    bl __start
    ldr lr, [sp], #16

    mov x8, #1
    svc #0
