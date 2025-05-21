//  ; .section .start,"ax",%progbits
    .align 4
    .global _start
//  ; .type _start,%function
_start:
    // Set up end of the stack frame linked list
    mov x29, #0          // x29 is the frame pointer
    str x29, [sp, #-16]! // Push 0 to instruction pointer
    str x29, [sp, #-16]! // Push 0 as base frame pointer
    mov x29, sp          // Set frame pointer to current stack pointer

    str lr, [sp, #-16]!
    bl ___main
    ldr lr, [sp], #16

    mov x8, #1
    svc #0
// ; .size _start, . - _start

// ; 	.global _start
// ; _start:
// ;     ldr x0, [sp]
// ;     add x1, sp, #8
// ;
// ;     str lr, [sp, #-16]!
// ;     bl __main
// ;     ldr lr, [sp], #16
// ;
// ;     mov x8, #1
// ;     svc #0
