#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>

void *checkedMalloc(size_t size);
void *checkedRealloc(void *ptr, size_t size);
void removeLastCharIfMatch(char* str, char c);
#endif
