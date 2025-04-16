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
 */

#include <assert.h>
#include <stdlib.h>

#define NORM_MEM_IMPLEMENTATION 1
#define NORM_MEM_MAX_CAPACITY 17000
#include "../norm_mallogator.h"

#define NORM_MEM_TEST_PRINT_OUT 1

void print_mem(norm_mem_ctx_t *ctx)
{
    if (NORM_MEM_TEST_PRINT_OUT != 1)
        return;
    printf("(memory)  free list head:    (ptr: 0x%016lx)\n", ctx->free);
    norm_mem_header_t *curr = (norm_mem_header_t *)(void *)ctx->memory;
    if (curr && ctx->free == ctx->memory &&
        curr->next_free == (uintptr_t)(void *)NULL) {
        printf("(empty)   0x%-lx [%ld]\n", (uintptr_t)(void *)curr,
               curr->data_size);
        curr = NULL;
    }
    while (curr != NULL) {
        size_t offset = 0;
        if (curr->status & NORM_MEM_FREE_FLAG) {
            printf("(free)    0x%016lx (mem: 0x%016lx) [%ld]\n",
                   (uintptr_t)(void *)curr, curr->memory, curr->data_size);
            offset = curr->data_size;
            if (curr->next_free == (uintptr_t)(void *)NULL)
                break;
        } else if (curr->status & NORM_MEM_ALLOCED_FLAG) {
            printf("(alloced) 0x%016lx (mem: 0x%016lx) [%ld + %zu]\n",
                   (uintptr_t)(void *)curr, curr->memory, curr->data_size,
                   curr->padding_size);
            offset = curr->data_size + curr->padding_size;
        }
        curr = (norm_mem_header_t *)(void *)((unsigned char *)(curr->memory) +
                                             offset);
    }
    printf("(memory)  end of allocator headers\n          [cap %ld  used "
           "%ld]\n\n",
           ctx->capacity, ctx->used);
    return;
}

int main(int argc, char *argv[])
{
    norm_mem_ctx_t default_ctx = {0};
    assert(norm_mem_init(&default_ctx, 1024) == NORM_MEM_ERR_OKAY);
    print_mem(&default_ctx);

    void *mem1 = norm_mem_alloc(&default_ctx, 1024, NORM_MEM_ZERO_REGION_OP);
    assert(mem1 != NULL);
    print_mem(&default_ctx);

    void *mem2 = norm_mem_alloc(&default_ctx, 756, NORM_MEM_ZERO_REGION_OP);
    assert(mem2 != NULL);
    print_mem(&default_ctx);

    void *mem3 = norm_mem_alloc(&default_ctx, 506, NORM_MEM_ZERO_REGION_OP);
    assert(mem3 != NULL);
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem2, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    void *mem4 = norm_mem_alloc(&default_ctx, 203, NORM_MEM_ZERO_REGION_OP);
    assert(mem4 != NULL);
    print_mem(&default_ctx);

    void *mem5 = norm_mem_alloc(&default_ctx, 1123, NORM_MEM_ZERO_REGION_OP);
    assert(mem5 != NULL);
    print_mem(&default_ctx);

    void *mem6 = norm_mem_alloc(&default_ctx, 0, NORM_MEM_ZERO_REGION_OP);
    assert(mem6 != NULL);
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem3, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem5, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    void *nmem1, *nmem2; // *nmem3, *nmem4, *nmem5, *nmem6, *nmem7, *nmem8;
    nmem1 = norm_mem_alloc(&default_ctx, 64, NORM_MEM_ZERO_REGION_OP);
    snprintf(nmem1, 64, "memory region 1 allocated");
    print_mem(&default_ctx);

    nmem2 = norm_mem_alloc(&default_ctx, 56, NORM_MEM_ZERO_REGION_OP);
    snprintf(nmem2, 56, "memort region 2 allocated");
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem6, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem1, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    norm_mem_free(&default_ctx, (uintptr_t)mem4, NORM_MEM_ZERO_REGION_OP);
    print_mem(&default_ctx);

    assert(norm_mem_deinit(&default_ctx) == NORM_MEM_ERR_OKAY);

    return EXIT_SUCCESS;
}

// vim: ft=c ts=4 sw=4 sts=4 et cin
