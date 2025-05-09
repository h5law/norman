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

#include "stdio.h"

int open(const char *path, int flag, ...)
{
    int mode = O_RDONLY;

    if (flag & O_CREAT) {
        va_list ap;
        mode |= va_arg(ap, int);
        va_end(ap);
    }

    return syscall(SYS_OPEN, path, flag, mode);
}

int close(unsigned int fd) { return syscall(SYS_CLOSE, fd); }

int read(unsigned int fd, char *buf, size_t count)
{
    return syscall(SYS_READ, fd, buf, count);
}

int write(unsigned int fd, const char *buf, size_t count)
{
    return syscall(SYS_WRITE, fd, buf, count);
}

int print(char const *f)
{
    size_t len = 0;

    while (f[len] != '\0') {
        ++len;
    }

    return write(STDOUT_FILENO, f, len);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
