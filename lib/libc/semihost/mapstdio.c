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

#include <semihost/shdefs.h>
#include <semihost/calls.h>
#include <sys/types.h>
#include <unistd.h>

static int fd_stdout, fd_stderr;
static int _check_done;

int semihost_map_stdio(int fd)
{
    if (!_check_done) {
        _check_done = 1;
        fd_stdout   = semihost_open(":tt", 4);
        fd_stderr   = semihost_open(":tt", 8);
    }
    switch (fd) {
    case 0:
        return -1;
    case 1:
        return fd_stdout;
    case 2:
        return fd_stderr;
    default:
        return fd;
    }
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
