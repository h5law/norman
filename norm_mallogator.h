/*
Copyright (c) 2024 Harry Law <h5law>

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

Harry Law
harry@h5law.com

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
  -> Define a underlying data structure to back the heap/arena and its metadata
    (map?)
  -> Support multiple arenas/pools/heaps not a single global instance

Memory Model:
    [  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ]
    [  {H | B      }  { H | B } { H | B          }       { H | B }           ]
    [                                               FFF            FFF  FFF  ]
    [     01-2          01-1          01-3          00-1   01-1      00-2    ]
    -> dealloc(#4)
    [  {H | B      }  { H | B } { H | B          }          FFF              ]
    [                                               FFF     FFF    FFF  FFF  ]
    [     01-2          01-1          01-3                    00-4           ]
    [
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NORM_MEM_H
#define NORM_MEM_H

#include <sys/types.h>

#ifndef NORM_MEM_MAX_CAPACITY
#define NORM_MEM_MAX_CAPACITY 655360
#endif

typedef struct norm_mem_header_t norm_mem_header_t;
struct norm_mem_header_t {
    unsigned char status; /* Memory Block Status Masks
                             - 0001 active
                             - 0010 marked
                             - 0100 free
                             - 1000 zero'd
                             - NOT USED CURRENTLY
                          */
    size_t data_size;
    size_t padding_size;
    norm_mem_header_t *next;
};

typedef struct norm_mem_ctx_t norm_mem_ctx_t;
struct norm_mem_ctx_t {
    size_t capacity;
    norm_mem_header_t *free;
    uintptr_t memory;
};

#define NORM_MEM_ERR_OKAY ((void *)0)
#define NORM_MEM_ERR_MMAP ((void *)1)
#define NORM_MEM_ERR_NO_CTX ((void *)2)

void *norm_mem_init(norm_mem_ctx_t *ctx, size_t min_size);
void *norm_mem_alloc(norm_mem_ctx_t *ctx, size_t size_bytes);
void *norm_mem_free(norm_mem_ctx_t *ctx);

#endif // !NORM_MEMORY_H

#ifdef NORM_MEM_IMPLEMENTATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static inline int memzcmp(void *memory, size_t size)
{
    unsigned char val = 0;
    unsigned char *mm = (unsigned char *)memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

static inline size_t norm__mem_align_to_pagesize(size_t unaligned)
{
    size_t pagesize = getpagesize();
    return ((pagesize - 1) & unaligned)
                   ? ((unaligned + pagesize) & ~(pagesize - 1))
                   : unaligned;
}

void *norm_mem_init(norm_mem_ctx_t *ctx, size_t size)
{
    if (!ctx)
        return NORM_MEM_ERR_NO_CTX;

    void *memory;
    size_t width;
    width = size == 0 ? norm__mem_align_to_pagesize(NORM_MEM_MAX_CAPACITY)
                      : norm__mem_align_to_pagesize(size);

    memory = mmap(NULL, width, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON,
                  -1, 0);
    if (memory == MAP_FAILED)
        return NORM_MEM_ERR_MMAP;

    norm_mem_header_t free;
    free.status = (1 << 2) | (1 << 3); // free and zero'd
    free.data_size = width - sizeof(norm_mem_header_t);
    free.padding_size = 0;
    free.next = NULL;
    memmove(((char *)(memory)), &free, sizeof(norm_mem_header_t));

    ctx->capacity = width;
    ctx->free = (norm_mem_header_t *)memory;
    ctx->memory = (uintptr_t)memory;

    return NORM_MEM_ERR_OKAY;
}

void *norm_mem_alloc(norm_mem_ctx_t *ctx, size_t size)
{
    if (!ctx)
        return NORM_MEM_ERR_NO_CTX;

    // size_t width = size + sizeof(norm_mem_header_t);

    return NULL;
}

// char *norm_mem_pool_alloc(size_t size_bytes)
// {
//     size_t size, width, left, right, start, end;
//     start = (char *)norm__memory_block;
//     end = (char *)norm__memory_block + (norm__memory_capacity - 1);
//     size = size_bytes ? size_bytes : 1;
//
//     return NULL;
// }
//

void *norm_mem_pool_free(norm_mem_ctx_t *ctx)
{
    munmap((void *)ctx->memory, ctx->capacity);
    memset(ctx, 0, sizeof(norm_mem_ctx_t));
    return NORM_MEM_ERR_OKAY;
}

#endif // !NORM_MEM_IMPLEMENTATION

#ifdef __cplusplus
} /* extern "C" */
#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et
