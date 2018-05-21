#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "lista.h"
#include "conjuntodados.h"

#include <stdio.h>

FILE *checkedFopen(char* name, char* mode);
void mergeSort(list_node_t* list, int listSize);
void read_file_countries (DADOS* dados, char[]);
void read_file_cities (DADOS* dados, char[]);
dados_temp *readFileLineToStruct(char* line, bool cidades);
int readFileToList(char* fileName, DADOS* dados, bool cidades);

#endif
