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

#ifndef NORM_STRING_H
#define NORM_STRING_H

#include <sys/types.h>

void *memset(void *b, int c, size_t len);
int   memcmp(const void *a, const void *b, size_t size);
void *memmove(void *dst, const void *src, size_t size);
int   memvacmp(void *memory, unsigned char val, size_t size);

size_t strlen(const char *str);
char  *strcpy(char *dest, const char *src);

#endif /* ifndef NORM_STRING_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
