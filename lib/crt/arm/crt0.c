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

#include <sys/cdefs.h>

#include "RP2350.h"
#include "core_cm33.h"
#include "../crt0.h"

// defined in CMSIS as __StackTop
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void _start(void);

void        Default_Handler(void);
__NO_RETURN __section(".reset") void Reset_Handler(void);
void        NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void        HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SecureFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// defined in system_RP2350.h
typedef void (*VECTOR_TABLE_Type)(void);
// __VECTOR_TABLE is __Vectors (m-profile/cmsis_gcc.h)
extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];
// attribute is defined as (used, section(".vectors"))
const VECTOR_TABLE_Type        __VECTOR_TABLE[496] __VECTOR_TABLE_ATTRIBUTE = {
        ( VECTOR_TABLE_Type )(&__INITIAL_SP),
        Reset_Handler,
        NMI_Handler,
        HardFault_Handler,
        MemManage_Handler,
        BusFault_Handler,
        UsageFault_Handler,
        SecureFault_Handler,
        0,
        0,
        0,
        SVC_Handler,
        DebugMon_Handler,
        0,
        PendSV_Handler,
        SysTick_Handler};

void Default_Handler(void)
{
    while (1)
        ;
}

__NO_RETURN void Reset_Handler(void)
{
    // defined in CMSIS as __StackLimit
    extern uint32_t __STACK_LIMIT;

    __set_PSP(( uint32_t )(&__INITIAL_SP));
    __set_MSPLIM(( uint32_t )(&__STACK_LIMIT));
    __set_PSPLIM(( uint32_t )(&__STACK_LIMIT));

    // copy data section from FLASH to RAM
    extern uint32_t *__data_start__;
    extern uint32_t *__data_end__;
    extern uint32_t *__data_source__;

    uint32_t *p = __data_start__;
    uint32_t *q = __data_source__;

    while (p < __data_end__) {
        *p++ = *q++;
    }

    _start();
}

void HardFault_Handler(void)
{
    while (1)
        ;
}

#ifdef CRT0_SEMIHOST
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

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
