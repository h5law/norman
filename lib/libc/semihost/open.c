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

#include <semihost/shdefs.h>
#include <sys/types.h>
#include <errno.h>

// extern struct timeval __semihost_creat_time __weak;
// extern int gettimeofday(struct timeval *restrict tv, void *restrict tz)
// __weak;

/*
 * NOTE: binary mode has been chosen below because otherwise
 *       files are treated on host side as text files which
 *       is most probably not intented.  This means that
 *       data transfer is transparent between target and host.
 */

int open(const char *pathname, int flags, int mode)
{
    int semiflags = 0;

    switch (flags & (O_RDONLY | O_WRONLY | O_RDWR)) {
    case O_RDONLY:
        semiflags = SH_OPEN_R_B; /* 'rb' */
        break;
    case O_WRONLY:
        if (flags & O_TRUNC)
            semiflags = SH_OPEN_W_B; /* 'wb' */
        else
            semiflags = SH_OPEN_A_B; /* 'ab' */
        break;
    default:
        if (flags & O_TRUNC)
            semiflags = SH_OPEN_W_PLUS_B; /* 'wb+' */
        else
            semiflags = SH_OPEN_A_PLUS_B; /* 'ab+' */
        break;
    }

    /* Make sure any stdout/stderr fds are allocated first */
    ( void )_map_stdio(0); // NOTE: Is this necessary?
    int ret;
    do {
        ret = sys_semihost_open(pathname, semiflags);
    } while (0 <= ret && ret <= 2);
    if (ret == -1)
        errno = sys_semihost_errno();
    // else if (&__semihost_creat_time && gettimeofday)
    //     gettimeofday(&__semihost_creat_time, NULL);

    return ret;
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
