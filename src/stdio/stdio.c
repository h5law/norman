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

#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <varg.h>

int open(const char *path, int flag, int mode)
{
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

FILE *fdopen(unsigned int fd, const char *mode)
{
    FILE *ret = NULL;
    ret       = ( FILE * )malloc(sizeof(FILE));
    if (mode[0] == 'a' || mode[0] == 'w')
        ret->io_dir = _IODIR_OUT;
    else
        ret->io_dir = _IODIR_IN;
    memset(ret->buf, 0, BUFSIZ);
    ret->pos   = 0;
    ret->fd    = fd;
    ret->eof   = FALSE;
    ret->error = FALSE;
    return ret;
}

int fflush(FILE *f)
{
    int ret = 0;
    switch (f->io_dir) {
    case _IODIR_IN:
        f->pos = 0;
        break;
    case _IODIR_OUT:
        if (write(f->fd, ( const char * )f->buf, f->pos) != 0)
            ret = -1;
        break;
    }
    f->pos = 0;
    return ret;
}

int _fwrite(const char *ptr, size_t size, FILE *f)
{
    int ret = 0;
    for (int i = 0; (i < size) && (ret == 0); ++i) {
        // TODO: Improve splits (\n, ...) with appropriate stream types
        f->buf[f->pos++] = ptr[i];
        if (f->pos == BUFSIZ) {
            ret    = fflush(f);
            f->pos = 0;
        }
    }
    return ret;
}

size_t fwrite(const void *buf, size_t size, size_t count, FILE *f)
{
    int         i   = 0;
    int         ok  = TRUE;
    const char *ptr = buf;
    for (i = 0; i < count && ok; i++) {
        if (_fwrite(ptr, size, f) != 0)
            ok = FALSE;
        ptr += size;
        // TODO: set error
    }
    return i;
}

int fputs(const char *str, FILE *f)
{
    int l = strlen(str);
    return _fwrite(str, l, f);
}

size_t fread(void *buf, size_t size, size_t n, FILE *f)
{
    int    ret   = 0;
    size_t total = size * n;

    int r = read(f->fd, buf, total);
    if (r < 0) {
        f->error = TRUE;
        ret      = 0;
    } else if (r == 0) {
        f->eof = TRUE;
        ret    = 0;
    } else {
        ret = r / size;
    }

    return ret;
}

int fclose(FILE *f)
{
    fflush(f);
    free(f);
    return 0;
}

int print(char const *str)
{
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return write(STDOUT_FILENO, str, len);
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
