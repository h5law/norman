    .text
    .align 4

//  ; _start:
//  ;     ldr x0, [sp]
//  ;     add x1, sp, #8
//  ;
//  ;     str lr, [sp, #-16]!
//  ;     bl __main
//  ;     ldr lr, [sp], #16
//  ;
//  ;     mov x8, #1
//  ;     svc #0

//  ; .section .start
    .global _start
//  ; .type _start,%function
_start:
    ldr x0, [sp]
    add x1, sp, #8

    str lr, [sp, #-16]!
    bl ___main
    ldr lr, [sp], #16

    mov x8, #1
    svc #0

//  ; .section .syscall
    .global syscall
//  ; .type syscall,%function
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
    svc     #0

    /* Check is x0 negative (x0 + 4096 < 0) */
    cmn     x0, #(4095 + 1)
    cneg    x0, x0, hi
    b.hi    __set_errno_internal

    ret

__set_errno_internal:
    mov x0, x0
    bl _set_errno
    ret
