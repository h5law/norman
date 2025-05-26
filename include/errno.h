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

/* LIBRARY DESCRIPTION: 21-25
 * A general implementation of standardised errors for use throughout norm
 * including errno and a standard error type to be manipulated by functions
 * to describe errors in specific details
 */

#ifndef NORM_ERR_H
#define NORM_ERR_H

#define ENOERR  0  /* No error / not used */
#define EPERM   1  /* Operation not permitted */
#define ENOENT  2  /* No such file or directory */
#define ESRCH   3  /* No such process */
#define EINTR   4  /* Interrupted system call */
#define EIO     5  /* I/O error */
#define ENXIO   6  /* No such device or address */
#define E2BIG   7  /* Argument list too long */
#define ENOEXEC 8  /* Exec format error */
#define EBADF   9  /* Bad file descriptor */
#define ECHILD  10 /* No child processes */
#define EAGAIN  11 /* Try again */
#define ENOMEM  12 /* Out of memory */
#define EACCES  13 /* Permission denied */
#define EFAULT  14 /* Bad address */
#define ENOTBLK 15 /* Block device required */
#define EBUSY   16 /* Device or resource busy */
#define EEXIST  17 /* File exists */
#define EXDEV   18 /* Cross-device link */
#define ENODEV  19 /* No such device */
#define ENOTDIR 20 /* Not a directory */
#define EISDIR  21 /* Is a directory */
#define EINVAL  22 /* Invalid argument */
#define ENFILE  23 /* File table overflow */
#define EMFILE  24 /* Too many open files */
#define ENOTTY  25 /* Inappropriate ioctl for device */
#define ETXTBSY 26 /* Text file busy */
#define EFBIG   27 /* File too large */
#define ENOSPC  28 /* No space left on device */
#define ESPIPE  29 /* Illegal seek */
#define EROFS   30 /* Read-only file system */
#define EMLINK  31 /* Too many links */
#define EPIPE   32 /* Broken pipe */
#define EDOM    33 /* Math argument out of domain of func */
#define ERANGE  34 /* Math result not representable */

extern int errno;

#define sys_nerr 34
const char *const sys_errlist[] = {"No error",
                                   "Operation not permitted",
                                   "No such file or directory",
                                   "No such process",
                                   "Interrupted system call",
                                   "I/O error",
                                   "No such device or address",
                                   "Argument list too long",
                                   "Exec format error",
                                   "Bad file number",
                                   "No child processes",
                                   "Try again",
                                   "Out of memory",
                                   "Permission denied",
                                   "Bad address",
                                   "Block device required ",
                                   "Device or resource busy",
                                   "File exists",
                                   "Cross-device link",
                                   "No such device",
                                   "Not a directory",
                                   "Is a directory",
                                   "Invalid argument",
                                   "File table overflow",
                                   "Too many open files",
                                   "Not a typewriter",
                                   "Text file busy",
                                   "File too large",
                                   "No space left on device",
                                   "Illegal seek",
                                   "Read-only file system",
                                   "Too many links",
                                   "Broken pipe",
                                   "Math argument out of domain of func",
                                   "Math result not representable"};

void  perror(const char *str);
char *strerror(int errnum);

int _set_errno(int num);

#endif /* ifndef NORM_ERR_H */

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
