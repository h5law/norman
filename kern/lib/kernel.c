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

#include <machine/uart.h>
#include <machine/psci.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"

void uart_input(void);

void kernel_entry(void)
{
    static bool kernel_initialized = false;

    uart_init(uart0_hw);
    uart_puts(uart0_hw, "[kernel] UART initialized\n");

    kernel_initialized = true;

    uart_input();
    psci_system_off();

    while (1)
        ;
}

void uart_input(void)
{
    char buf[32] = {0};
    uart_gets(uart0_hw, buf, 32, 512 * 1024 * 1024);
    int len = strlen(buf);
    uart_puts(uart0_hw, "\n[kernel] Input string [");
    char digit[33] = {0};
    itoa(len, digit, 10);
    uart_puts(uart0_hw, digit);
    uart_puts(uart0_hw, " chars]: ");
    uart_puts(uart0_hw, buf);
    uart_putc(uart0_hw, '\n');
}
