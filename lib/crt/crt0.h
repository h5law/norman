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

#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <semihost/calls.h>

void _start(void);       /* csu entry point */
int  main(int, char **); /* application entry point */

extern FILE *stdin, *stdout, *stderr;

static __noreturn __always_inline void __start(void)
{
#ifndef PROGNAME
#define PROGNAME "norman-libc-program"
#endif /* ifndef PROGNAME */

#if defined(CRT0_SEMIHOST) && defined(__ARM_SEMIHOST)
#define CMDLINE_LEN 1024
#define ARGV_LEN    64
    static char  cmdline[CMDLINE_LEN];
    static char *argv[ARGV_LEN];
    int          argc = 0;

    if (semihost_get_cmdline(cmdline, sizeof(cmdline)) == 0 && cmdline[0]) {
        char *c = cmdline;
        while (*c && argc < ARGV_LEN - 1) {
            argv[argc++] = c;
            while (*c && *c != ' ')
                c++;
            if (!*c)
                break;
            *c = '\0';
            while (*++c == ' ')
                ;
        }
    }
#else
    static int    argc = 0;
    static char **argv = {};
#endif

    argv[argc]   = ( char * )NULL;
    argv[argc++] = ( char * )PROGNAME;
    int ret      = main(argc, argv);
#ifdef CRT0_EXIT
    exit(ret);
#else
    ( void )ret;
    for (;;)
        ;
#endif
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
