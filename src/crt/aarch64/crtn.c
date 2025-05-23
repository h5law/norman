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

typedef void (*ctr_fn)(void);

ctr_fn _init_array_end[0] __attribute__((used, section(".init_array"),
                                         aligned(sizeof(ctr_fn)))) = {};
ctr_fn _fini_array_end[0] __attribute__((used, section(".fini_array"),
                                         aligned(sizeof(ctr_fn)))) = {};

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
