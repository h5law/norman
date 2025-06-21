#include "../cmsis/stub/CMSIS/Core/Include/core_cm33.h"
#include <string.h>
#include <stdlib.h>

void printErrorMsg(const char *errMsg)
{
    while (memvacmp(errMsg, '\0', 1) != 0) {
        ITM_SendChar(*errMsg);
        ++errMsg;
    }
}

void Handle_HardFault(void)
{
    static char msg[80] = {0};
    printErrorMsg("In Hard Fault Handler\n");
    static char reg[9] = {0};
    itoa(SCB->HFSR, reg, 16);
    memmove(msg, "SCB->HFSR = 0x", 14);
    memmove(msg + 14, reg, 9);
    printErrorMsg(msg);
    __asm__ volatile("BKPT #01");
    while (1)
        ;
}
