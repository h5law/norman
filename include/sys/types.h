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

#ifndef NORM_SYS_TYPES_H
#define NORM_SYS_TYPES_H

#ifndef __size_t
#define __size_t

#ifndef __SIZE_T_DECLARED
#define __SIZE_T_DECLARED
typedef __SIZE_TYPE__ size_t;
#endif /* ifndef __SIZE_T_DECLARED */

#ifndef __SSIZE_T_DECLARED
#define __SSIZE_T_DECLARED
// use (unsigned -> signed) remapping for signed variant definition
#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned // unmap (unsigned -> signed) reverting to normal
#endif          /* ifndef __SSIZE_T_DECLARED */

#endif /* ifndef __size_t */

#ifdef __LP64__
#define __WORDSIZE 64
#else
#define __WORDSIZE                32
#define __WORDSIZE32_SIZE_ULONG   0
#define __WORDSIZE32_PTRDIFF_LONG 0
#endif

#if __WORDSIZE == 64
#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef long int intptr_t;
#endif /* ifndef __intptr_t_defined */
typedef unsigned long int uintptr_t;
#else
#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef int intptr_t;
#endif /* ifndef __intptr_t_defined */
typedef unsigned int uintptr_t;
#endif /* if __WORDSIZE == 64 */

#endif /* ifndef NORM_SYS_TYPES_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
