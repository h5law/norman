/*
 * Copyright (c) 2025 h5law <dev@h5law.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <sys/mem.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

typedef struct __free_block {
    size_t               size;
    struct __free_block *next;
} free_block;

static free_block   free_block_list_head = {0, NULL};
// memory block width alignment size: machine word size in bits
static const size_t align_to             = sizeof(uintptr_t) * (__WORDSIZE / 8);

int brk(const void *addr)
{
    int x = syscall(SYS_BRK, ( unsigned long )addr);
    return (( unsigned long )x > ( unsigned long )addr) ? 0 : -1;
}

void *sbrk(int incr)
{
    void *old = ( void * )(( uintptr_t )syscall(SYS_BRK, 0));
    void *new = ( void * )( uintptr_t )syscall(
            SYS_BRK, ( uintptr_t )old + ( uintptr_t )incr);
    return ((( uintptr_t )new) == ((( uintptr_t )old) + ( uintptr_t )incr))
                   ? old
                   : (( void * )-1);
}

void *malloc(size_t size)
{
    size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
    free_block  *block = free_block_list_head.next;
    free_block **head  = &(free_block_list_head.next);
    while (block) {
        if (block->size >= size) {
            *head = block->next;
            return (( char * )block) + sizeof(size_t);
        }
        head  = &(block->next);
        block = block->next;
    }

    block       = ( free_block * )sbrk(size);
    block->size = size;
    memset((( char * )block) + sizeof(size_t), 0, size);

    return (( char * )block) + sizeof(size_t);
}

void *calloc(size_t count, size_t size)
{
    size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
    free_block  *block = free_block_list_head.next;
    free_block **head  = &(free_block_list_head.next);
    while (block) {
        if (block->size >= size) {
            *head = block->next;
            memset((( char * )block) + sizeof(size_t), 0, size);
            return (( char * )block) + sizeof(size_t);
        }
        head  = &(block->next);
        block = block->next;
    }

    block       = ( free_block * )sbrk(size);
    block->size = size;
    memset((( char * )block) + sizeof(size_t), 0, size);

    return (( char * )block) + sizeof(size_t);
}

void free(void *ptr)
{
    free_block *block = ( free_block * )((( char * )ptr) - sizeof(size_t));
    block->next       = free_block_list_head.next;
    free_block_list_head.next = block;
}

// vim: ft=c ts=4 sts=4 sw=4 et ai cin
