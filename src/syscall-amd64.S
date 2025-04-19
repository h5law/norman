.global _start
.global syscall

.text

_start:
    mov 0x0(%rsp), %rdi
    lea 0x8(%rsp), %rsi
    call __main
    mov $1, %rax
    syscall

syscall:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
;   mov 0x8(%rsp), %r9
    syscall
    ret
