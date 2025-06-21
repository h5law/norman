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

#ifndef ROLLO_HWINT_H
#define ROLLO_HWINT_H

#include <stdint.h>

typedef volatile uint64_t       io_rw_64; /* 64-bit read+write register */
typedef const volatile uint64_t io_ro_64; /* 64-bit read only register */
typedef volatile uint64_t       io_wo_64; /* 64-bit write only register */

typedef volatile uint32_t       io_rw_32; /* 32-bit read+write register */
typedef const volatile uint32_t io_ro_32; /* 32-bit read only register */
typedef volatile uint32_t       io_wo_32; /* 32-bit write only register */

typedef volatile uint16_t       io_rw_16; /* 16-bit read+write register */
typedef const volatile uint16_t io_ro_16; /* 16-bit read only register */
typedef volatile uint16_t       io_wo_16; /* 16-bit write only register */

typedef volatile uint8_t       io_rw_8;   /* 8-bit read+write register */
typedef const volatile uint8_t io_ro_8;   /* 8-bit read only register */
typedef volatile uint8_t       io_wo_8;   /* 8-bit write only register */

typedef volatile uint8_t *const ioptr;
typedef ioptr const             const_ioptr;

#endif /* #ifndef ROLLO_HWINT_H */
