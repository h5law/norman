/*
Copyright (c) 2025 Harry Law <h5law>

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

#include <stdio.h>
#include <stdlib.h>

#include "../norm_dyn.h"
#include "../norm_utils.h"
#include "xxhash.h"

size_t hasher_fn(const char *key, size_t key_length)
{
    size_t hash;
    XXH32_hash_t h = XXH32(key, key_length, 0);
    hash = ( size_t )h;
    return hash;
}

int main(int argc, char *argv[])
{
    norm_vector_t zero, vec = {0};

    vec = norm_vector_init(10, 0.7, 8);
    assertf(memcmp(&vec, &zero, sizeof(norm_vector_t)) != 0, "Vector shit");
    assertf((vec.size == 0 && vec.end_ptr == 0) && vec.capacity == 10,
            "Vector shit");

    for (uint64_t i = 0; i < 6; ++i) {
        uint64_t j = i + 1;
        int res = norm_vector_push(&vec, &j, sizeof(uint64_t));
        assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
    }

    assertf((vec.size == 6 && vec.end_ptr == 6) && vec.capacity == 10,
            "Vector shit");
    uint64_t i = 7;
    int res = norm_vector_push(&vec, &i, sizeof(uint64_t));
    assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
    assertf((vec.size == 7 && vec.end_ptr == 7) && vec.capacity == 20,
            "Vector shit");

    res = norm_vector_spos(&vec, 0, &i, sizeof(uint64_t));
    assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
    uint64_t *ret = ( uint64_t * )norm_vector_gpos(&vec, 0, sizeof(uint64_t));
    assertf(*ret == i, "Vector shit");

    // // 1 .. 2 .. 4 .. 8 [ERR] <break>
    for (i = 1; i < vec.capacity; i <<= 1) {
        res = norm_vector_zpos(&vec, i, sizeof(uint64_t));
        if (i < 8)
            assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
        else {
            assertf(res == NORM_DYN_ERR_IARGS, "Vector shit");
            break;
        }
        ret = norm_vector_gpos(&vec, i, sizeof(uint64_t));
        assertf(*ret == 0, "Vector shit");
    }

    assertf(vec.end_ptr == 7 && vec.size == 4, "Vector shit");
    res = norm_vector_compact(&vec, sizeof(uint64_t));
    assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
    assertf(vec.end_ptr == 4 && vec.size == 4, "Vector shit");

    norm_vector_t vec2 = {0};
    vec2 = norm_vector_clone(&vec, sizeof(uint64_t));
    assertf(vec.size == vec2.size && vec.capacity == vec2.capacity &&
                    vec.end_ptr == vec2.end_ptr &&
                    vec.load_factor == vec2.load_factor,
            "Vector shit");
    assertf(memcmp((( char * )(vec.array)), (( char * )(vec2.array)),
                   sizeof(uint64_t) * vec.capacity) == 0,
            "Vector shit");

    norm_vector_t vec3 = norm_vector_clone(&vec, sizeof(uint64_t));
    assertf(memcmp((( char * )(vec.array)), (( char * )(vec3.array)),
                   sizeof(uint64_t) * vec.capacity) == 0,
            "Vector shit");
    res = norm_vector_empty(&vec3, sizeof(uint64_t));
    assertf(res == NORM_DYN_ERR_OKAY, "Vector shit");
    assertf(memzcmp(( char * )(vec3.array), sizeof(uint64_t) * vec3.capacity),
            "Vector shit");
    assertf(vec3.size == 0 && vec3.capacity == 20 &&
                    vec3.load_factor == vec.load_factor && vec3.end_ptr == 0,
            "Vector shit");

    vec3 = norm_vector_concat(&vec, &vec2, sizeof(uint64_t));
    assertf(vec3.capacity == (vec.capacity + vec2.capacity), "Vector shit");
    assertf(vec3.size == (vec.size + vec2.size), "Vector shit");
    for (int j = 0; j < vec3.capacity; ++j) {
        uint64_t *u, *t;
        u = norm_vector_gpos(&vec3, j, sizeof(uint64_t));
        if (j >= vec.capacity) {
            t = norm_vector_gpos(&vec2, j - vec.capacity, sizeof(uint64_t));
        } else {
            t = norm_vector_gpos(&vec, j, sizeof(uint64_t));
        }
        assertf(*u == *t, "Vector shit");
    }

    assertf(norm_vector_free(&vec) == NORM_DYN_ERR_OKAY, "Freeing vector");
    assertf(norm_vector_free(&vec2) == NORM_DYN_ERR_OKAY, "Freeing vector");
    assertf(norm_vector_free(&vec3) == NORM_DYN_ERR_OKAY, "Freeing vector");

    norm_map_t map = norm_map_init(20, 0.7, sizeof(uint64_t), hasher_fn,
                                   norm_map_linear_probe);
    uint64_t p = 20;
    assertf(norm_map_set(&map, "test1", &p, 5, sizeof(uint64_t)) ==
                    NORM_DYN_ERR_OKAY,
            "Setting key");
    uint64_t *nret = norm_map_get(&map, "test1", 5, sizeof(uint64_t));
    assertf(*nret == 20, "Retrieved key successfully");
    p = 30;
    assertf(norm_map_set(&map, "test2", &p, 5, sizeof(uint64_t)) ==
                    NORM_DYN_ERR_OKAY,
            "Setting key");
    nret = norm_map_get(&map, "test2", 5, sizeof(uint64_t));
    assertf(*nret == 30, "Retrieved key successfully");
    p = 40;
    assertf(norm_map_set(&map, "test3", &p, 5, sizeof(uint64_t)) ==
                    NORM_DYN_ERR_OKAY,
            "Setting key");
    nret = norm_map_get(&map, "test3", 5, sizeof(uint64_t));
    assertf(*nret == 40, "Retrieved key successfully");

    assertf(norm_map_delete(&map, "test1", 5, sizeof(uint64_t)) ==
                    NORM_DYN_ERR_OKAY,
            "Deleting key");
    nret = norm_map_get(&map, "test1", 5, sizeof(uint64_t));
    assertf(nret == NULL, "Key not found");

    assertf(norm_map_clear(&map, sizeof(uint64_t)) == NORM_DYN_ERR_OKAY,
            "Emptying map");
    for (int i = 0; i < map.table.capacity; ++i) {
        assertf(memzcmp((( char * )(map.table.array)) +
                                (i * (sizeof(norm_map_entry_t) +
                                      NORM_MAP_MAX_KEY_LEN + sizeof(uint64_t))),
                        sizeof(norm_map_entry_t) + NORM_MAP_MAX_KEY_LEN +
                                sizeof(uint64_t)),
                "Slot is empty");
    }

    uint64_t *aaaa;
    uint64_t v = 1;
    for (int i = 0; i < 32; ++i) {
        char k[32] = {0};
        int len = sprintf(k, "testkey%d%d%d", i, i, i);
        assertf(norm_map_set(&map, k, &v, len, sizeof(uint64_t)) ==
                        NORM_DYN_ERR_OKAY,
                "Setting value");
        aaaa = norm_map_get(&map, k, len, sizeof(uint64_t));
        assertf(*aaaa == v, "Retrieved correct value");
        ++v;
    }
    assertf(map.table.capacity == 64, "Map cap == 64");

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

    assertf(norm_map_free(&map) == NORM_DYN_ERR_OKAY, "Freeing map");
}

// vim: ft=c ts=4 sts=4 sw=4 cin et
