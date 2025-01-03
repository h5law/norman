/*
Copyright (c) 2024 Harry Law <h5law>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Harry Law
harry@h5law.com
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NORM_UTILS_H
#define NORM_UTILS_H

#include <sys/types.h>

extern inline int memvcmp(void *memory, unsigned char val, size_t size);

#endif /* ifndef NORM_UTILS_H */

#ifdef NORM_UTILS_IMPLEMENTATION

#include <string.h>

extern inline int memvcmp(void *memory, unsigned char val, size_t size)
{
    unsigned char *mm = (unsigned char *)memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

#endif /* ifdef NORM_UTILS_IMPLEMENTATION */

#ifdef __cplusplus
} /* extern "C" */
#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et
