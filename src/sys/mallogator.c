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

// #include <stdio.h>
// #include <string.h>
// #include <sys/mman.h>
// #include <unistd.h>
//
// #include <mallogator.h>
//
// static inline size_t norm__mem_align_to_pagesize(size_t unaligned)
// {
//     size_t pagesize = getpagesize();
//     return ((pagesize - 1) & unaligned)
//                    ? ((unaligned + pagesize) & ~(pagesize - 1))
//                    : unaligned;
// }
//
// static inline size_t norm__mem_pointer_align(size_t unaligned)
// {
//     size_t rem  = unaligned & (sizeof(uintptr_t) - 1);
//     size_t diff = rem > 0 ? sizeof(uintptr_t) - rem : 0;
//     return (diff + unaligned);
// }
//
// void *norm_mem_init(norm_mem_ctx_t *ctx, size_t size)
// {
//     if (!ctx)
//         return NORM_MEM_ERR_NO_CTX;
//
//     void  *memory;
//     size_t width;
//     width = size == 0 ? norm__mem_align_to_pagesize(NORM_MEM_MAX_CAPACITY)
//                       : norm__mem_align_to_pagesize(size);
//
//     memory = mmap(NULL, width, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON,
//                   -1, 0);
//     if (memory == MAP_FAILED)
//         return NORM_MEM_ERR_MMAP;
//
//     norm_mem_header_t free;
//     free.status       = NORM_MEM_FREE_FLAG | NORM_MEM_ZEROED_FLAG;
//     free.data_size    = width - sizeof(norm_mem_header_t);
//     free.padding_size = 0;
//     free.next_free    = ( uintptr_t )NULL;
//     free.memory       = ( uintptr_t )( void * )(( unsigned char * )memory +
//                                           sizeof(norm_mem_header_t));
//     memmove((( char * )(memory)), &free, sizeof(norm_mem_header_t));
//
//     ctx->capacity = width;
//     ctx->used     = sizeof(norm_mem_header_t);
//     ctx->free     = ( uintptr_t )( void * )memory;
//     ctx->memory   = ( uintptr_t )( void * )memory;
//
//     return NORM_MEM_ERR_OKAY;
// }
//
// void *norm_mem_alloc(norm_mem_ctx_t *ctx, size_t size, uint32_t op_flags)
// {
//     if (!ctx)
//         return NORM_MEM_ERR_NO_CTX;
//
//     size_t padded_length;
//     padded_length = norm__mem_pointer_align(size);
//
//     norm_mem_header_t *prev, *free;
//     prev = ( norm_mem_header_t * )( void * )ctx->free;
//     free = ( norm_mem_header_t * )( void * )ctx->free;
//     // printf("s = %ld\tfds = %ld\n", size, free->data_size);
//     while (free && free->data_size < padded_length) {
//         if (!free->next_free)
//             return NORM_MEM_ERR_NO_SPACE;
//         // printf("(free) probe: %ld\n", free->data_size);
//         prev = free;
//         free = ( norm_mem_header_t * )( void * )free->next_free;
//     }
//     if (!free)
//         return NORM_MEM_ERR_NO_SPACE;
//
//     // printf("(width): %ld    (ds): %zu    (ptr):  0x%-lx\n", padded_length,
//     //        free->data_size, (uintptr_t)(void *)free);
//
//     norm_mem_header_t *next_free_header = NULL;
//     if (free->data_size < (padded_length + sizeof(norm_mem_header_t))) {
//         next_free_header = ( norm_mem_header_t * )( void * )free->next_free;
//         prev->next_free  = free->next_free;
//         ctx->used -= sizeof(norm_mem_header_t);
//     } else {
//         next_free_header =
//                 ( norm_mem_header_t * )( void * )(( unsigned char * )
//                                                           free->memory +
//                                                   padded_length);
//
//         next_free_header->data_size =
//                 free->data_size - (padded_length +
//                 sizeof(norm_mem_header_t));
//         next_free_header->status =
//                 NORM_MEM_FREE_FLAG | (free->status & NORM_MEM_ZEROED_FLAG);
//
//         next_free_header->memory =
//                 free->memory + padded_length + sizeof(norm_mem_header_t);
//
//         next_free_header->next_free = free->next_free;
//         prev->next_free =
//                 ( uintptr_t )( void * )(( unsigned char * )(free->memory) +
//                                         padded_length);
//         ctx->used += sizeof(norm_mem_header_t);
//     }
//     if (( uintptr_t )( void * )free == ctx->free)
//         ctx->free = ( uintptr_t )( void * )next_free_header;
//     // printf("next -> 0x%-lx\n", (uintptr_t)(void *)next_free_header);
//
//     ctx->used += padded_length;
//
//     norm_mem_header_t *new_mem_header = free;
//     size_t             padding_width;
//     padding_width                = padded_length - size;
//     new_mem_header->data_size    = size;
//     new_mem_header->padding_size = padding_width;
//     new_mem_header->next_free    = ( uintptr_t )( void * )NULL;
//     new_mem_header->status =
//             (free->status & NORM_MEM_ZEROED_FLAG) | NORM_MEM_ALLOCED_FLAG;
//
//     if (op_flags & NORM_MEM_ZERO_REGION_OP) {
//         new_mem_header->status |= NORM_MEM_ZEROED_FLAG;
//         memset(( unsigned char * )( void * )(new_mem_header->memory), 0,
//                new_mem_header->data_size + new_mem_header->padding_size);
//     }
//
//     return ( void * )new_mem_header->memory;
// }
//
// void *norm_mem_free(norm_mem_ctx_t *ctx, uintptr_t memory, uint32_t op_flags)
// {
//     norm_mem_header_t *header =
//             ( norm_mem_header_t
//                       * )( void * )( unsigned char
//                                              * )(( unsigned char * )(memory)
//                                              -
//                                                  sizeof(norm_mem_header_t));
//     header->status ^= NORM_MEM_FREE_FLAG | NORM_MEM_ALLOCED_FLAG;
//     header->data_size += header->padding_size;
//     header->padding_size = 0;
//     ctx->used -= header->data_size;
//     // printf("(free) 0x%-lx\n", (uintptr_t)(void *)header);
//     if (!ctx->free)
//         ctx->free = ( uintptr_t )( void * )header;
//     else if (ctx->free > ( uintptr_t )( void * )header) {
//         header->next_free = ctx->free;
//         ctx->free         = ( uintptr_t )( void * )header;
//     } else {
//         norm_mem_header_t *prev = ( norm_mem_header_t * )( void * )ctx->free;
//         while (prev && prev->next_free < ( uintptr_t )( void * )header) {
//             if (( void * )prev->next_free == NULL)
//                 break;
//             if (prev->next_free < ( uintptr_t )( void * )header)
//                 prev = ( norm_mem_header_t * )( void * )prev->next_free;
//             else
//                 break;
//         }
//         header->next_free = prev->next_free;
//         prev->next_free   = ( uintptr_t )( void * )header;
//
//         if (( uintptr_t )( void * )((( unsigned char
//                                                * )( void * )(prev->memory)) +
//                                     prev->data_size) ==
//             ( uintptr_t )( void * )header) {
//             prev->data_size += header->data_size + sizeof(norm_mem_header_t);
//             // printf("(free merge) new size: %-zu\n", header->data_size);
//             prev->next_free = header->next_free;
//             ctx->used -= sizeof(norm_mem_header_t);
//             header = prev;
//         }
//     }
//
//     if (( uintptr_t )( void * )((( unsigned char
//                                            * )( void * )(header->memory)) +
//                                 header->data_size) == header->next_free) {
//         norm_mem_header_t *neighbour =
//                 ( norm_mem_header_t * )( void * )header->next_free;
//         header->data_size += neighbour->data_size +
//         sizeof(norm_mem_header_t);
//         // printf("(free merge) new size: %-zu\n", header->data_size);
//         header->next_free = neighbour->next_free;
//         ctx->used -= sizeof(norm_mem_header_t);
//     }
//
//     if (op_flags & NORM_MEM_ZERO_REGION_OP) {
//         header->status |= NORM_MEM_ZEROED_FLAG;
//         memset(( unsigned char * )( void * )(header->memory), 0,
//                header->data_size);
//     }
//
//     // printf("(freed next) %-lx\n", header->next_free);
//
//     return NORM_MEM_ERR_OKAY;
// }
//
// void *norm_mem_deinit(norm_mem_ctx_t *ctx)
// {
//     munmap(( void * )ctx->memory, ctx->capacity);
//     memset(ctx, 0, sizeof(norm_mem_ctx_t));
//     return NORM_MEM_ERR_OKAY;
// }

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
