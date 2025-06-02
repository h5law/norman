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
#include <semihost/calls.h>
#include <sys/types.h>
#include <stdio.h>

ssize_t read(int fd, char *buf, size_t count)
{
    if (fd == 0) {
        int ch         = semihost_getc(( FILE * )0);
        *( char * )buf = ch;
        return 1;
    }
    fd            = semihost_map_stdio(fd);
    uintptr_t ret = semihost_read(fd, buf, count);

    ssize_t got   = count - ( ssize_t )ret;
    return got;
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
