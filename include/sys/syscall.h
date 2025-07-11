/*
Copyright (c) 2025 h5law <dev@h5law.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#ifndef NORM_SYSCALL_H
#define NORM_SYSCALL_H

#ifndef __syscall_declared
#define __syscall_declared
// defined in the assembler
extern int syscall(int num, ...);
#endif /* ifndef __syscall_declared */

// Use the same syscall number definitions for all architectures
// Expand syscall number definitions to cover all calls
// TODO: Enable overriding the syscall number definitions
#define SYS_SYSCALL 0x20000000
#define SYS_EXIT    0x20000001
#define SYS_FORK    0x20000002
#define SYS_READ    0x20000003
#define SYS_WRITE   0x20000004
#define SYS_OPEN    0x20000005
#define SYS_CLOSE   0x20000006
#define SYS_WAIT4   7
#define SYS_CHDIR   8
#define SYS_EXECVE  9
#define SYS_BRK     10
#define SYS_SBRK    11
#define SYS_MMAP    0x200000C5
// ...

#endif /* ifndef NORM_SYSCALL_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
