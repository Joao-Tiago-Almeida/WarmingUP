#include "ficheiros.h"
#include "lista.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50
#define BUFFER_SIZE 100
#define MAX_STRING 500

//Obtem o tamanho do ficheiro em bytes
long getFileSize(FILE *file)
{
    long size = 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);

    return size;
}

void read_file_countries(DADOS *dados, char *_nomeFilePaises, char *_nomeFileCidades)
{
    long totalFileSize = 0;

    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE *fileInput = NULL;

    printf("A ler dados dos países...\n");

    dados->headCountriesOriginal = create_list();
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = 0;

    fileInput = fopen(_nomeFilePaises, "r");
    if (fileInput == NULL)
    {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    totalFileSize = getFileSize(fileInput);

    //Vetor de que contem o apontador para o inicio de cada ano na lista
    //TODO
    //int sizeAnoPointers = 2100;
    //list_node_t** anoPointers = malloc(sizeAnoPointers*sizeof(list_node_t*));

    printf("Progresso: 0%%");

    while (fgets(buffer, BUFFER_SIZE, fileInput) != NULL)
    {

        dados_temp *a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^\n]",
                       &a->dt.ano,
                       &a->dt.mes,
                       &a->dt.dia,
                       &a->temp,
                       &a->incerteza,
                       a->pais);

        //TODO meter nome dos paises e cidades com malloc
        //strcpy(a->pais, nome_temp);

        if (check == 6)
        {
            insert_node(dados->headCountriesOriginal, a);

            //caso seja o ano min, determinar o menor mes
            if (a->dt.ano == dados->countriesAnoMin)
            {
                dados->countriesMesMin = a->dt.mes;
            }

            //Determinar ano máx e mín
            if (a->dt.ano < dados->countriesAnoMin)
            {
                dados->countriesAnoMin = a->dt.ano;
                dados->countriesMesMin = a->dt.mes;
            }
            if (a->dt.ano > dados->countriesAnoMax)
            {
                dados->countriesAnoMax = a->dt.ano;
            }

            i++;
        }

        //A cada 100 linhas lidas atualiza o progresso de leitura
        if (i % 100)
        {
            printf("\rProgresso: %ld%%", ftell(fileInput) * 100 / totalFileSize);
        }
    }
    printf("\rProgresso: 100%%\n");

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
    dados->countriesListSize = i;
}

void read_file_cities(DADOS *dados, char *_nomeFilePaises, char *_nomeFileCidades)
{
    long totalFileSize = 0;

    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE * fileInput = NULL;
    char longitude_c;
    char latitude_c;

    printf("A ler dados das cidades...\n");

    dados->headCitiesOriginal = create_list();
    dados->citiesAnoMin = __INT32_MAX__;
    dados->citiesAnoMax = 0;

    fileInput = fopen(_nomeFileCidades, "r");
    if (fileInput == NULL)
    {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    totalFileSize = getFileSize(fileInput);

    //Vetor de que contem o apontador para o inicio de cada ano na lista
    //TODO
    //int sizeAnoPointers = 2100;
    //list_node_t** anoPointers = malloc(sizeAnoPointers*sizeof(list_node_t*));

    printf("Progresso: 0%%");
    while(fgets(buffer, BUFFER_SIZE, fileInput) != NULL){
        if (buffer[strlen(buffer)-1] == '\r')
        {
            buffer[strlen(buffer)-1] = '\0';
        }
        dados_temp* a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^,],%[^,],%f%c,%f%c",
            &a->dt.ano,
            &a->dt.mes,
            &a->dt.dia,
            &a->temp,
            &a->incerteza,
            a->pais,
            a->cidade,
            &a->latitude.angular,
            &latitude_c,
            &a->longitude.angular,
            &longitude_c);

        //TODO meter nome dos paises e cidades com malloc
        //strcpy(a->pais, nome_temp);

        //printf("check :: %d\n", check);

        if (check == 11)
        {
            insert_node(dados->headCitiesOriginal, a);

            //caso seja o ano min, determinar o menor mes
            if (a->dt.ano == dados->citiesAnoMin)
            {
                dados->citiesMesMin = a->dt.mes;
            }

            //Determinar ano máx e mín
            if (a->dt.ano < dados->citiesAnoMin)
            {
                dados->citiesAnoMin = a->dt.ano;
                dados->citiesMesMin = a->dt.mes;
            }
            if (a->dt.ano > dados->citiesAnoMax)
            {
                dados->citiesAnoMax = a->dt.ano;
            }

            a->latitude.direcao = (latitude_c == 'N') ? NORTE : SUL;
            a->longitude.direcao = (longitude_c == 'E') ? ESTE : OESTE;

            i++;
        }
        else
        {
            //printf("%d\t%s\n", i, buffer);
        }

        //A cada 100 linhas lidas atualiza o progresso de leitura
        if (i % 100)
        {
            printf("\rProgresso: %ld%%", ftell(fileInput) * 100 / totalFileSize);
        }
    }
    printf("\rProgresso: 100%%\n");

    //free(anoPointers);
    fclose(fileInput);
    dados->headCitiesFiltrada = dados->headCitiesOriginal;
    dados->citiesListSize = i;
}
