#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "estruturas.h"
#include "lista.h"
#include "menus.h"
#include "ficheiros.h"
#include "conjuntodados.h"
#include "modografico.h"

bool modoTextual(char *nomeFilePaises, char *nomeFileCidades, DADOS *dados);
void display_usage(char *arg);
void ler_argumentos(int argc, char *argv[], bool *modoGrafico, char **nomeFilePaises, char **nomeFileCidades);


#endif
