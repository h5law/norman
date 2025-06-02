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

/* clang-format off */
// ARM Semihosting SVC numbers
#define angel_SWIreason_EnterSVC        0x17 /* Sets the processor to Supervisor mode and disable all interrupts */
#define angel_SWIreason_ReportException 0x18 /* Directly report an exception to the (inc. execution completion) */
#define SH_EXIT                        angel_SWIreason_ReportException
#define SH_EXIT_EXTENDED               0x20
#define SH_CLOSE                       0x02 /* Closes a file on the host system */
#define SH_CLOCK                       0x10 /* Returns the number of centiseconds since the execution started */
#define SH_ELAPSED                     0x30 /* Returns the number of elapsed target ticks since execution started */
#define SH_ERRNO                       0x13 /* Returns the value of the C semihosting environment errno value */
#define SH_FLEN                        0x0C /* Returns the length of a specified file */
#define SH_GET_CMDLINE                 0x15 /* Returns the command line used for the call to the executable, that is, argc and argv */
#define SH_HEAPINFO                    0x16 /* Returns the system stack and heap parameters */
#define SH_ISERROR                     0x08 /* Determines whether the return code from another semihosting call is an error status or not */
#define SH_ISTTY                       0x09 /* Checks whether a file is connected to an interactive device */
#define SH_OPEN                        0x01 /* Opens a file on the host system */
#define SH_READ                        0x06 /* Reads the contents of a file into a buffer */
#define SH_READC                       0x07 /* Reads a byte from the console */
#define SH_REMOVE                      0x0E /* Deletes a specified file on the host filing system */
#define SH_RENAME                      0x0F /* Renames a specified file */
#define SH_SEEK                        0x0A /* Seeks to a specified position in a file using an offset specified from the start of the file */
#define SH_SYSTEM                      0x12 /* Passes a command to the host command-line interpreter */
#define SH_TICKFREQ                    0x31 /* Returns the tick frequency */
#define SH_TIME                        0x11 /* Returns the number of seconds since 00:00 January 1, 1970 */
#define SH_TMPNAM                      0x0D /* Returns a temporary name for a file identified by a system file identifier */
#define SH_WRITE                       0x05 /* Writes the contents of a buffer to a specified file at the current file position */
#define SH_WRITEC                      0x03 /* Writes a character byte, pointed to by R1, to the debug channel */
#define SH_WRITE0                      0x04 /* Writes a null-terminated string to the debug channel from R1 pointer */

/* Feature bits are reported using a sequence of bytes, which are accessed by
 * using the SH_OPEN call with the special path name :semihosting-features */
#define SHFB_MAGIC_0                    0x53
#define SHFB_MAGIC_1                    0x48
#define SHFB_MAGIC_2                    0x46
#define SHFB_MAGIC_3                    0x42

typedef uintptr_t sh_param_t;

extern uintptr_t semihost(uintptr_t op, uintptr_t param);

/* Helper functions to simplify semihosting calls with indirect arguments. */
static inline uintptr_t
semihost1(long op, sh_param_t arg1)
{
    struct {
        sh_param_t field1;
    } indirect_args = {
        .field1 = arg1,
    };
    return semihost(op, (uintptr_t)&indirect_args);
}

static inline uintptr_t
semihost2(long op, sh_param_t arg1, sh_param_t arg2)
{
    struct {
        sh_param_t field1;
        sh_param_t field2;
    } indirect_args = {
        .field1 = arg1,
        .field2 = arg2,
    };
    return semihost(op, (uintptr_t)&indirect_args);
}

static inline uintptr_t
semihost3(long op, sh_param_t arg1, sh_param_t arg2, sh_param_t arg3)
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
    return semihost(op, (uintptr_t)&indirect_args);
}

int
semihost_map_stdio(int fd);

#endif /* ifndef NORM_SHDEFS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
