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

#ifndef NORM_SYS_CDEFS_H
#define NORM_SYS_CDEFS_H

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_extension
#define __has_extension __has_feature
#endif
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#ifndef __has_include
#define __has_include(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_attribute(__noreturn__)
#define __noreturn __attribute__((__noreturn__))
#else
#define __noreturn
#endif

#if __has_attribute(__always_inline__)
#define __always_inline __inline __attribute__((__always_inline__))
#else
#define __always_inline
#endif

#if __has_attribute(__disable_sanitizer_instrumentation__)
#define __disable_sanitizer_instrumentation                                    \
    __attribute__((__disable_sanitizer_instrumentation__))
#else
#define __disable_sanitizer_instrumentation
#endif

#if __has_attribute(__no_sanitize_address__)
#define __no_sanitize_address __attribute__((__no_sanitize_address__))
#else
#define __no_sanitize_address
#endif

#if __has_attribute(__no_sanitize__)
#define __no_sanitize(s) __attribute__((__no_sanitize__(s)))
#else
#define __no_sanitize(s)
#endif

#if __has_attribute(__no_sanitize_undefined__)
#define __no_sanitize_undefined __attribute__((__no_sanitize_undefined__))
#else
#define __no_sanitize_undefined
#endif

#define __disable_sanitizer                                                    \
    __no_sanitize_address __no_sanitize_undefined                              \
            __disable_sanitizer_instrumentation __no_sanitize("undefined")

#if __has_attribute(__naked__)
#define __naked __attribute__((__naked__))
#else
#define __naked
#endif

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS   }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#define __weak_symbol       __attribute__((__weak__))
#define __dead2             __attribute__((__noreturn__))
#define __pure2             __attribute__((__const__))
#define __unused            __attribute__((__unused__))
#define __used              __attribute__((__used__))
#define __deprecated        __attribute__((__deprecated__))
#define __deprecated1(msg)  __attribute__((__deprecated__(msg)))
#define __packed            __attribute__((__packed__))
#define __aligned(x)        __attribute__((__aligned__(x)))
#define __section(x)        __attribute__((__section__(x)))
#define __writeonly         __unused
#define __alloc_size(x)     __attribute__((__alloc_size__(x)))
#define __alloc_size2(n, x) __attribute__((__alloc_size__(n, x)))
#define __alloc_align(x)    __attribute__((__alloc_align__(x)))

#endif /* ifndef NORM_SYS_CDEFS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
