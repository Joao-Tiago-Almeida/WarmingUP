#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include "conjuntodados.h"
#include <stdbool.h>

void *checkedMalloc(size_t size);
void *checkedRealloc(void *ptr, size_t size);
void removeLastCharIfMatch(char* str, char c);
int perguntar_menu_choice();
void getstring(char* string, char _comentario[]);
char pergunta_tecla(char* comentario, char opcao1, char opcao2);
bool ModoTextual(char *nomeFilePaises, char *nomeFileCidades, DADOS *dados);
int perguntar_referencia_a_analisar(int _min, int _max, char *_comentario);

#endif
