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

#include <stdio.h>

FILE __stdin  = {.fd     = STDIN_FILENO,
                 .io_dir = _IODIR_IN,
                 .pos    = 0,
                 .error  = 0,
                 .eof    = 0};
FILE __stdout = {.fd     = STDOUT_FILENO,
                 .io_dir = _IODIR_OUT,
                 .pos    = 0,
                 .error  = 0,
                 .eof    = 0};
FILE __stderr = {.fd     = STDERR_FILENO,
                 .io_dir = _IODIR_OUT,
                 .pos    = 0,
                 .error  = 0,
                 .eof    = 0};

FILE *stdin   = &__stdin;
FILE *stdout  = &__stdout;
FILE *stderr  = &__stderr;

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
