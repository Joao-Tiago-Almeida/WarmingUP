#include "util.h"

#include <stdio.h>
#include <string.h>

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


void removeLastCharIfMatch(char* str, char c) {
    if(str[strlen(str)-1] == c) {
        str[strlen(str)-1] = '\0';
    }
}