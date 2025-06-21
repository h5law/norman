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
#include <virt/uart.h>

#include <stdarg.h>
#include <stdlib.h>

void uart_init(uart_hw_t *uart)
{
    // Disable UART before configuration
    uart->cr    = 0x00;

    // TODO: Base on system clock used by the uart peripheral itself
    // Set baud rate (e.g., 115200 baud, assuming 24 MHz clock)
    uart->ibrd  = 0x0d; // Integer part: 24,000,000 / (16 * 115200) ≈ 13
    uart->fbrd  = 0x01; // Fractional part: (0.02 * 64 + 0.5) ≈ 1

    // Set line control to 8-bit words (0b11), no parity, 1 stop bit
    uart->lcr_h = 0x60;

    // Enable UARTEN, TXE, and RXE
    uart->cr    = 0x301;
}

void uart_putc(uart_hw_t *uart, char c)
{
    uint32_t timeout = 1000000;   /* Timeout threshold */
    while (uart->fr & (1 << 5)) { /* Wait if UART is busy */
        if (--timeout == 0) {
            char *timeout_msg = "[UART Timeout]\n";
            while (*timeout_msg) {
                uart->dr = ( char )*timeout_msg++;
            }
            return;
        }
    }
    uart->dr = c; /* Output character to UART */
}

void uart_puts(uart_hw_t *uart, const char *str)
{
    while (*str) {
        uart_putc(uart, *str++);
    }
}

char uart_getc(uart_hw_t *uart)
{
    char c;
    do {
        while (uart->fr & (1 << 4)) {
        } /* Wait for UART FIFO to be not empty */
        c = ( char )uart->dr;
    } while (c == 0);
    return c;
}

int uart_getc_timeout(uart_hw_t *uart, char *c, uint32_t timeout_ticks)
{
    uint32_t tick = 0;

    while (uart->fr & (1 << 4)) {
        if (++tick == timeout_ticks) {
            char *timeout_msg = "[UART Timeout]\n";
            while (*timeout_msg) {
                uart->dr = ( char )*timeout_msg++;
            }
            return -1; // TODO: Handle errno?
        }
    }
    *c = ( char )uart->dr;
    return 0;
}

void uart_gets(uart_hw_t *uart, char *buffer, int max_length,
               uint32_t timeout_ticks)
{
    int  i = 0;
    char c;
    while (i < max_length - 1) {
        if (uart_getc_timeout(uart, &c, timeout_ticks) == 0) {
            if (c == '\r' || c == '\n') {
                buffer[i] = '\0';
                uart_puts(uart, "\n");
                break;
            }
            if (c == '\b' && i > 0) {
                uart_puts(uart, "\b \b");
                i--;
            } else if (c >= 32 && c < 127) {
                buffer[i++] = c;
                uart->dr    = c;
            }
        } else {
            buffer[0] = '\0';
            uart_puts(uart, "[kernel] UART read timeout.\n");
            return;
        }
    }
    buffer[i] = '\0';
}

void uart_printf(uart_hw_t *uart, const char *fmt, ...)
{
    uart_puts(uart, "Entered\n");
    va_list args;
    va_start(args, fmt);
    uart_puts(uart, "Setup vargs\n");

    char buffer[256];
    char temp[32];

    const char *ptr;
    for (ptr = fmt; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            if (*ptr == 'd') {
                int num = va_arg(args, int);
                itoa(num, temp, 10);
                uart_puts(uart, temp);
            } else if (*ptr == 'x') {
                int num = va_arg(args, int);
                itoa(num, temp, 16);
                uart_puts(uart, temp);
            } else if (*ptr == 's') {
                char *str = va_arg(args, char *);
                uart_puts(uart, str);
            } else if (*ptr == '%') {
                uart_putc(uart, '%');
            }
        } else {
            uart_putc(uart, *ptr);
        }
    }

    va_end(args);
}
