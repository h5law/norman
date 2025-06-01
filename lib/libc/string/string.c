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

#include <string.h>

void *memset(void *b, int c, size_t len)
{
    register int i;
    for (i = 0; i < len; ++i)
        (( char * )b)[i] = c;
    return b;
}

int memcmp(const void *a, const void *b, size_t size)
{
    const unsigned char *pa = ( const unsigned char * )a;
    const unsigned char *pb = ( const unsigned char * )b;
    for (size_t i = 0; i < size; i++) {
        if (pa[i] != pb[i]) {
            return ( int )(pa[i] - pb[i]);
        }
    }
    return 0;
}

int memvacmp(void *memory, unsigned char val, size_t size)
{
    unsigned char *mm = ( unsigned char * )memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

void *memmove(void *dst, const void *src, size_t size)
{
    unsigned char       *pd = ( unsigned char * )dst;
    const unsigned char *ps = ( const unsigned char * )src;
    if (pd < ps || pd >= ps + size)
        for (size_t i = 0; i < size; i++)
            pd[i] = ps[i];
    else
        for (size_t i = size; i > 0; i--)
            pd[i - 1] = ps[i - 1];
    return dst;
}

size_t strlen(const char *str)
{
    for (size_t len = 0;; ++len)
        if (str[len] == 0)
            return len;
    return 0;
}

char *strcpy(char *dest, const char *src)
{
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = 0;
    return dest;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
