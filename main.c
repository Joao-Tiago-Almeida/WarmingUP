#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estruturas.h"
#include "lista.h"
#include "menus.h"
#include "ficheiros.h"
#include "conjuntodados.h"

void display_usage()
{
    printf("Argumentos invalidos!\n\tExemplo: ./WarmingUp –g -f1 tempcountries.csv -f2 tempcities.csv");
}

void ler_argumentos(int argc, char *argv[], bool *modoGrafico, char **nomeFilePaises, char **nomeFileCidades) {
    //Quando é lido -f1 ou -f2 esta variavel indica que
    // o argumento seguinte é o nome do ficheiro
    bool expectingFileName = false;

    for (int i = 1; i < argc; i++)
    {
        if(expectingFileName) {
            if (strcmp(argv[i-1], "-f1") == 0) {
                *nomeFilePaises = argv[i];
            } else {
                *nomeFilePaises = argv[i];
            }
            expectingFileName = false;
        } else {
            if (strcmp(argv[i], "-g") == 0)
            {
                *modoGrafico = true;
            }
            else if (strcmp(argv[i], "-f1") == 0 || strcmp(argv[i], "-f2") == 0)
            {
                expectingFileName = true;
            }
            else
            {
                display_usage();
                exit(EXIT_FAILURE);
            }
        }
    }

    //Caso o acabe em -fx sem nome do ficheiro ou não seja colocado o nome de um dos ficheiros
    if(expectingFileName || *nomeFilePaises == NULL || *nomeFilePaises == NULL)
    {
        display_usage();
        exit(EXIT_FAILURE);
    }
}

void modoTextual(char *nomeFilePaises, char *nomeFileCidades) {
    DADOS dados;
    dados_init(&dados);

    dados.countriesSize = read_file_countries (&dados.headCountriesOriginal);
    //print_list(headListaDadosOriginal);

    menu_principal(&dados);

    //TODO fazer free das listas, e dos dados correspondentes(payload*)

    printf("END\n");
}

int main(int argc, char *argv[])
{
    bool modoGrafico = false;
    char *nomeFilePaises = NULL, *nomeFileCidades = NULL;
    ler_argumentos(argc, argv, &modoGrafico, &nomeFilePaises, &nomeFileCidades);

    if(modoGrafico) {
        //Inicializar modo grafico
        printf("Modo grafico!\n");
    } else {
        modoTextual(nomeFilePaises, nomeFileCidades);
    }

    return EXIT_SUCCESS;
}
