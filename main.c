#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
=======
#include <stdbool.h>
>>>>>>> miguel

#include "estruturas.h"
#include "lista.h"
#include "menus.h"
<<<<<<< HEAD
#include "ficheiros.h"
=======

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
    int ano_a_analisar = -1, periodo_a_analisar = -1;
    list_node_t *headListaDados = NULL;

    headListaDados = create_list();

    //Teste de como criar elementos na lista
    for (int i = 0; i < 2; i++)
    {
        dados_temp a;
        a.dt.ano = 1;
        a.dt.mes = 1;
        a.dt.dia = 1;
        a.temp = 10.0f;
        a.incerteza = 1.0f;
        strcpy(a.pais, "Portugal");
        strcpy(a.cidade, "Santarem");
        a.latitude.angular = 1.0f;
        a.latitude.direcao = NORTE;
        
        insert_node(headListaDados, &a);
    }
    print_list(headListaDados);
>>>>>>> miguel

    menu_principal(&periodo_a_analisar, &ano_a_analisar);

    printf("END\n");
}

int main(int argc, char *argv[])
{
<<<<<<< HEAD
    int ano_a_analisar = -1, periodo_a_analisar = -1;
    int size_countries_file = 0;
    list_node_t* headListaDados = NULL;

    size_countries_file = read_file_countries (&headListaDados);
    print_list(headListaDados);

    headListaDados = insertionSort(&headListaDados);

    //print_list(headListaDados);




    menu_principal(&periodo_a_analisar, &ano_a_analisar);

=======
    bool modoGrafico = false;
    char *nomeFilePaises = NULL, *nomeFileCidades = NULL;
    ler_argumentos(argc, argv, &modoGrafico, &nomeFilePaises, &nomeFileCidades);

    if(modoGrafico) {
        //Inicializar modo grafico
        printf("Modo grafico!\n");
    } else {
        modoTextual(nomeFilePaises, nomeFileCidades);
    }
>>>>>>> miguel

    return EXIT_SUCCESS;
}
