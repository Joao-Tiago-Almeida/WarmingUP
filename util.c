#include "util.h"

#include <stdio.h>

//Aloca memoria, e faz a verificação se o return do malloc não foi NULL
void *checkedMalloc(size_t size) {
    void *mem = malloc(size);

    if(mem == NULL)
    {
        printf("Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return mem;
}