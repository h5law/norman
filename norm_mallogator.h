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

TODO:
  - Define a underlying data structure to back the heap/arena and its metadata
    (map?)
  - Support multiple arenas/pools/heaps not a single global instance
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NORM_MEMORY_H
#define NORM_MEMORY_H

#ifndef NORM_MEMORY_INCLUDE
#define NORM_MEMORY_INCLUDE

#include <stdint.h>
#include <unistd.h>

#endif // !NORM_MEMORY_INCLUDE

#ifndef NORM_MEMORY_MAX_CAPACITY
#define NORM_MEMORY_MAX_CAPACITY 655360
#endif

#define NORM_MEMORY_MIN_ALLOC 1

typedef struct norm__memory_metadata_blob {
    uint32_t ptr_size;
    uintptr_t ptr_addr;
} norm__memory_metadata_blob;

#define NORM_MEMORY_ERR_OKAY ((void *)0)
#define NORM_MEMORY_ERR_MMAP_POOL ((void *)1)
#define NORM_MEMORY_ERR_MMAP_META ((void *)2)

// #define NORM_MEMORY_META_PTR_MASK (1 << 48) - 1
// #define NORM_MEMORY_META_SIZE_MASK ((1 << 32) - 1) << 47
#define NORM_MEMORY_META_CONV(ptr, size)                                       \
    ({                                                                         \
        norm__memory_metadata_blob blob = {                                    \
                .ptr_size = (uint32_t)size,                                    \
                .ptr_addr = (uintptr_t)ptr,                                    \
        };                                                                     \
        blob;                                                                  \
    })
#define NORM_MEMORY_META_GET_PTR(meta) (uintptr_t)meta.ptr_addr
#define NORM_MEMORY_META_GET_SIZE(meta) (uint32_t)meta.ptr_size

void *norm_mem_pool_alloc(size_t size_bytes);
void *norm_mem_pool_free();

#endif // !NORM_MEMORY_H

#ifdef NORM_MEMORY_IMPLEMENTATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static inline size_t norm__mem_align_to_pagesize(uint32_t unaligned)
{
    size_t pagesize = getpagesize();
    return ((pagesize - 1) & unaligned)
                   ? ((unaligned + pagesize) & ~(pagesize - 1))
                   : unaligned;
}

static size_t norm__memory_capacity;
static norm__memory_metadata_blob *norm__memory_metadata;
static char *norm__memory_block;

#ifndef NORM_MEMORY_INITIALISED
#define NORM_MEMORY_INITIALISED

#define NORM_MEMORY_INIT() norm__memory_initialiser();

static inline void *norm__memory_initialiser()
{
    norm__memory_capacity =
            norm__mem_align_to_pagesize(NORM_MEMORY_MAX_CAPACITY);

    norm__memory_block =
            (char *)mmap(NULL, norm__memory_capacity, PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANON, -1, 0);
    if (norm__memory_block == MAP_FAILED)
        return NORM_MEMORY_ERR_MMAP_POOL;

    norm__memory_metadata = (norm__memory_metadata_blob *)mmap(
            NULL, norm__memory_capacity * sizeof(norm__memory_metadata_blob),
            PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (norm__memory_metadata == MAP_FAILED) {
        munmap(norm__memory_block, norm__memory_capacity);
        return NORM_MEMORY_ERR_MMAP_META;
    }

    norm__memory_metadata[norm__memory_capacity - 1] =
            NORM_MEMORY_META_CONV(norm__memory_block, 0);

    printf("%ldx\t%ldx\n%ld (%d) = %d + %ldx\n",
           (uintptr_t)norm__memory_metadata, (uintptr_t)norm__memory_block,
           norm__memory_capacity - 1, NORM_MEMORY_MAX_CAPACITY - 1,
           NORM_MEMORY_META_GET_SIZE(
                   norm__memory_metadata[norm__memory_capacity - 1]),
           NORM_MEMORY_META_GET_PTR(
                   norm__memory_metadata[norm__memory_capacity - 1]));

    return norm__memory_block;
}
#endif // !NORM_MEMORY_INITIALISED

char *norm_mem_pool_alloc(size_t size_bytes)
{
    size_t size, width, left, right, start, end;
    start = (char *)norm__memory_block;
    end = (char *)norm__memory_block + (norm__memory_capacity - 1);
    size = size_bytes ? size_bytes : 1;

    return NULL;
}

void *norm_mem_pool_free()
{
    munmap(norm__memory_block,
           norm__mem_align_to_pagesize(NORM_MEMORY_MAX_CAPACITY));
    munmap(norm__memory_metadata,
           norm__mem_align_to_pagesize(NORM_MEMORY_MAX_CAPACITY) *
                   sizeof(norm__memory_metadata_blob));
    norm__memory_block = NULL;
    norm__memory_metadata = NULL;
    return NORM_MEMORY_ERR_OKAY;
}

#endif // !NORM_MEMORY_IMPLEMENTATION

#ifdef __cplusplus
} /* extern "C" */
#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et
