#include "ficheiros.h"
#include "lista.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50
#define BUFFER_SIZE 100
#define MAX_STRING 500

void read_file_countries(DADOS* dados, char *_nomeFilePaises, char *_nomeFileCidades)
{
    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE * fileInput = NULL;

    dados->headCountriesOriginal = create_list();
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = 0;

    fileInput = fopen(_nomeFilePaises, "r");
    if (fileInput == NULL) {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    //Vetor de que contem o apontador para o inicio de cada ano na lista
    //TODO
    //int sizeAnoPointers = 2100;
    //list_node_t** anoPointers = malloc(sizeAnoPointers*sizeof(list_node_t*));

    while(fgets(buffer, BUFFER_SIZE, fileInput) != NULL){

        dados_temp* a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^\n]",
            &a->dt.ano,
            &a->dt.mes,
            &a->dt.dia,
            &a->temp,
            &a->incerteza,
            a->pais);

        //TODO meter nome dos paises e cidades com malloc
        //strcpy(a->pais, nome_temp);

        if ( check == 6 )
        {
            insert_node(dados->headCountriesOriginal, a);

            //caso seja o ano min, determinar o menor mes
            if(a->dt.ano == dados->countriesAnoMin) {
                dados->countriesMesMin = a->dt.mes;
            }

            //Determinar ano máx e mín
            if(a->dt.ano < dados->countriesAnoMin) {
                dados->countriesAnoMin = a->dt.ano;
                dados->countriesMesMin = a->dt.mes;
            }
            if(a->dt.ano > dados->countriesAnoMax) {
                dados->countriesAnoMax = a->dt.ano;
            }


            //TODO remover vv
            printf("%d\t", i);
            i++;
        }
    }
    printf("\nMax: %d, min: %d\n", dados->countriesAnoMax, dados->countriesAnoMin);

    //Teste ao sort
    /*for(int i = 0 ; i< 50 ; i++)
    {
        dados_temp a;
        a.dt.ano = rand()%20 +1;
        a.dt.mes = rand()%6 +7;
            list_node_t* node = create_node(&a);
            sortedInsert(*_head, node);
            printf("%d\t", i);
    }*/

    //free(anoPointers);
    fclose(fileInput);
    dados->headCountriesFiltrada = dados->headCountriesOriginal;
}

void read_file_cities(DADOS* dados, char *_nomeFilePaises, char *_nomeFileCidades)
{
    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE * fileInput = NULL;
    char longitude_c[2];
    char latitude_c[2];

    dados->headCitiesOriginal = create_list();
    dados->citiesAnoMin = __INT32_MAX__;
    dados->citiesAnoMax = 0;

    fileInput = fopen(_nomeFileCidades, "r");
    if (fileInput == NULL) {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    //Vetor de que contem o apontador para o inicio de cada ano na lista
    //TODO
    //int sizeAnoPointers = 2100;
    //list_node_t** anoPointers = malloc(sizeAnoPointers*sizeof(list_node_t*));

    while(fgets(buffer, BUFFER_SIZE, fileInput) != NULL){

        dados_temp* a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^,],%[^,],%f%[^,],%f%[^\n]",
            &a->dt.ano,
            &a->dt.mes,
            &a->dt.dia,
            &a->temp,
            &a->incerteza,
            a->pais,
            a->cidade,
            &a->latitude.angular,
            latitude_c,
            &a->longitude.angular,
            longitude_c);

        //TODO meter nome dos paises e cidades com malloc
        //strcpy(a->pais, nome_temp);

        printf("check :: %d\n", check);

        if ( check == 11 )
        {
            insert_node(dados->headCitiesOriginal, a);

            //caso seja o ano min, determinar o menor mes
            if(a->dt.ano == dados->citiesAnoMin) {
                dados->citiesMesMin = a->dt.mes;
            }

            //Determinar ano máx e mín
            if(a->dt.ano < dados->citiesAnoMin) {
                dados->citiesAnoMin = a->dt.ano;
                dados->citiesMesMin = a->dt.mes;
            }
            if(a->dt.ano > dados->citiesAnoMax) {
                dados->citiesAnoMax = a->dt.ano;
            }

            if(strcmp(latitude_c,"N"))
            {
                a->latitude.direcao = 0;
            }
            else
            {
                a->latitude.direcao = 1;
            }
            if(strcmp(longitude_c,"E"))
            {
                a->longitude.direcao = 2;
            }
            else
            {
                a->longitude.direcao = 3;
            }
            //TODO remover vv
            i++;
            printf("%d\t", i);
            printf("%s\n", buffer);
            printf("%s\n", a->cidade);
            printf("latitude:: %d\n", a->latitude.direcao);
            printf("longitude:: %d\n", a->longitude.direcao);
        }
    }
    printf("\nMax: %d, min: %d\n", dados->citiesAnoMax, dados->citiesAnoMin);

    //Teste ao sort
    /*for(int i = 0 ; i< 50 ; i++)
    {
        dados_temp a;
        a.dt.ano = rand()%20 +1;
        a.dt.mes = rand()%6 +7;
            list_node_t* node = create_node(&a);
            sortedInsert(*_head, node);
            printf("%d\t", i);
    }*/

    printf("ola2\n");
    //free(anoPointers);
    fclose(fileInput);
    printf("ola1\n");
    dados->headCitiesFiltrada = dados->headCitiesOriginal;
}
