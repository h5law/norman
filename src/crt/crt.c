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

#include "../stdio.h"
#include "../system.h"

int    envc = 0;
char **envp;

extern void _init(void);
extern void _fini(void);

FILE *stdin, *stdout, *stderr;

int __main(int argc, char **argv)
{
    envp = &(argv[argc + 1]);

    // Initialize stdin, stdout, stderr
    stdin  = fdopen(STDIN_FILENO, "r");
    stdout = fdopen(STDOUT_FILENO, "w");
    stderr = fdopen(STDERR_FILENO, "w");

    // Set up default signal handlers
    // signal(SIGINT, SIG_DFL);
    // signal(SIGSEGV, SIG_DFL);
    // signal(SIGTERM, SIG_DFL);

    // Initialize heap (normally handled by malloc implementation)
    // This is just a placeholder - most C libraries handle this internally

    return main(argc, argv);
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
