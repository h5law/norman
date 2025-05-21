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

#include "errno.h"

int errno = 0;

int set_errno(int num)
{
    // ; sub sp, sp, #16
    // ;
    // ; str w0, [sp, #12]
    // ; ldr w8, [sp, #12]
    // ;
    // ; adrp x9, _errno@GOTPAGE
    // ; ldr x9, [x9, _errno@GOTPAGEOFF]
    // ;
    // ; str w8, [x9]
    // ; add sp, sp, #16
    // ;
    // ; ret

    errno = num;
    return -1;
}

// void perror(const char *str)
// {
//     int _errno = errno;
//     if (str && str[0]) {
//         fputs(str, stderr);
//         fputs(": ", stderr);
//     }
//     fputs(strerror(_errno), stderr);
//     fputs("\n", stderr);
//     fflush(stderr);
// }
//
// static char _errbuf[128];
//
// char *strerror(int errnum)
// {
//     if (errnum < sys_nerr) {
//         strcpy(_errbuf, sys_errlist[errnum]);
//     } else {
//         strcpy(_errbuf, "Error ");
//         itoa(errnum, _errbuf + strlen(_errbuf), 10);
//     }
//
//     return _errbuf;
// }

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
