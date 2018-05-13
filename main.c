#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "estruturas.h"
#include "lista.h"
#include "menus.h"
#include "ficheiros.h"
#include "conjuntodados.h"

#define comando_mal 0

void display_usage(char *arg)
{
    if(arg == comando_mal)
    {
        printf("Código errado!\n\tExemplo: ./WarmingUp –g -f1 tempcountries.csv -f2 tempcities.csv\n\n");
    }
    else
    {
        printf("Argumento %s inválido!\n\tExemplo: ./WarmingUp –g -f1 tempcountries.csv -f2 tempcities.csv\n\n", arg);
    }
}

//TODO esta função tem muitas linhas
//TODO alterado
void ler_argumentos(int argc, char *argv[], bool *modoGrafico, char **nomeFilePaises, char **nomeFileCidades) {
    //Quando é lido -f1 ou -f2 esta variavel indica que
    // o argumento seguinte é o nome do ficheiro
    bool expectingFileName = false;
    bool expectingProgramType = true;

    if ( argc == 6)
    {
        for (int i = 1; i < argc; i++)
        {
            //quando há argumrntos repetidos
            for(int j = 0; j < i; j++)
            {
                if(strcmp(argv[j], argv[i]) == 0){
                    display_usage(argv[i]);
                    exit(EXIT_FAILURE);
                }
            }
            if(expectingFileName) {
                if (strcmp(argv[i-1], "-f1") == 0) {
                    *nomeFilePaises = argv[i];
                } else {
                    *nomeFileCidades = argv[i];
                }
                expectingFileName = false;
            } else {
                //printf("tamanho:: %lu\n", strlen(argv[i]));
                //printf("parametro:: %s\n", argv[i]);
                if (strcmp(argv[i], "-g") == 0)
                {
                    *modoGrafico = true;
                    expectingProgramType = false;
                }
                else if (strcmp(argv[i], "-t") == 0)
                {
                    expectingProgramType = false;
                }
                else if (strcmp(argv[i], "-f1") == 0 || strcmp(argv[i], "-f2") == 0)
                {
                  expectingFileName = true;
                }
            else
            {
                printf("argumento:: %d\n", i+1);
                display_usage(argv[i]);
                exit(EXIT_FAILURE);
            }
            }
        }
    }


    //Caso o acabe em -fx sem nome do ficheiro ou não seja colocado o nome de um dos ficheiros
    if(expectingProgramType || *nomeFilePaises == NULL || *nomeFileCidades == NULL)
    //if( expectingFileName || expectingProgramType || *nomeFilePaises == NULL || *nomeFileCidades == NULL)
    {
        printf("argumento:: %d\n", argc);
        display_usage(comando_mal);
        exit(EXIT_FAILURE);
    }
}

void modoTextual(char *nomeFilePaises, char *nomeFileCidades) {
    DADOS dados;
    dados_init(&dados);

    read_file_countries (&dados, nomeFilePaises, nomeFileCidades);
    read_file_cities (&dados, nomeFilePaises, nomeFileCidades);
    //print_list(headListaDados);

    menu_principal(&dados);

    dados_free(&dados);

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
