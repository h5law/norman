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

#include "string.h"

void *memset(void *b, int c, size_t len)
{
    register int i;
    for (i = 0; i < len; ++i)
        (( char * )b)[i] = c;
    return b;
}

int memcmp(const void *a, const void *b, size_t size)
{
    register int i = 0;
    for (i = 0; i < size; ++i) {
        if ((( unsigned char * )a)[i] != (( unsigned char * )b)[i])
            return -1;
    }
    return 0;
}

int memvacmp(void *memory, unsigned char val, size_t size)
{
    unsigned char *mm = ( unsigned char * )memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
