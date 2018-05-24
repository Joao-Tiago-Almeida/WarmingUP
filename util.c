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


//Aloca memoria, e faz a verificação se o return do realloc não foi NULL
void *checkedRealloc(void *ptr, size_t size) {
    void *mem = realloc(ptr, size);

    if(mem == NULL)
    {
        printf("Out of memory!\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}
/**
* Elimina o carater do parâmetro de entrada 2, se este estiver na penúltima posição da string
*/
void removeLastCharIfMatch(char* str, char c) {
    if(str[strlen(str)-1] == c)
    {
        str[strlen(str)-1] = '\0';
    }
}
