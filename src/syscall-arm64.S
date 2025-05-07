; .global __start
; .type   __start, @function
.global __syscall
.globl  __cerror

.text

; __start:
;     mov 0x0(%rsp), %rdi
;     lea 0x8(%rsp), %rsi
;     call __main
;     mov $1, %rax
;     __syscall

__syscall:
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

    /* Check is x0 negative (x0 + 0 < 0) */
    cmn     x0, #0
    /* Branch to __cerror if negative flag set */
    ; b.mi    __cerror

    /* Return error code stored in x0 */
    ret

/*
 * Error handler - sets errno and returns -1
 */
; __cerror:
;     neg     x0, x0                  /* Make error code positive */
;     adrp    x1, errno               /* Load address of errno */
;     str     w0, [x1, :lo12:errno]   /* Store error code in errno */
;     mov     x0, #-1                 /* Return -1 */
;     ret
