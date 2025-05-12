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

#ifndef NORM_SYS_H
#define NORM_SYS_H

#ifndef NULL
#define NULL (( void * )0)
#endif /* ifndef NULL */

#ifndef EOF
#define EOF (-1)
#endif /* ifndef EOF */

#ifndef __size_t
typedef __SIZE_TYPE__ size_t;
// use (unsigned -> signed) remapping for signed variant definition
#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned // unmap (unsigned -> signed) reverting to normal
#endif          /* ifndef __size_t */

typedef int pid_t;
struct rusage;

// Wait process option flags
#define WCONTINUED 0x0000    // select processes after SIGCONT signal
#define WNOHANG    0x0001    // do not block
#define WUNTRACED  0x0002    // select stopped processes (TIN, TOU, STP, STOP)
#define WSTOPPED   WUNTRACED // an alias for WUNTRACED
#define WTRAPPED   0x0004 // select processes being traced (implicit for wait4)
#define WEXITED    0x0008 // select terminated processes (implicit for wait4)
#define WNOWAIT    0x0010 // keep selected processes in a waitable state

extern char **envp;
int           __main(int argc, char *argv[]);

// TODO: Add other architectures and call numbers
#define SYS_SYSCALL 0
#define SYS_EXIT    1
#define SYS_FORK    2
#define SYS_READ    3
#define SYS_WRITE   4
#define SYS_OPEN    5
#define SYS_CLOSE   6
#define SYS_WAIT4   7
#define SYS_CHDIR   8
#define SYS_EXECVE  9
#define SYS_BRK     10
#define SYS_SBRK    11
// ...

// defined in the assembler
extern int syscall(int num, ...);

// syscall wrappers
void  exit(int status);
int   fork(void);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
int   execve(const char *path, const char *argv[], const char *envp[]);
int   chdir(const char *dir);
int   brk(const void *addr);
void *sbrk(long incr);

#endif /* ifndef NORM_SYS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
