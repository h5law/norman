
#include <sys/cdefs.h>

#include "../crt0.h"

/* Cortex-mM includes an NVIC and starts with SP initialized */

extern const void *__interrupt_vector[];

#define CPACR (( volatile uint32_t * )(0xE000ED88))

#ifdef __clang__
const void *__interrupt_reference = __interrupt_vector;
#endif

void __disable_sanitizer _start(void)
{
    /* Generate a reference to __interrupt_vector so we get one loaded */
    __asm__(".equ __my_interrupt_vector, __interrupt_vector");
    /* Access to the coprocessor has to be enabled in CPACR, if either FPU or
     * MVE is used. This is described in "Arm v8-M Architecture Reference
     * Manual". */
#if defined __ARM_FP || defined __ARM_FEATURE_MVE
    /* Enable FPU */
    *CPACR |= 0xf << 20;
    /*
     * Wait for the write enabling FPU to reach memory before
     * executing the instruction accessing the status register
     */
    __asm__("dsb");
    __asm__("isb");

    /* Clear FPU status register. 0x40000 will initialize FPSCR.LTPSIZE to
     * a valid value for 8.1-m low overhead loops. */
#if __ARM_ARCH >= 8 && __ARM_ARCH_PROFILE == 'M'
#define INIT_FPSCR 0x40000
#else
#define INIT_FPSCR 0x0
#endif
    __asm__("vmsr fpscr, %0" : : "r"(INIT_FPSCR));
#endif

#if defined(__ARM_FEATURE_PAC_DEFAULT) || defined(__ARM_FEATURE_BTI_DEFAULT)
    uint32_t control;
    __asm__("mrs %0, CONTROL" : "=r"(control));
#ifdef __ARM_FEATURE_PAC_DEFAULT
    control |= (3 << 6);
#endif
#ifdef __ARM_FEATURE_BTI_DEFAULT
    control |= (3 << 4);
#endif
    __asm__("msr CONTROL, %0" : : "r"(control));
#endif
    __start();
}

#ifdef CRT0_SEMIHOST

/*
 * Trap faults, print message and exit when running under semihost
 */

#include <semihost/calls.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define _REASON(r) #r
#define REASON(r)  _REASON(r)

static void arm_fault_write_reg(const char *prefix, unsigned reg)
{
    fputs(prefix, stdout);

    for (unsigned i = 0; i < 8; i++) {
        char    *digitstr;
        unsigned digitval = 0xF & (reg >> (28 - 4 * i));
        itoa(digitval, digitstr, 10);
        print(digitstr);
    }

    print("");
}

#define GET_SP                                                                 \
    struct fault *sp;                                                          \
    __asm__("mov %0, sp" : "=r"(sp))

struct fault {
    unsigned int r0;
    unsigned int r1;
    unsigned int r2;
    unsigned int r3;
    unsigned int r12;
    unsigned int lr;
    unsigned int pc;
    unsigned int xpsr;
};

static const char *const reasons[] = {"hardfault\n", "memmanage\n",
                                      "busfault\n", "usagefault\n"};

#define REASON_HARDFAULT 0
#define REASON_MEMMANAGE 1
#define REASON_BUSFAULT  2
#define REASON_USAGE     3

static void __used arm_fault(struct fault *f, int reason)
{
    fputs("ARM fault: ", stdout);
    fputs(reasons[reason], stdout);
    arm_fault_write_reg("\tR0:   0x", f->r0);
    arm_fault_write_reg("\tR1:   0x", f->r1);
    arm_fault_write_reg("\tR2:   0x", f->r2);
    arm_fault_write_reg("\tR3:   0x", f->r3);
    arm_fault_write_reg("\tR12:  0x", f->r12);
    arm_fault_write_reg("\tLR:   0x", f->lr);
    arm_fault_write_reg("\tPC:   0x", f->pc);
    arm_fault_write_reg("\tXPSR: 0x", f->xpsr);
    _exit(1);
}

void __naked __disable_sanitizer arm_hardfault_isr(void)
{
    __asm__("mov r0, sp");
    __asm__("movs r1, #" REASON(REASON_HARDFAULT));
    __asm__("bl  arm_fault");
}

void __naked __disable_sanitizer arm_memmange_isr(void)
{
    __asm__("mov r0, sp");
    __asm__("movs r1, #" REASON(REASON_MEMMANAGE));
    __asm__("bl  arm_fault");
}

void __naked __disable_sanitizer arm_busfault_isr(void)
{
    __asm__("mov r0, sp");
    __asm__("movs r1, #" REASON(REASON_BUSFAULT));
    __asm__("bl  arm_fault");
}

void __naked __disable_sanitizer arm_usagefault_isr(void)
{
    __asm__("mov r0, sp");
    __asm__("movs r1, #" REASON(REASON_USAGE));
    __asm__("bl  arm_fault");
}

#endif /* CRT0_SEMIHOST */
