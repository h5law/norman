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

#ifndef NORM_SYS_H
#define NORM_SYS_H

#ifndef NULL
#define NULL (( void * )0)
#endif /* ifndef NULL */

#ifndef EOF
#define EOF (-1)
#endif /* ifndef EOF */

#ifndef __size_t
typedef __SIZE_TYPE__ size_t;
#endif /* ifndef __size_t */

// TODO: Add other architectures and call numbers
#define SYS_SYSCALL 0
#define SYS_EXIT 1
#define SYS_FORK 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_WAIT4 7
// ...

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif /* ifndef BUF_SIZE */

typedef enum IO_DIR { IO_DIR_IN = 0, IO_DIR_OUT = 1 } IO_DIR;

typedef struct FILE FILE;
struct FILE {
    int fd;
    int pos;
    IO_DIR dir;
    int error;
    int eof;
    unsigned char buf[BUF_SIZE];
};

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

extern char **envp;

extern int main(int argc, char **argv);
int __main(int argc, char **argv);

// defined in the assembler
extern int syscall(int num, ...);
extern void sys_exit(int status);
extern int sys_fork(void);
extern int sys_read(unsigned int fd, char *buf, size_t buf_size);
extern int sys_write(unsigned int fd, const char *buf, size_t buf_size);
extern int sys_open(const char *path, int flag, ...);
extern int sys_close(unsigned int fd);

#endif /* ifndef NORM_SYS_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
