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

#ifndef ROLLO_MACH_UART_H
#define ROLLO_MACH_UART_H

#include <virt/uart.h>

#include <stdint.h>

void uart_init(uart_hw_t *uart);
void uart_reset(uart_hw_t *uart);

void uart_puts(uart_hw_t *uart, const char *str);
void uart_putc(uart_hw_t *uart, char c);
char uart_getc(uart_hw_t *uart);
int  uart_getc_timeout(uart_hw_t *uart, char *c, uint32_t timeout_ticks);
void uart_gets(uart_hw_t *uart, char *buffer, int max_length,
               uint32_t timeout_ticks);
void uart_printf(uart_hw_t *uart, const char *fmt, ...);

#endif /* #ifndef ROLLO_MACH_UART_H */
