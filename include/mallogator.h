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
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */

/* LIBRARY DESCRIPTION: 21-50

                                 .--.  .--.
                                /    \/    \
                               | .-.  .-.   \
                               |/_  |/_  |   \
                               || `\|| `\|    `----.
                               |\0_/ \0_/    --,    \_
             .--"""""-.       /              (` \     `-.
            /          \-----'-.              \          \
            \  () ()                         /`\          \
            |           mallogator    .___.-'   |          \
            \                        /` \|      /           ;
             `-.___             ___.' .-.`.---.|             \
                \| ``-..___,.-'`\| / /   /     |              `\
                 `      \|      ,`/ /   /   ,  /
                         `      |\ /   /    |\/
                          ,   .'`-;   '     \/
                     ,    |\-'  .'   ,   .-'`
                   .-|\--;`` .-'     |\.'
                  ( `"'-.|\ (___,.--'`'
                   `-.    `"`          _.--'
                      `.          _.-'`-.
                        `''---''``       `.

TODO:
  -> Look into potentially supporting dynamic (page-aligned) memory backends
     (mmap/malloc/sbrk/...) that grow like vectors (./norm_dyn.h)
  -> Add support to customise the memory backend (mmap/malloc/sbrk/...)
 */

#ifndef NORM_MEM_H
#define NORM_MEM_H

#include <stdint.h>
#include <sys/types.h>

#define NORM_MEM_BACKEND_LIBC_MALLOC 0
#define NORM_MEM_BACKEND_MMAP 1

#ifndef NORM_MEM_BACKEND
#define NORM_MEM_BACKEND NORM_MEM_BACKEND_MMAP
#endif // NORM_MEM_BACKEND

#ifndef NORM_MEM_MAX_CAPACITY
#define NORM_MEM_MAX_CAPACITY 655360
#endif

enum {
    NORM_MEM_ZEROED_FLAG = 0x00000001,
    NORM_MEM_ALLOCED_FLAG = 0x00000002,
    NORM_MEM_FREE_FLAG = 0x00000004,
    // NORM_MEM_MARKED_FLAG = 0x00000008,
};

enum {
    NORM_MEM_ZERO_REGION_OP = 0x00000001,
};

typedef struct norm_mem_header_t norm_mem_header_t;
struct norm_mem_header_t {
    unsigned char status;
    size_t data_size;
    size_t padding_size;
    uintptr_t next_free; // NULL unless status & NORM_MEM_FREE_FLAG > 0
    uintptr_t memory;
};
// } __attribute__((aligned(sizeof(uintptr_t))));

typedef struct norm_mem_ctx_t norm_mem_ctx_t;
struct norm_mem_ctx_t {
    size_t capacity;
    size_t used;
    uintptr_t free;
    uintptr_t memory;
};

#define NORM_MEM_ERR_OKAY (( void * )0)
#define NORM_MEM_ERR_MMAP (( void * )1)
#define NORM_MEM_ERR_NO_CTX (( void * )2)
#define NORM_MEM_ERR_NO_SPACE (( void * )3)

void *norm_mem_init(norm_mem_ctx_t *ctx, size_t min_size);
void *norm_mem_alloc(norm_mem_ctx_t *ctx, size_t size_bytes, uint32_t op_flags);
void *norm_mem_free(norm_mem_ctx_t *ctx, uintptr_t memory, uint32_t op_flags);
void *norm_mem_deinit(norm_mem_ctx_t *ctx);

#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
