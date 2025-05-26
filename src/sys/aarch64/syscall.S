    .global syscall

    .section __DATA,.syscall
    .align 8
    ; .type syscall, @function
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
