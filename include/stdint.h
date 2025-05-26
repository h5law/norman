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

#ifndef NORM_STDINT_H
#define NORM_STDINT_H

#if __WORDSIZE == 64

#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef long int intptr_t;
#endif /* ifndef __intptr_t_defined */

#ifndef __uintptr_t_defined
#define __uintptr_t_defined
typedef unsigned long int uintptr_t;
#endif /* uifndef __intptr_t_defined */

#else

#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef int intptr_t;
#endif /* ifndef __intptr_t_defined */

#ifndef __uintptr_t_defined
#define __uintptr_t_defined
typedef unsigned int uintptr_t;
#endif /* uifndef __intptr_t_defined */

#endif /* if __WORDSIZE == 64 */

// TODO: Ensure correct sizes across 32 and 64 bit architectures

#ifndef __INT32_TYPE
#define __INT32_TYPE
typedef int int32_t;
#endif /* ifndef __INT32_TYPE */

#ifndef __UINT32_TYPE
#define __UINT32_TYPE
typedef unsigned int uint32_t;
#endif /* ifndef __UINT32_TYPE */

#ifndef __INT64_TYPE
#define __INT64_TYPE
typedef long int int64_t;
#endif /* ifndef __INT64_TYPE */

#ifndef __UINT64_TYPE
#define __UINT64_TYPE
typedef unsigned long int uint64_t;
#endif /* ifndef __UINT64_TYPE */

#endif /* ifndef NORM_STDINT_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
