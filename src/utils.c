#include <stdio.h>
#include <string.h>

#include "../include/utils.h"

int memvcmp(void *memory, unsigned char val, size_t size)
{
    unsigned char *mm = ( unsigned char * )memory;
    return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

void assertf(int eval, char *desc)
{
    char *symbol = "❌";
    if (eval == 1)
        symbol = "✅";
    printf("%s [%d]\t%s %s\n", __FILE__, __LINE__, symbol, desc);
}

// vim: ft=c ts=4 sts=4 sw=4 cin et nospell
