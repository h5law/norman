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

norm_dyn.h:
    This single header file is the definition and implementation for two common
    and useful data structures in pure C99 with zero external dependencies -
    simply standard C headers. This library provides the following:
        -> Generic (void *) dynamic vector/array
        -> Generic (char * -> void *) hashmap with customisable hashing and
           probing/addressing methodologies

TODO:
  -> Add tests covering the different possible outcomes of the implementation
  -> Add some proper documentation in the header comment and hint comments
  -> Think about and support NULL/zero detection for size changes
  -> Add a default hasher_fn - implementing SipHash 2-4/xxHash/lookup/murmur/
     cityhash/etc from paper/spec
  -> Add quadratic probic probe_fn implementation
  -> Replace probe_fn arg with global enum
  -> Support non-string key types (hash the byte values of the key - not a
     pointer)
  -> Tidy up guards for header, tests and implementation
  -> Remove allocs / allow for custom alloc function definitions
 */

#ifdef __cplusplus
extern "C" {
#endif

// #ifdef NORM_DYN_DS_TESTS
// #ifndef NORM_DYN_DS_IMPLEMENTATION
// #define NORM_DYN_DS_IMPLEMENTATION
// #endif /* ifndef NORM_DYN_DS_IMPLEMENTATION */
// #endif

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

#endif /* ifndef NORM_DYN_DS_H */

#ifdef NORM_DYN_DS_IMPLEMENTATION

#include <checkint.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int memzcmp(void *memory, size_t size)
{
    char val = 0;
    unsigned char *mm = (unsigned char *)memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

////////////////////////////////////////////////////////////////////////////////
//                         Dynamic Array Void Methods                         //
////////////////////////////////////////////////////////////////////////////////

norm_vector_t norm_vector_init(size_t min_capacity, double load_factor,
                               size_t elem_size)
{
    norm_vector_t vec = {0};

    if (load_factor <= 0 || load_factor > 1 || min_capacity <= 0 ||
        elem_size <= 0)
        return vec;

    void *array;
    array = calloc(min_capacity, elem_size);
    if (array == NULL)
        return vec;

    vec.capacity = min_capacity;
    vec.load_factor = load_factor;
    vec.array = array;

    return vec;
}

void *norm_vector_gpos(norm_vector_t *vec, size_t index, size_t elem_size)
{
    if (!vec || !vec->array || index < 0 || index >= vec->capacity ||
        elem_size <= 0)
        return NULL;
    return (char *)vec->array + (index * elem_size);
}

int norm_vector_spos(norm_vector_t *vec, size_t index, void *elem,
                     size_t elem_size)
{
    if (!vec || !vec->array || index < 0 || index >= vec->capacity ||
        elem_size <= 0)
        return NORM_DYN_ERR_IARGS;
    int new_size = vec->size;
    if (memzcmp(((char *)(vec->array)) + (index * elem_size), elem_size)) {
        int add_err = CHECKINT_NO_ERROR;
        new_size = check_int32_add(vec->size, 1, &add_err);
        if (add_err != CHECKINT_NO_ERROR) {
            return NORM_DYN_ERR_OFLOW;
        }
    }
    if ((double)new_size >= (double)vec->capacity * vec->load_factor) {
        void *new_arr;
        int mul_err = CHECKINT_NO_ERROR;
        size_t new_cap;
        new_cap = check_int64_mul(vec->capacity, 2, &mul_err);
        if (mul_err != CHECKINT_NO_ERROR)
            return NORM_DYN_ERR_OFLOW;
        new_arr = calloc(new_cap, elem_size);
        if (new_arr == NULL)
            return NORM_DYN_ERR_ALLOC;
        memmove(new_arr, (char *)vec->array, vec->capacity * elem_size);
        free(vec->array);
        vec->array = new_arr;
        vec->capacity = new_cap;
    }
    memmove((char *)vec->array + (index * elem_size), elem, elem_size);
    vec->size = new_size;
    if (index >= vec->end_ptr)
        vec->end_ptr = index + 1;
    return NORM_DYN_ERR_OKAY;
}

int norm_vector_zpos(norm_vector_t *vec, size_t index, size_t elem_size)
{
    if (!vec || !vec->array || vec->size == 0 || index < 0 ||
        index >= vec->end_ptr || elem_size <= 0)
        return NORM_DYN_ERR_IARGS;
    int new_size = vec->size - 1;
    if (memzcmp(((char *)(vec->array)) + (index * elem_size), elem_size)) {
        return NORM_DYN_ERR_OKAY;
    }
    // check for shrink?
    memset((char *)vec->array + (index * elem_size), 0, elem_size);
    vec->size = new_size;
    if (vec->end_ptr == index + 1)
        vec->end_ptr = index;
    return NORM_DYN_ERR_OKAY;
}

int norm_vector_push(norm_vector_t *vec, void *elem, size_t elem_size)
{
    return norm_vector_spos(vec, vec->end_ptr, elem, elem_size);
}

int norm_vector_compact(norm_vector_t *vec, size_t elem_size)
{
    if (!vec || !vec->array || vec->size == 0)
        return NORM_DYN_ERR_IARGS;

    int curr, left, right;
    left = 0;
    right = 0;
    size_t lsize, rsize;
    lsize = 0;
    rsize = 0;
    while (left + (lsize / elem_size) <= vec->end_ptr) {
        curr = memzcmp(((char *)(vec->array)) + (left * elem_size) + lsize,
                       elem_size);
        if (curr) {
            lsize += elem_size;
            continue;
        }
        if (left == 0 && lsize == 0) {
            ++left;
            continue;
        }
        right = left + (lsize / elem_size);
        curr = memzcmp(((char *)(vec->array)) + (right * elem_size) + rsize,
                       elem_size);
        if (!curr && right + (rsize / elem_size) < vec->end_ptr) {
            rsize += elem_size;
            continue;
        }
        memmove(((char *)(vec->array)) + (left * elem_size),
                ((char *)(vec->array)) + (right * elem_size), rsize);
        size_t offset = rsize > lsize ? rsize - lsize : lsize - rsize;
        memset(((char *)(vec->array)) + (left * elem_size) + rsize, 0,
               elem_size + rsize + offset);
        if (rsize < lsize)
            left = left + (rsize / elem_size);
        else
            left = right + (rsize / elem_size);
        lsize = 0;
        rsize = 0;
    }

    vec->end_ptr = vec->size;
    return NORM_DYN_ERR_OKAY;
}

norm_vector_t norm_vector_clone(norm_vector_t *vec, size_t elem_size)
{
    norm_vector_t dup = {0};
    void *array;
    array = calloc(vec->capacity, elem_size);
    if (array == NULL)
        return dup;
    memmove((char *)array, (char *)vec->array, vec->capacity * elem_size);
    dup.capacity = vec->capacity;
    dup.size = vec->size;
    dup.end_ptr = vec->end_ptr;
    dup.load_factor = vec->load_factor;
    dup.array = array;
    return dup;
}

int norm_vector_empty(norm_vector_t *vec, size_t elem_size)
{
    if (!vec || !vec->array || vec->size == 0)
        return NORM_DYN_ERR_IARGS;
    memset((char *)vec->array, 0, vec->capacity * elem_size);
    vec->size = 0;
    vec->end_ptr = 0;
    return NORM_DYN_ERR_OKAY;
}

norm_vector_t norm_vector_concat(norm_vector_t *vec1, norm_vector_t *vec2,
                                 size_t elem_size)
{
    norm_vector_t new = {0};

    if (!vec1 || !vec1->array || !vec2 || !vec2->array)
        return new;

    void *array;
    array = calloc(vec1->capacity + vec2->capacity, elem_size);
    if (array == NULL)
        return new;
    memmove((char *)array, (char *)vec1->array, vec1->capacity * elem_size);
    memmove((char *)array + (vec1->capacity * elem_size), (char *)vec2->array,
            vec2->capacity * elem_size);

    new.size = vec1->size + vec2->size;
    new.capacity = vec1->capacity + vec2->capacity;
    new.end_ptr = vec1->capacity + vec2->end_ptr - 1;
    new.load_factor = fmin(vec1->load_factor, vec2->load_factor);
    new.array = array;

    return new;
}

int norm_vector_resize(norm_vector_t *vec, size_t new_capacity,
                       size_t elem_size)
{
    if (!vec || !vec->array)
        return NORM_DYN_ERR_IARGS;

    void *new_array;
    new_array = calloc(new_capacity, elem_size);
    if (new_array == NULL)
        return NORM_DYN_ERR_ALLOC;

    int x = new_capacity;
    int y = vec->capacity;
    int len = x > y ? y : x;
    memmove(new_array, (char *)vec->array, len * elem_size);
    free(vec->array);

    if (new_capacity < vec->size) {
        vec->size = new_capacity;
        vec->end_ptr = new_capacity;
    }

    vec->array = new_array;
    vec->capacity = new_capacity;

    return NORM_DYN_ERR_OKAY;
}

int norm_vector_free(norm_vector_t *vec)
{
    if (!vec || !vec->array)
        return NORM_DYN_ERR_IARGS;
    free(vec->array);
    memset(vec, 0, sizeof(norm_vector_t));
    return NORM_DYN_ERR_OKAY;
}

////////////////////////////////////////////////////////////////////////////////
//                               HashMap Methods                              //
////////////////////////////////////////////////////////////////////////////////

static inline void *norm__map_entry_value(norm_map_entry_t *entry)
{
    // return entry + (2 * sizeof(size_t)) + NORM_MAP_MAX_KEY_LEN;
    return entry->kv + NORM_MAP_MAX_KEY_LEN;
}

static inline size_t norm__map_entry_size(size_t elem_size)
{
    return sizeof(norm_map_entry_t) + NORM_MAP_MAX_KEY_LEN + elem_size;
}

norm_map_t norm_map_init(size_t min_capacity, double load_factor,
                         size_t elem_size, norm_map_hasher_fn hasher_fn,
                         norm_map_probe_fn probe_fn)
{
    norm_map_t map = {0};
    if (load_factor <= 0 || load_factor > 1 || min_capacity <= 0 ||
        elem_size <= 0 || hasher_fn == NULL)
        return map;
    uint32_t cap2 = min_capacity;
    --cap2;
    cap2 |= cap2 >> 1;
    cap2 |= cap2 >> 2;
    cap2 |= cap2 >> 4;
    cap2 |= cap2 >> 8;
    cap2 |= cap2 >> 16;
    ++cap2;
    norm_vector_t vec = {0};
    vec = norm_vector_init(cap2, load_factor, norm__map_entry_size(elem_size));
    if (vec.capacity != cap2)
        return map;
    map.table = vec;
    map.hasher_fn = hasher_fn;
    map.probe_fn = probe_fn;
    return map;
}

int norm_map_free(norm_map_t *map)
{
    if (!map)
        return NORM_DYN_ERR_IARGS;
    int res = NORM_DYN_ERR_OKAY;
    res = norm_vector_free(&(map->table));
    if (res != NORM_DYN_ERR_OKAY)
        return res;
    memset(map, 0, sizeof(norm_map_t));
    return NORM_DYN_ERR_OKAY;
}

int norm_map_set(norm_map_t *map, const char *key, void *elem,
                 size_t key_length, size_t elem_size)
{
    if (!map || key_length <= 0 || key_length > NORM_MAP_MAX_KEY_LEN ||
        elem_size <= 0)
        return NORM_DYN_ERR_IARGS;
    int res = NORM_DYN_ERR_OKAY;
    size_t index, entry_size, start_table_cap;
    start_table_cap = map->table.capacity;
    entry_size = norm__map_entry_size(elem_size);
    index = map->probe_fn(map->hasher_fn, (void *)(map->table.array), key,
                          key_length, entry_size, map->table.capacity);
    if (index == ~((size_t)0))
        return NORM_DYN_ERR_IIDX;
    norm_map_entry_t *entry = calloc(
            1, sizeof(norm_map_entry_t) + NORM_MAP_MAX_KEY_LEN + elem_size);
    if (entry == NULL)
        return NORM_DYN_ERR_ALLOC;
    entry->key_length = key_length;
    entry->elem_size = elem_size;
    memmove(entry->kv, key, key_length);
    memmove(((char *)(entry->kv)) + NORM_MAP_MAX_KEY_LEN, elem, elem_size);
    res = norm_vector_spos(&(map->table), index, entry, entry_size);
    free(entry);
    if (res != NORM_DYN_ERR_OKAY) {
        return res;
    }
    if (map->table.capacity != start_table_cap)
        return norm_map_rehash(map, elem_size);
    return NORM_DYN_ERR_OKAY;
}

void *norm_map_get(norm_map_t *map, const char *key, size_t key_length,
                   size_t elem_size)
{
    size_t index, entry_size;
    entry_size = norm__map_entry_size(elem_size);
    index = map->probe_fn(map->hasher_fn, map->table.array, key, key_length,
                          entry_size, map->table.capacity);
    if (index == ~((size_t)0))
        return NULL;
    norm_map_entry_t *entry =
            norm_vector_gpos(&(map->table), index, entry_size);
    if (entry == NULL)
        return NULL;
    if (entry->key_length != key_length ||
        memcmp((char *)(entry->kv), key, key_length) != 0)
        return NULL;
    return (void *)(((char *)(entry->kv)) + NORM_MAP_MAX_KEY_LEN);
}

int norm_map_delete(norm_map_t *map, const char *key, size_t key_length,
                    size_t elem_size)
{
    size_t index, entry_size;
    entry_size = norm__map_entry_size(elem_size);
    index = map->probe_fn(map->hasher_fn, map->table.array, key, key_length,
                          entry_size, map->table.capacity);
    if (index == ~((size_t)0))
        return NORM_DYN_ERR_IIDX;
    return norm_vector_zpos(&(map->table), index, entry_size);
}

int norm_map_clear(norm_map_t *map, size_t elem_size)
{
    return norm_vector_empty(&(map->table), norm__map_entry_size(elem_size));
}

int norm_map_rehash(norm_map_t *map, size_t elem_size)
{
    size_t entry_size;
    entry_size = norm__map_entry_size(elem_size);
    char *new_arr = calloc(map->table.capacity, entry_size);
    if (new_arr == NULL)
        return NORM_DYN_ERR_ALLOC;
    size_t index;
    size_t end_ptr = 0;
    for (size_t i = 0; i < map->table.capacity; ++i) {
        norm_map_entry_t *entry;
        entry = norm_vector_gpos(&(map->table), i, entry_size);
        if (entry == NULL || memzcmp(entry, entry_size))
            continue;
        index = map->probe_fn(map->hasher_fn, new_arr, (char *)entry->kv,
                              entry->key_length, entry_size,
                              map->table.capacity);
        if (index == ~((size_t)0))
            return NORM_DYN_ERR_IIDX;
        if (index > end_ptr)
            end_ptr = index;
        memmove(new_arr + (index * entry_size), entry, entry_size);
    }
    free(map->table.array);
    map->table.array = new_arr;
    map->table.end_ptr = end_ptr + 1;
    return NORM_DYN_ERR_OKAY;
}

// TODO: Implement a hasher to be used in the default hasher_fn
//       Implement either SipHash 2-4 or xxHash from scratch (64 bit variant)

size_t norm_map_linear_probe(norm_map_hasher_fn hasher_fn, void *table,
                             const char *key, size_t key_length,
                             size_t elem_size, size_t table_length)
{
    size_t index;
    index = hasher_fn(key, key_length);
    index = index & (table_length - 1);
    norm_map_entry_t *curr = calloc(1, elem_size);
    if (curr == NULL)
        return ~((size_t)0);
    memmove((char *)curr, ((char *)(table)) + (index * elem_size), elem_size);
    while (!memzcmp((char *)curr, elem_size)) {
        if (memcmp(curr->kv, key, key_length) == 0) {
            break;
        }
        index = (index + 1) & (table_length - 1);
        memset((char *)curr, 0, elem_size);
        memmove((char *)curr, ((char *)(table)) + (index * elem_size),
                elem_size);
    }
    free(curr);
    return index;
}

#endif /* ifdef NORM_DYN_DS_IMPLEMENTATION */

////////////////////////////////////////////////////////////////////////////////
//                                  Test Suite                                //
////////////////////////////////////////////////////////////////////////////////

// #ifdef NORM_DYN_DS_TESTS
//
// static char *statuses[2] = {"✅", "❌"};
//
// typedef struct norm_dyn_ds_test_case {
//   // char name[128], desc[256], cmd[512];
//   const char *name, *desc, *condition;
//   int status;
// } norm_dyn_ds_test_case;
//
// #define NORM__DYN_DS_TEST_RUNNER(name, desc, cond, result) \
//   ({ \
//     const char *status = result ? statuses[0] : statuses[1]; \
//     printf("%s [%d] : %s → %s\n\t↳ %s\n", __FILE__, __LINE__, name,
//     status, \
//            result == 0 ? desc : cond); \
//   })
//
// void norm_dyn_ds_test_suite(void) {
//   norm_vector_t zvec = {0};
//
//   norm_vector_t vec = norm_vector_init(10, 0.7, 4);
//
//   norm_dyn_ds_test_case cases[1] = {
//       {
//           .name = "vector initialises non-null",
//           .desc = "norm_vector_init() returns an initialised vector",
//           .condition =
//               "memcmp(&norm_vector_init(), 0, sizeof(norm_vector_t)) !=
//               0",
//           .status = memcmp(&vec, &zvec, sizeof(norm_vector_t)) != 0,
//       },
//       {
//           .name = "vector push grows array size",
//           .desc = "norm_vector_push() adds elemets to the vector",
//           .condition "vec.size == 5 && vec.end_ptr == 5",
//           .status = vec.size == 5 && vec.end_ptr == 5,
//       },
//   };
//
//   for (int i = 0; i < 1; ++i) {
//     NORM__DYN_DS_TEST_RUNNER(cases[i].name, cases[i].desc,
//     cases[i].condition,
//                              cases[i].status);
//   }
// }
//
// #endif

#ifdef __cplusplus
} /* extern "C" */
#endif

// vim: ft=c ts=4 sts=4 sw=4 cin et
