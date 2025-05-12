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

// File handling flags
#define O_RDONLY   0x0000    // open for reading only
#define O_WRONLY   0x0001    // open for writing only
#define O_RDWR     0x0002    // open for reading and writing
#define O_ACCMODE  0x0003    // mask for flags above
#define O_NONBLOCK 0x0004    // do not block on open
#define O_APPEND   0x0008    // append on each write
#define O_CREAT    0x0200    // create file if it does not exist
#define O_TRUNC    0x0400    // truncate size to 0
#define O_EXCL     0x0800    // error if create and file already exists
#define O_EXEC     0x0004000 // open for execute only
#define O_SEARCH   O_EXEC    // open for search only (alias of O_EXEC)
// ...

// File status constants
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif /* ifndef BUFSIZ */

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef enum __io_dir {
    _IODIR_IN = 0,
    _IODIR_OUT = 1
} _io_dir;

typedef struct __sFILE {
    int fd;
    int pos;
    _io_dir io_dir;
    int error;
    int eof;
    unsigned char buf[BUFSIZ];
} FILE;

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
