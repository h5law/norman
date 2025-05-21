    // ; .section .init,"ax",%progbits
    .align 4
    .global _init
    // ; .type _init,%function
_init:
    stp x29, x30, [sp, #-16]!  // Save frame pointer and link register
    mov x29, sp                // Set up frame pointer
    // cc will put the contents of crtbegin.o's .init section here.

    // ; .section .fini,"ax",%progbits
    .align 4
    .global _fini
    // ; .type _fini,%function
_fini:
    stp x29, x30, [sp, #-16]!  // Save frame pointer and link register
    mov x29, sp                // Set up frame pointer
    // cc will put the contents of crtbegin.o's .fini section here.
