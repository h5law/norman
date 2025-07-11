/** rp2350/addresses.h
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ADDRESSMAP_H
#define _ADDRESSMAP_H

#include <sys/cdefs.h>

// Register address offsets for atomic RMW aliases
#define REG_ALIAS_RW_BITS                    (_u(0x0) << _u(12))
#define REG_ALIAS_XOR_BITS                   (_u(0x1) << _u(12))
#define REG_ALIAS_SET_BITS                   (_u(0x2) << _u(12))
#define REG_ALIAS_CLR_BITS                   (_u(0x3) << _u(12))

#define ROM_BASE                             _u(0x00000000)
#define XIP_BASE                             _u(0x10000000)
#define XIP_SRAM_BASE                        _u(0x13ffc000)
#define XIP_END                              _u(0x14000000)
#define XIP_NOCACHE_NOALLOC_BASE             _u(0x14000000)
#define XIP_SRAM_END                         _u(0x14000000)
#define XIP_NOCACHE_NOALLOC_END              _u(0x18000000)
#define XIP_MAINTENANCE_BASE                 _u(0x18000000)
#define XIP_NOCACHE_NOALLOC_NOTRANSLATE_BASE _u(0x1c000000)
#define SRAM0_BASE                           _u(0x20000000)
#define XIP_NOCACHE_NOALLOC_NOTRANSLATE_END  _u(0x20000000)
#define SRAM_BASE                            _u(0x20000000)
#define SRAM_STRIPED_BASE                    _u(0x20000000)
#define SRAM4_BASE                           _u(0x20040000)
#define SRAM8_BASE                           _u(0x20080000)
#define SRAM_STRIPED_END                     _u(0x20080000)
#define SRAM_SCRATCH_X_BASE                  _u(0x20080000)
#define SRAM9_BASE                           _u(0x20081000)
#define SRAM_SCRATCH_Y_BASE                  _u(0x20081000)
#define SRAM_END                             _u(0x20082000)
#define SYSINFO_BASE                         _u(0x40000000)
#define SYSCFG_BASE                          _u(0x40008000)
#define CLOCKS_BASE                          _u(0x40010000)
#define PSM_BASE                             _u(0x40018000)
#define RESETS_BASE                          _u(0x40020000)
#define IO_BANK0_BASE                        _u(0x40028000)
#define IO_QSPI_BASE                         _u(0x40030000)
#define PADS_BANK0_BASE                      _u(0x40038000)
#define PADS_QSPI_BASE                       _u(0x40040000)
#define XOSC_BASE                            _u(0x40048000)
#define PLL_SYS_BASE                         _u(0x40050000)
#define PLL_USB_BASE                         _u(0x40058000)
#define ACCESSCTRL_BASE                      _u(0x40060000)
#define BUSCTRL_BASE                         _u(0x40068000)
#define UART0_BASE                           _u(0x40070000)
#define UART1_BASE                           _u(0x40078000)
#define SPI0_BASE                            _u(0x40080000)
#define SPI1_BASE                            _u(0x40088000)
#define I2C0_BASE                            _u(0x40090000)
#define I2C1_BASE                            _u(0x40098000)
#define ADC_BASE                             _u(0x400a0000)
#define PWM_BASE                             _u(0x400a8000)
#define TIMER0_BASE                          _u(0x400b0000)
#define TIMER1_BASE                          _u(0x400b8000)
#define HSTX_CTRL_BASE                       _u(0x400c0000)
#define XIP_CTRL_BASE                        _u(0x400c8000)
#define XIP_QMI_BASE                         _u(0x400d0000)
#define WATCHDOG_BASE                        _u(0x400d8000)
#define BOOTRAM_BASE                         _u(0x400e0000)
#define BOOTRAM_END                          _u(0x400e0400)
#define ROSC_BASE                            _u(0x400e8000)
#define TRNG_BASE                            _u(0x400f0000)
#define SHA256_BASE                          _u(0x400f8000)
#define POWMAN_BASE                          _u(0x40100000)
#define TICKS_BASE                           _u(0x40108000)
#define OTP_BASE                             _u(0x40120000)
#define OTP_DATA_BASE                        _u(0x40130000)
#define OTP_DATA_RAW_BASE                    _u(0x40134000)
#define OTP_DATA_GUARDED_BASE                _u(0x40138000)
#define OTP_DATA_RAW_GUARDED_BASE            _u(0x4013c000)
#define CORESIGHT_PERIPH_BASE                _u(0x40140000)
#define CORESIGHT_ROMTABLE_BASE              _u(0x40140000)
#define CORESIGHT_AHB_AP_CORE0_BASE          _u(0x40142000)
#define CORESIGHT_AHB_AP_CORE1_BASE          _u(0x40144000)
#define CORESIGHT_TIMESTAMP_GEN_BASE         _u(0x40146000)
#define CORESIGHT_ATB_FUNNEL_BASE            _u(0x40147000)
#define CORESIGHT_TPIU_BASE                  _u(0x40148000)
#define CORESIGHT_CTI_BASE                   _u(0x40149000)
#define CORESIGHT_APB_AP_RISCV_BASE          _u(0x4014a000)
#define DFT_BASE                             _u(0x40150000)
#define GLITCH_DETECTOR_BASE                 _u(0x40158000)
#define TBMAN_BASE                           _u(0x40160000)
#define DMA_BASE                             _u(0x50000000)
#define USBCTRL_BASE                         _u(0x50100000)
#define USBCTRL_DPRAM_BASE                   _u(0x50100000)
#define USBCTRL_REGS_BASE                    _u(0x50110000)
#define PIO0_BASE                            _u(0x50200000)
#define PIO1_BASE                            _u(0x50300000)
#define PIO2_BASE                            _u(0x50400000)
#define XIP_AUX_BASE                         _u(0x50500000)
#define HSTX_FIFO_BASE                       _u(0x50600000)
#define CORESIGHT_TRACE_BASE                 _u(0x50700000)
#define SIO_BASE                             _u(0xd0000000)
#define SIO_NONSEC_BASE                      _u(0xd0020000)
#define PPB_BASE                             _u(0xe0000000)
#define PPB_NONSEC_BASE                      _u(0xe0020000)
#define EPPB_BASE                            _u(0xe0080000)

#endif // _ADDRESSMAP_H
