.section .init
    // cc will put the contents of crtend.o's .init section here.
    ldp x29, x30, [sp], #16  // Restore frame pointer and link register
    ret

.section .fini
    // cc will put the contents of crtend.o's .fini section here.
    ldp x29, x30, [sp], #16  // Restore frame pointer and link register
    ret
