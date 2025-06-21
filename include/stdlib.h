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

#ifndef NORM_STDLIB_H
#define NORM_STDLIB_H

#include <sys/types.h>

#ifndef NULL
#define NULL (( void * )0)
#endif /* ifndef NULL */

#ifndef EOF
#define EOF (-1)
#endif /* ifndef EOF */

#ifndef FALSE
#define FALSE 0
#endif /* ifndef FALSE */

#ifndef TRUE
#define TRUE 1
#endif /* ifndef TRUE */

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif /* ifndef EXIT_SUCCESS */

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif /* ifndef EXIT_FAILURE */

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

extern int    envc;
extern char **envp;

#define SWAP(x, y)                                                             \
    do {                                                                       \
        x = x ^ y;                                                             \
        y = x ^ y;                                                             \
        x = x ^ y;                                                             \
    } while (0)

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

#ifndef __environ_declared
#define __environ_declared
extern char **environ;
extern char  *__progname;
#endif /* ifndef __environ_declared */

// syscall wrappers
void exit(int status);
void _Exit(int status);
int  atexit(void (*func)(void));

div_t  div(int n, int base);
ldiv_t ldiv(long n, long base);
int    abs(int x);
long   labs(long x);

char *ltoa(long n, char *str, int base);
char *itoa(int n, char *str, int base);
void  reverse(char *str, size_t len);

int   fork(void);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
int   execve(const char *path, const char *argv[], const char *envp[]);
int   chdir(const char *dir);

#endif /* ifndef NORM_STDLIB_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
