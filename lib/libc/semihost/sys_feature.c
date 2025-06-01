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

#include <stdint.h>
#include <semihost/shdefs.h>
#include <semihost/calls.h>
#include <string.h>

#define SH_EXT_NUM_BYTES ((SH_EXT_NUM + 7) >> 3)

static int     got_feature_bytes;
static uint8_t feature_bytes[SH_EXT_NUM_BYTES];

static const uint8_t fb_magic[4] = {
        SHFB_MAGIC_0,
        SHFB_MAGIC_1,
        SHFB_MAGIC_2,
        SHFB_MAGIC_3,
};

static void get_features(void)
{
    if (got_feature_bytes)
        return;
    got_feature_bytes = 1;

    int fd            = sys_semihost_open(":semihosting-features", 0);
    if (fd == -1)
        return;

    int len = sys_semihost_flen(fd);
    if (len < ( int )sizeof(fb_magic))
        goto do_close;

    uint8_t magic[sizeof(fb_magic)];
    if (sys_semihost_read(fd, magic, sizeof(fb_magic)) != 0)
        goto do_close;
    if (memcmp(magic, fb_magic, sizeof(fb_magic)) != 0)
        goto do_close;

    int to_read = len - sizeof(fb_magic);
    if (to_read > ( int )sizeof(feature_bytes))
        to_read = sizeof(feature_bytes);

    ( void )sys_semihost_read(fd, feature_bytes, to_read);
do_close:
    sys_semihost_close(fd);
}

int sys_semihost_feature(uint8_t feature)
{
    get_features();
    uint8_t byte = (feature >> 3);
    uint8_t bit  = (feature & 7);
    return (feature_bytes[byte] & (1 << bit)) != 0;
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
