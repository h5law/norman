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

#ifndef NORM_SHDEFS_H
#define NORM_SHDEFS_H

#include <sys/cdefs.h>
#include <stdint.h>

#include "calls.h"

/* clang-format off */
// ARM Semihosting SVC numbers
#define angel_SWIreason_EnterSVC        0x17 /* Sets the processor to Supervisor mode and disable all interrupts */
#define angel_SWIreason_ReportException 0x18 /* Directly report an exception to the (inc. execution completion) */
#define SYS_EXIT                        angel_SWIreason_ReportException
#define SYS_CLOSE                       0x02 /* Closes a file on the host system */
#define SYS_CLOCK                       0x10 /* Returns the number of centiseconds since the execution started */
#define SYS_ELAPSED                     0x30 /* Returns the number of elapsed target ticks since execution started */
#define SYS_ERRNO                       0x13 /* Returns the value of the C semihosting environment errno value */
#define SYS_FLEN                        0x0C /* Returns the length of a specified file */
#define SYS_GET_CMDLINE                 0x15 /* Returns the command line used for the call to the executable, that is, argc and argv */
#define SYS_HEAPINFO                    0x16 /* Returns the system stack and heap parameters */
#define SYS_ISERROR                     0x08 /* Determines whether the return code from another semihosting call is an error status or not */
#define SYS_ISTTY                       0x09 /* Checks whether a file is connected to an interactive device */
#define SYS_OPEN                        0x01 /* Opens a file on the host system */
#define SYS_READ                        0x06 /* Reads the contents of a file into a buffer */
#define SYS_READC                       0x07 /* Reads a byte from the console */
#define SYS_REMOVE                      0x0E /* Deletes a specified file on the host filing system */
#define SYS_RENAME                      0x0F /* Renames a specified file */
#define SYS_SEEK                        0x0A /* Seeks to a specified position in a file using an offset specified from the start of the file */
#define SYS_SYSTEM                      0x12 /* Passes a command to the host command-line interpreter */
#define SYS_TICKFREQ                    0x31 /* Returns the tick frequency */
#define SYS_TIME                        0x11 /* Returns the number of seconds since 00:00 January 1, 1970 */
#define SYS_TMPNAM                      0x0D /* Returns a temporary name for a file identified by a system file identifier */
#define SYS_WRITE                       0x05 /* Writes the contents of a buffer to a specified file at the current file position */
#define SYS_WRITEC                      0x03 /* Writes a character byte, pointed to by R1, to the debug channel */
#define SYS_WRITE0                      0x04 /* Writes a null-terminated string to the debug channel from R1 pointer */

/* Feature bits are reported using a sequence of bytes, which are accessed by
 * using the SYS_OPEN call with the special path name :semihosting-features */
#define SHFB_MAGIC_0                    0x53
#define SHFB_MAGIC_1                    0x48
#define SHFB_MAGIC_2                    0x46
#define SHFB_MAGIC_3                    0x42

typedef uintptr_t sh_param_t;

extern uintptr_t sys_semihost(uintptr_t op, uintptr_t param);

/* Helper functions to simplify semihosting calls with indirect arguments. */
static inline uintptr_t
sys_semihost1(long op, sh_param_t arg1)
{
    struct {
        sh_param_t field1;
    } indirect_args = {
        .field1 = arg1,
    };
    return sys_semihost(op, (uintptr_t)&indirect_args);
}

static inline uintptr_t
sys_semihost2(long op, sh_param_t arg1, sh_param_t arg2)
{
    struct {
        sh_param_t field1;
        sh_param_t field2;
    } indirect_args = {
        .field1 = arg1,
        .field2 = arg2,
    };
    return sys_semihost(op, (uintptr_t)&indirect_args);
}

static inline uintptr_t
sys_semihost3(long op, sh_param_t arg1, sh_param_t arg2, sh_param_t arg3)
{
    struct {
        sh_param_t field1;
        sh_param_t field2;
        sh_param_t field3;
    } indirect_args = {
        .field1 = arg1,
        .field2 = arg2,
        .field3 = arg3,
    };
    return sys_semihost(op, (uintptr_t)&indirect_args);
}

int
_map_stdio(int fd);

#endif /* ifndef NORM_SHDEFS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
