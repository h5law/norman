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

#pragma once

#ifndef NORM_SHCALLS_H
#define NORM_SHCALLS_H

#include <sys/cdefs.h>
#include <stdio.h>
#include <stdint.h>

/* Semihost exceptions for semihost_exit */
#define ADP_Stopped_BranchThroughZero   0x20000
#define ADP_Stopped_UndefinedInstr      0x20001
#define ADP_Stopped_SoftwareInterrupt   0x20002
#define ADP_Stopped_PrefetchAbort       0x20003
#define ADP_Stopped_DataAbort           0x20004
#define ADP_Stopped_AddressException    0x20005
#define ADP_Stopped_IRQ                 0x20006
#define ADP_Stopped_FIQ                 0x20007
#define ADP_Stopped_BreakPoint          0x20020
#define ADP_Stopped_WatchPoint          0x20021
#define ADP_Stopped_StepComplete        0x20022
#define ADP_Stopped_RunTimeErrorUnknown 0x20023
#define ADP_Stopped_InternalError       0x20024
#define ADP_Stopped_UserInterruption    0x20025
#define ADP_Stopped_ApplicationExit     0x20026
#define ADP_Stopped_StackOverflow       0x20027
#define ADP_Stopped_DivisionByZero      0x20028
#define ADP_Stopped_OSSpecific          0x20029

/* Semihost extensions */
#define SH_EXT_EXIT_EXTENDED            0
#define SH_EXT_STDOUT_STDERR            1
#define SH_EXT_NUM                      2

/* Open modes */
#define SH_OPEN_R                       0
#define SH_OPEN_R_B                     1
#define SH_OPEN_R_PLUS                  2
#define SH_OPEN_R_PLUS_B                3
#define SH_OPEN_W                       4
#define SH_OPEN_W_B                     5
#define SH_OPEN_W_PLUS                  6
#define SH_OPEN_W_PLUS_B                7
#define SH_OPEN_A                       8
#define SH_OPEN_A_B                     9
#define SH_OPEN_A_PLUS                  10
#define SH_OPEN_A_PLUS_B                11

struct semihost_mem_t {
    void *heap_base;
    void *heap_limit;
    void *stack_base;
    void *stack_limit;
};

uintptr_t semihost_clock(void);
int       semihost_close(int fd);
uint64_t  semihost_elapsed(void);
int       semihost_errno(void);
int       semihost_feature(uint8_t feature);
uintptr_t semihost_flen(int fd);
int       semihost_get_cmdline(char *buf, int size);
int       semihost_getc(FILE *file);
void      semihost_heapinfo(struct semihost_mem_t *block);
int       semihost_iserror(intptr_t status);
int       semihost_istty(int fd);
int       semihost_open(const char *pathname, int semiflags);
int       semihost_putc(char c, FILE *file);
uintptr_t semihost_read(int fd, void *buf, size_t count);
int       semihost_remove(const char *pathname);
int       semihost_rename(const char *old_pathname, const char *new_pathname);
int       semihost_seek(int fd, uintptr_t pos);
int       semihost_system(const char *command);
uintptr_t semihost_tickfreq(void);
uintptr_t semihost_time(void);
int       semihost_tmpnam(char *pathname, int identifier, int maxpath);
uintptr_t semihost_write(int fd, const void *buf, size_t count);
void      semihost_write0(const char *string);
__noreturn void semihost_exit(uintptr_t exception, uintptr_t subcode);
__noreturn void semihost_exit_extended(uintptr_t code);

#endif /* ifndef NORM_SHCALLS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
