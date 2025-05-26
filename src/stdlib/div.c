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

#include <stdint.h>
#include <stdlib.h>

int mod(int n, int base)
{
    if (n == 0 || base == 0)
        return 0;

    int leading_zeroes = __builtin_clz(base);
    int log2_divisor   = 31 - leading_zeroes;

    // Handle powers of two
    int      is_power_2       = ((base & (base - 1)) == 0);
    int      is_power_2_minus = ((base + 1 & (base)) == 0);
    uint32_t power_2_mask =
            ~(( uint32_t )(( int32_t )0x80000000 >> leading_zeroes));

    if (is_power_2)
        return n & (base - 1);

    unsigned int m;
    if (is_power_2_minus) {
        int                leading_zeroes = __builtin_clz(base + 1);
        int                log2_divisor   = 31 - leading_zeroes;
        const unsigned int s              = log2_divisor;
        for (m = n; n > base; n = m) {
            for (m = 0; n; n >>= s) {
                m += n & base;
            }
        }
        return m == base ? 0 : m;
    }

    return n % base;
}

div_t div(int n, int base)
{
    // Handle negative operands
    if (n < 0 || base < 0) {
        div_t div = {n / base, n % base};
        return div;
    }

    int leading_zeroes = __builtin_clz(base);
    int log2_divisor   = 31 - leading_zeroes;

    // Handle powers of two
    int      is_power_2 = ((base & (base - 1)) == 0);
    uint32_t power_2_mask =
            ~(( uint32_t )(( int32_t )0x80000000 >> leading_zeroes));

    if (is_power_2) {
        div_t div = {(n >> log2_divisor), n & power_2_mask};
        return div;
    }

    // General case using multiply-shift
    uint64_t m         = ( uint64_t )1 << (log2_divisor + 32);
    uint64_t c_floor   = m / base;
    uint64_t c_ceiling = c_floor + 1;

    // Fallback to multiply-add-shift
    uint32_t N             = 0xFFFFFFFF;
    int      need_fallback = (base - (m % base) >= m / N + 1);
    uint64_t c             = need_fallback ? c_floor : c_ceiling;

    uint64_t prod;
    if (need_fallback)
        prod = (c * n + c);
    else
        prod = c * n;

    int   quot = prod >> (32 + log2_divisor);
    int   rem  = mod(n - (quot * base), base);
    div_t div  = {quot, rem};

    return div;
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
