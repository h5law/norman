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
#include <stdio.h>
#include <stdlib.h>

#define NORM_DYN_DS_IMPLEMENTATION 1
#include "../norm_dyn.h"
#include "xxhash.h"

size_t hasher_fn(const char *key, size_t key_length)
{
    size_t hash;
    XXH32_hash_t h = XXH32(key, key_length, 0);
    hash = (size_t)h;
    return hash;
}

int main(int argc, char *argv[])
{
    norm_vector_t zero, vec = {0};

    vec = norm_vector_init(10, 0.7, 8);
    assert(memcmp(&vec, &zero, sizeof(norm_vector_t)) != 0);
    assert((vec.size == 0 && vec.end_ptr == 0) && vec.capacity == 10);

    for (uint64_t i = 0; i < 6; ++i) {
        uint64_t j = i + 1;
        int res = norm_vector_push(&vec, &j, sizeof(uint64_t));
        assert(res == NORM_DYN_ERR_OKAY);
    }

    assert((vec.size == 6 && vec.end_ptr == 6) && vec.capacity == 10);
    uint64_t i = 7;
    int res = norm_vector_push(&vec, &i, sizeof(uint64_t));
    assert(res == NORM_DYN_ERR_OKAY);
    assert((vec.size == 7 && vec.end_ptr == 7) && vec.capacity == 20);

    res = norm_vector_spos(&vec, 0, &i, sizeof(uint64_t));
    assert(res == NORM_DYN_ERR_OKAY);
    uint64_t *ret = (uint64_t *)norm_vector_gpos(&vec, 0, sizeof(uint64_t));
    assert(*ret == i);

    // // 1 .. 2 .. 4 .. 8 [ERR] <break>
    for (i = 1; i < vec.capacity; i <<= 1) {
        res = norm_vector_zpos(&vec, i, sizeof(uint64_t));
        if (i < 8)
            assert(res == NORM_DYN_ERR_OKAY);
        else {
            assert(res == NORM_DYN_ERR_IARGS);
            break;
        }
        ret = norm_vector_gpos(&vec, i, sizeof(uint64_t));
        assert(*ret == 0);
    }

    assert(vec.end_ptr == 7 && vec.size == 4);
    res = norm_vector_compact(&vec, sizeof(uint64_t));
    assert(res == NORM_DYN_ERR_OKAY);
    assert(vec.end_ptr == 4 && vec.size == 4);

    norm_vector_t vec2 = {0};
    vec2 = norm_vector_clone(&vec, sizeof(uint64_t));
    assert(vec.size == vec2.size && vec.capacity == vec2.capacity &&
           vec.end_ptr == vec2.end_ptr && vec.load_factor == vec2.load_factor);
    assert(memcmp(((char *)(vec.array)), ((char *)(vec2.array)),
                  sizeof(uint64_t) * vec.capacity) == 0);

    norm_vector_t vec3 = norm_vector_clone(&vec, sizeof(uint64_t));
    assert(memcmp(((char *)(vec.array)), ((char *)(vec3.array)),
                  sizeof(uint64_t) * vec.capacity) == 0);
    res = norm_vector_empty(&vec3, sizeof(uint64_t));
    assert(res == NORM_DYN_ERR_OKAY);
    assert(memzcmp((char *)(vec3.array), sizeof(uint64_t) * vec3.capacity));
    assert(vec3.size == 0 && vec3.capacity == 20 &&
           vec3.load_factor == vec.load_factor && vec3.end_ptr == 0);

    vec3 = norm_vector_concat(&vec, &vec2, sizeof(uint64_t));
    assert(vec3.capacity == (vec.capacity + vec2.capacity));
    assert(vec3.size == (vec.size + vec2.size));
    for (int j = 0; j < vec3.capacity; ++j) {
        uint64_t *u, *t;
        u = norm_vector_gpos(&vec3, j, sizeof(uint64_t));
        if (j >= vec.capacity) {
            t = norm_vector_gpos(&vec2, j - vec.capacity, sizeof(uint64_t));
        } else {
            t = norm_vector_gpos(&vec, j, sizeof(uint64_t));
        }
        assert(*u == *t);
    }

    assert(norm_vector_free(&vec) == NORM_DYN_ERR_OKAY);
    assert(norm_vector_free(&vec2) == NORM_DYN_ERR_OKAY);
    assert(norm_vector_free(&vec3) == NORM_DYN_ERR_OKAY);

    norm_map_t map = norm_map_init(20, 0.7, sizeof(uint64_t), hasher_fn,
                                   norm_map_linear_probe);
    uint64_t p = 20;
    assert(norm_map_set(&map, "test1", &p, 5, sizeof(uint64_t)) ==
           NORM_DYN_ERR_OKAY);
    uint64_t *nret = norm_map_get(&map, "test1", 5, sizeof(uint64_t));
    assert(*nret == 20);
    p = 30;
    assert(norm_map_set(&map, "test2", &p, 5, sizeof(uint64_t)) ==
           NORM_DYN_ERR_OKAY);
    nret = norm_map_get(&map, "test2", 5, sizeof(uint64_t));
    assert(*nret == 30);
    p = 40;
    assert(norm_map_set(&map, "test3", &p, 5, sizeof(uint64_t)) ==
           NORM_DYN_ERR_OKAY);
    nret = norm_map_get(&map, "test3", 5, sizeof(uint64_t));
    assert(*nret == 40);

    assert(norm_map_delete(&map, "test1", 5, sizeof(uint64_t)) ==
           NORM_DYN_ERR_OKAY);
    nret = norm_map_get(&map, "test1", 5, sizeof(uint64_t));
    assert(nret == NULL);

    assert(norm_map_clear(&map, sizeof(uint64_t)) == NORM_DYN_ERR_OKAY);
    for (int i = 0; i < map.table.capacity; ++i) {
        assert(memzcmp(((char *)(map.table.array)) +
                               (i * (sizeof(norm_map_entry_t) +
                                     NORM_MAP_MAX_KEY_LEN + sizeof(uint64_t))),
                       sizeof(norm_map_entry_t) + NORM_MAP_MAX_KEY_LEN +
                               sizeof(uint64_t)));
    }

    uint64_t *aaaa;
    uint64_t v = 1;
    for (int i = 0; i < 32; ++i) {
        char k[32] = {0};
        int len = sprintf(k, "testkey%d%d%d", i, i, i);
        assert(norm_map_set(&map, k, &v, len, sizeof(uint64_t)) ==
               NORM_DYN_ERR_OKAY);
        aaaa = norm_map_get(&map, k, len, sizeof(uint64_t));
        assert(*aaaa == v);
        ++v;
    }
    assert(map.table.capacity == 64);

    // for (int i = 0; i < map.table.capacity; ++i) {
    //     norm_map_entry_t *entry = norm_vector_gpos(
    //             &(map.table), i, norm__map_entry_size(sizeof(uint64_t)));
    //     if (!entry || memzcmp(entry, norm__map_entry_size(sizeof(uint64_t))))
    //     {
    //         printf("%d -> (null)\n", i);
    //         continue;
    //     }
    //     char k[NORM_MAP_MAX_KEY_LEN] = {0};
    //     snprintf(k, NORM_MAP_MAX_KEY_LEN, "%s", (char *)entry->kv);
    //     uint64_t *val = NULL;
    //     val = (void *)(entry->kv + NORM_MAP_MAX_KEY_LEN);
    //     printf("%d -> %s : %llu\n", i, k, *val);
    // }

    assert(norm_map_free(&map) == NORM_DYN_ERR_OKAY);
}

// vim: ft=c ts=4 sts=4 sw=4 cin et
