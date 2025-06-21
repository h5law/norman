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

#ifndef ROLLO_HW_VIRT_UART_H
#define ROLLO_HW_VIRT_UART_H

#include <sys/cdefs.h>
#include <stdint.h>

#include "hwint.h"

#define UART0_BASE            _u(0x09000000)
#define UART1_BASE            _u(0x09040000)

#define UART_UARTDR_OFFSET    _u(0x00000000)
#define UART_UARTRSR_OFFSET   _u(0x00000004)
#define UART_UARTFR_OFFSET    _u(0x00000018)
#define UART_UARTILPR_OFFSET  _u(0x00000020)
#define UART_UARTIBRD_OFFSET  _u(0x00000024)
#define UART_UARTFBRD_OFFSET  _u(0x00000028)
#define UART_UARTLCR_H_OFFSET _u(0x0000002c)
#define UART_UARTCR_OFFSET    _u(0x00000030)
#define UART_UARTIFLS_OFFSET  _u(0x00000034)
#define UART_UARTIMSC_OFFSET  _u(0x00000038)
#define UART_UARTRIS_OFFSET   _u(0x0000003c)
#define UART_UARTMIS_OFFSET   _u(0x00000040)
#define UART_UARTICR_OFFSET   _u(0x00000044)
#define UART_UARTDMACR_OFFSET _u(0x00000048)

/* clang-format: off */
/* UART Hardware Structure for Register Layout and Controls
 * BITMASK  [BITRANGE]  FIELDNAME  (RESET VALUE)  DESCRIPTION
 */
