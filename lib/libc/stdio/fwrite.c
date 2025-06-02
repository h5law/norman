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
#include <sys/syscall.h>
#include <stdlib.h>

int _fwrite(const char *ptr, size_t size, FILE *f)
{
    int ret = 0;
    for (int i = 0; (i < size) && (ret == 0); ++i) {
        // TODO: Improve splits (\n, ...) with appropriate stream types
        f->buf[f->pos++] = ptr[i];
        if (f->pos == BUFSIZ) {
            ret    = fflush(f);
            f->pos = 0;
        }
    }
    return ret;
}

size_t fwrite(const void *buf, size_t size, size_t count, FILE *f)
{
    int         i   = 0;
    int         ok  = TRUE;
    const char *ptr = buf;
    for (i = 0; i < count && ok; i++) {
        if (_fwrite(ptr, size, f) != 0)
            ok = FALSE;
        ptr += size;
        // TODO: set error
    }
    return i;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
