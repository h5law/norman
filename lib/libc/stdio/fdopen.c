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
#include <string.h>
#include <sys/mem.h>

FILE *fdopen(int fd, const char *mode)
{
    FILE *ret = NULL;
    ret       = ( FILE * )malloc(sizeof(FILE));
    if (mode[0] == 'a' || mode[0] == 'w')
        ret->io_dir = _IODIR_OUT;
    else
        ret->io_dir = _IODIR_IN;
    memset(ret->buf, 0, BUFSIZ);
    ret->pos   = 0;
    ret->fd    = fd;
    ret->eof   = FALSE;
    ret->error = FALSE;
    return ret;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
