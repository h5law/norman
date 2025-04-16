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

/* LIBRARY DESCRIPTION: 21-39
 * dynamic types implemented from scratch with zero dependencies outside of
 * the norm(an) library itself. Types include:
 *  -> dynamic arrays/vectors
 *  -> hashmaps with customisable probing and hashing mechanisms
 *
 *   TODO:
 *  -> Add tests covering the different possible outcomes of the implementation
 *  -> Add some proper documentation in the header comment and hint comments
 *  -> Think about and support NULL/zero detection for size changes
 *  -> Add a default hasher_fn - implementing SipHash 2-4/xxHash/lookup/murmur/
 *     cityhash/etc from paper/spec
 *  -> Add quadratic probic probe_fn implementation
 *  -> Replace probe_fn arg with global enum
 *  -> Support non-string key types (hash the byte values of the key - not a
 *     pointer)
 *  -> Tidy up guards for header, tests and implementation
 *  -> Remove allocs / allow for custom alloc function definitions
 */

#ifndef NORM_DYN_DS_H
#define NORM_DYN_DS_H

#define NORM_DYN_ERR_OKAY 0
#define NORM_DYN_ERR_IARGS 1
#define NORM_DYN_ERR_ALLOC 2
#define NORM_DYN_ERR_OFLOW 3
#define NORM_DYN_ERR_UFLOW 4
#define NORM_DYN_ERR_IIDX 5

#include <sys/types.h>

struct norm_vector_t {
    // size: number of elements currently in the array
    // end_ptr: the index of the next empty with the greatest index
    // capacity: the size of the array in total, counting empty slots
    size_t size, end_ptr, capacity;
    // Percentage of the used capacity before a grow occurs
    // Example:
    //  load_factor = 0.8 -> grow at 80% used
    double load_factor;
    void *array;
};
typedef struct norm_vector_t norm_vector_t;

void *norm_vector_gpos(norm_vector_t *vec, size_t index, size_t elem_size);
int norm_vector_spos(norm_vector_t *vec, size_t index, void *elem,
                     size_t elem_size);
int norm_vector_zpos(norm_vector_t *vec, size_t index, size_t elem_size);
int norm_vector_push(norm_vector_t *vec, void *elem, size_t elem_size);
int norm_vector_compact(norm_vector_t *vec, size_t elem_size);
int norm_vector_resize(norm_vector_t *vec, size_t new_capacity,
                       size_t elem_size);
int norm_vector_empty(norm_vector_t *vec, size_t elem_size);
norm_vector_t norm_vector_init(size_t min_capacity, double load_factor,
                               size_t elem_size);
int norm_vector_free(norm_vector_t *vec);
norm_vector_t norm_vector_clone(norm_vector_t *vec, size_t elem_size);
norm_vector_t norm_vector_concat(norm_vector_t *vec1, norm_vector_t *vec2,
                                 size_t elem_size);

#ifndef NORM_MAP_MAX_KEY_LEN
#define NORM_MAP_MAX_KEY_LEN 32
#endif

struct norm_map_entry_t {
    size_t key_length;
    size_t elem_size;
    char kv[];
};
typedef struct norm_map_entry_t norm_map_entry_t;

typedef size_t (*norm_map_hasher_fn)(const char *key, size_t key_length);
typedef size_t (*norm_map_probe_fn)(norm_map_hasher_fn hasher, void *table,
                                    const char *key, size_t key_length,
                                    size_t elem_size, size_t table_length);

struct norm_map_t {
    norm_vector_t table;
    norm_map_hasher_fn hasher_fn;
    norm_map_probe_fn probe_fn;
};
typedef struct norm_map_t norm_map_t;

norm_map_t norm_map_init(size_t min_capacity, double load_factor,
                         size_t elem_size, norm_map_hasher_fn hasher_fn,
                         norm_map_probe_fn probe_fn);

int norm_map_free(norm_map_t *map);
int norm_map_rehash(norm_map_t *map, size_t elem_size);
void *norm_map_get(norm_map_t *map, const char *key, size_t key_length,
                   size_t elem_size);
int norm_map_set(norm_map_t *map, const char *key, void *elem,
                 size_t key_length, size_t elem_size);
int norm_map_delete(norm_map_t *map, const char *key, size_t key_length,
                    size_t elem_size);
int norm_map_clear(norm_map_t *map, size_t elem_size);

size_t norm_map_linear_probe(norm_map_hasher_fn hasher, void *table,
                             const char *key, size_t key_length,
                             size_t elem_size, size_t table_length);

#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
