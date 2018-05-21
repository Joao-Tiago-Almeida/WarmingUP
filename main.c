#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "data.h"

#define comando_mal 0

int main(int argc, char *argv[])
{
    bool modo_grafico = false;
    char *nomeFilePaises = NULL, *nomeFileCidades = NULL;
    ler_argumentos(argc, argv, &modo_grafico, &nomeFilePaises, &nomeFileCidades);

    DADOS dados;
    dados_init(&dados);
    bool mudaDeModo = true;
    do
    {
        if(modo_grafico) {
            //Inicializar modo grafico
            printf("Modo grafico!\n");
            mudaDeModo = modoGrafico(nomeFilePaises, nomeFileCidades, &dados);
        } else {
            mudaDeModo = modoTextual(nomeFilePaises, nomeFileCidades, &dados);
        }
        modo_grafico = !modo_grafico; // Se tiver no modo grafico muda para o modo textual e vice versa
    } while (mudaDeModo); //A variavel mudaDeModo é true se for para continuar no programa, e muda de modo


    dados_free(&dados);
    printf("END OF THE PROGRAM!!\n");
    return EXIT_SUCCESS;
}

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

// Devolve false se for para sair do programa ou true se for para mudar para o modo textual
bool modoTextual(char *nomeFilePaises, char *nomeFileCidades, DADOS *dados )
{
    if(dados->headCitiesOriginal == NULL)  read_file_cities (dados,nomeFileCidades);
    if(dados->headCountriesOriginal == NULL)  read_file_countries (dados, nomeFilePaises);

    //print_list(dados->headCountriesOriginal);
    //print_list(dados->headCitiesOriginal);

    return menu_principal(dados);
}
