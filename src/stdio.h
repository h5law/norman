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

#ifndef NORM_STDIO_H
#define NORM_STDIO_H

#include "system.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// typedef struct __sFILE {
//     // ...
// } FILE;
typedef void FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int read(unsigned int fd, char *buf, size_t buf_size);
int write(unsigned int fd, const char *buf, size_t buf_size);
int open(const char *path, int flag, ...);
int close(unsigned int fd);
int print(const char *f);

#endif /* ifndef NORM_STDIO_H */

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