typedef struct {
    /* 0x00000800 [11]    OE        (-) Overrun error
     * 0x00000400 [10]    BE        (-) Break error
     * 0x00000200 [9]     PE        (-) Parity error
     * 0x00000100 [8]     FE        (-) Framing error
     * 0x000000ff [7:0]   DATA      (-) Receive (read) data character
     */
    io_rw_32 dr; /* Data Register */

    /* 0x00000008 [3]     OE        (0) Overrun error
     * 0x00000004 [2]     BE        (0) Break error
     * 0x00000002 [1]     PE        (0) Parity error
     * 0x00000001 [0]     FE        (0) Framing error
     */
    io_rw_32 rsr; /* Receive Status/Error Clear Register */

    uint32_t _pad0[4];

    /* 0x00000100 [8]     RI        (-) Ring indicator
     * 0x00000080 [7]     TXFE      (1) Transmit FIFO empty
     * 0x00000040 [6]     RXFF      (0) Receive FIFO full
     * 0x00000020 [5]     TXFF      (0) Transmit FIFO full
     * 0x00000010 [4]     RXFE      (1) Receive FIFO empty
     * 0x00000008 [3]     BUSY      (0) UART busy
     * 0x00000004 [2]     DCD       (-) Data carrier detect
     * 0x00000002 [1]     DSR       (-) Data set ready
     * 0x00000001 [0]     CTS       (-) Clear to send
     */
    io_ro_32 fr; /* Flag Register */

    // 0x000000ff [7:0]   ILPDVSR   (0x00) 8-bit low-power divisor value
    io_rw_32 ilpr; /* IrDA Low-Power Counter Register */

    // 0x0000ffff [15:0]  BAUD_INT  (0x0000) The integer baud rate divisor
    io_rw_32 ibrd; /* Integer Baud Rate Register */

    // 0x0000003f [5:0]   BAUD_FRAC (0x00) The fractional baud rate divisor
    io_rw_32 fbrd; /* Fractional Baud Rate Register */

    /* 0x00000080 [7]     SPS       (0) Stick parity select
     * 0x00000060 [6:5]   WLEN      (0x0) Word length
     * 0x00000010 [4]     FEN       (0) Enable FIFOs:
     *                                  0 = FIFOs are disabled (character
     *                                  mode)...
     * 0x00000008 [3]     STP2      (0) Two stop bits select
     * 0x00000004 [2]     EPS       (0) Even parity select
     * 0x00000002 [1]     PEN       (0) Parity enable:
     *                                  0 = parity is disabled and no
     *                                  parity bit...
     * 0x00000001 [0]     BRK       (0) Send break
     */
    io_rw_32 lcr_h; /* Line Control Register */

    /* 0x00008000 [15]    CTSEN     (0) CTS hardware flow control enable
     * 0x00004000 [14]    RTSEN     (0) RTS hardware flow control enable
     * 0x00002000 [13]    OUT2      (0) This bit is the complement of the
     *                                  UART Out2 (nUARTOut2)...
     * 0x00001000 [12]    OUT1      (0) This bit is the complement of the
     *                                  UART Out1 (nUARTOut1)...
     * 0x00000800 [11]    RTS       (0) Request to send
     * 0x00000400 [10]    DTR       (0) Data transmit ready
     * 0x00000200 [9]     RXE       (1) Receive enable
     * 0x00000100 [8]     TXE       (1) Transmit enable
     * 0x00000080 [7]     LBE       (0) Loopback enable
     * 0x00000004 [2]     SIRLP     (0) SIR low-power IrDA mode
     * 0x00000002 [1]     SIREN     (0) SIR enable:
     *                                  0 = IrDA SIR ENDEC is disabled
     * 0x00000001 [0]     UARTEN    (0) UART enable: 0 = UART is disabled
     */
    io_rw_32 cr; /* Control Register */

    /* 0x00000038 [5:3]   RXIFLSEL  (0x2) Receive interrupt FIFO level select
     * 0x00000007 [2:0]   TXIFLSEL  (0x2) Transmit interrupt FIFO level select
     */
    io_rw_32 ifls; /* Interrupt FIFO Level Select Register */

    /* 0x00000400 [10]    OEIM      (0) Overrun error interrupt mask
     * 0x00000200 [9]     BEIM      (0) Break error interrupt mask
     * 0x00000100 [8]     PEIM      (0) Parity error interrupt mask
     * 0x00000080 [7]     FEIM      (0) Framing error interrupt mask
     * 0x00000040 [6]     RTIM      (0) Receive timeout interrupt mask
     * 0x00000020 [5]     TXIM      (0) Transmit interrupt mask
     * 0x00000010 [4]     RXIM      (0) Receive interrupt mask
     * 0x00000008 [3]     DSRMIM    (0) nUARTDSR modem interrupt mask
     * 0x00000004 [2]     DCDMIM    (0) nUARTDCD modem interrupt mask
     * 0x00000002 [1]     CTSMIM    (0) nUARTCTS modem interrupt mask
     * 0x00000001 [0]     RIMIM     (0) nUARTRI modem interrupt mask
     */
    io_rw_32 imsc; /* Interrupt Mask Set/Clear Register */

    /* 0x00000400 [10]    OERIS     (0) Overrun error interrupt status
     * 0x00000200 [9]     BERIS     (0) Break error interrupt status
     * 0x00000100 [8]     PERIS     (0) Parity error interrupt status
     * 0x00000080 [7]     FERIS     (0) Framing error interrupt status
     * 0x00000040 [6]     RTRIS     (0) Receive timeout interrupt status
     * 0x00000020 [5]     TXRIS     (0) Transmit interrupt status
     * 0x00000010 [4]     RXRIS     (0) Receive interrupt status
     * 0x00000008 [3]     DSRRMIS   (-) nUARTDSR modem interrupt status
     * 0x00000004 [2]     DCDRMIS   (-) nUARTDCD modem interrupt status
     * 0x00000002 [1]     CTSRMIS   (-) nUARTCTS modem interrupt status
     * 0x00000001 [0]     RIRMIS    (-) nUARTRI modem interrupt status
     */
    io_ro_32 ris; /* Raw Interrupt Status Register */

    /* 0x00000400 [10]    OEMIS     (0) Overrun error masked interrupt status
     * 0x00000200 [9]     BEMIS     (0) Break error masked interrupt status
     * 0x00000100 [8]     PEMIS     (0) Parity error masked interrupt status
     * 0x00000080 [7]     FEMIS     (0) Framing error masked interrupt status
     * 0x00000040 [6]     RTMIS     (0) Receive timeout masked interrupt status
     * 0x00000020 [5]     TXMIS     (0) Transmit masked interrupt status
     * 0x00000010 [4]     RXMIS     (0) Receive masked interrupt status
     * 0x00000008 [3]     DSRMMIS   (-) nUARTDSR modem masked interrupt status
     * 0x00000004 [2]     DCDMMIS   (-) nUARTDCD modem masked interrupt status
     * 0x00000002 [1]     CTSMMIS   (-) nUARTCTS modem masked interrupt status
     * 0x00000001 [0]     RIMMIS    (-) nUARTRI modem masked interrupt status
     */
    io_ro_32 mis; /* Masked Interrupt Status Register */

    /* 0x00000400 [10]    OEIC      (-) Overrun error interrupt clear
     * 0x00000200 [9]     BEIC      (-) Break error interrupt clear
     * 0x00000100 [8]     PEIC      (-) Parity error interrupt clear
     * 0x00000080 [7]     FEIC      (-) Framing error interrupt clear
     * 0x00000040 [6]     RTIC      (-) Receive timeout interrupt clear
     * 0x00000020 [5]     TXIC      (-) Transmit interrupt clear
     * 0x00000010 [4]     RXIC      (-) Receive interrupt clear
     * 0x00000008 [3]     DSRMIC    (-) nUARTDSR modem interrupt clear
     * 0x00000004 [2]     DCDMIC    (-) nUARTDCD modem interrupt clear
     * 0x00000002 [1]     CTSMIC    (-) nUARTCTS modem interrupt clear
     * 0x00000001 [0]     RIMIC     (-) nUARTRI modem interrupt clear
     */
    io_rw_32 icr; /* Interrupt Clear Register */

    /* 0x00000004 [2]     DMAONERR  (0) DMA on error
     * 0x00000002 [1]     TXDMAE    (0) Transmit DMA enable
     * 0x00000001 [0]     RXDMAE    (0) Receive DMA enable
     */
    io_rw_32 dmacr; /* DMA Control Register */
} uart_hw_t;
/* clang-format: on */

/* UART Peripheral definitions using full descriptive structs */
#define uart0_hw (( uart_hw_t * )UART0_BASE)
#define uart1_hw (( uart_hw_t * )UART1_BASE)

#endif /* #ifndef ROLLO_HW_VIRT_UART_H */
