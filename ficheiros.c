#include "ficheiros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lista.h"
#include "estruturas.h"
#include "util.h"

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


void read_file_countries(DADOS *dados, char *_nomeFilePaises)
{
    long totalFileSize = 0;
    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE *fileInput = NULL;

    //Vetor de que contem o uma lista para cada ano
    int sizeAnoPointers = 2100; //TODO realloc
    list_node_t **insertionSortStartForYear = (list_node_t **) checkedMalloc(sizeAnoPointers * sizeof(list_node_t*));

    printf("A ler dados dos países...\n");

    dados->headCountriesOriginal = create_list();
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = 0;

    //Inicializa o vetor para todos os anos terem a sua lista
    for(int i = 0; i<sizeAnoPointers; i++) {
        insertionSortStartForYear[i] = create_list();
    }

    fileInput = fopen(_nomeFilePaises, "r");
    if (fileInput == NULL)
    {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    totalFileSize = getFileSize(fileInput);

    printf("Progresso: 0%%");

    while (fgets(buffer, BUFFER_SIZE, fileInput) != NULL)
    {
        removeLastCharIfMatch(buffer, '\n');
        removeLastCharIfMatch(buffer, '\r');
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
            list_node_t *new_node = create_node(a);
            list_node_t *remover_TODO_tail = NULL;
            sortedInsert(insertionSortStartForYear[a->dt.ano], &remover_TODO_tail, new_node);

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


    dados->headCountriesOriginal->next = insertionSortStartForYear[0]->next;
    free(insertionSortStartForYear[0]); //Free da dummy node da lista para o ano 0
    //TODO tail
    for(int i = 1; i<sizeAnoPointers; i++) {
        //Vai achar a tail da lista original
        list_node_t *tailOrig = dados->headCountriesOriginal;
        while(tailOrig->next != NULL)
        {
            tailOrig = tailOrig->next;
        }

        tailOrig->next = insertionSortStartForYear[i]->next;
        free(insertionSortStartForYear[i]); //Free da dummy node da lista para este ano
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
    //TODO free a cada lista 
    free(insertionSortStartForYear);
    fclose(fileInput);
    dados->headCountriesFiltrada = dados->headCountriesOriginal;
    dados->countriesListSize = i;
}

//TODO fazer com que a funcao de ler paises e cidades sejam só uma com um parametro bool 
// pq são quase iguais
/*void read_file_countries(DADOS *dados, char *_nomeFilePaises)
{
    long totalFileSize = 0;
    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE *fileInput = NULL;
    clock_t timeCounter = clock();

    //Vetor de que contem o uma lista para cada ano
    int sizeAnoPointers = 2100; //TODO realloc
    list_node_t **yearsListHead = (list_node_t **) checkedMalloc(sizeAnoPointers * sizeof(list_node_t*));
    list_node_t **yearsListTail = (list_node_t **) checkedMalloc(sizeAnoPointers * sizeof(list_node_t*));

    printf("A ler dados dos países...\n");

    dados->headCountriesOriginal = create_list();
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = -__INT32_MAX__;

    //Inicializa o vetor para todos os anos terem a sua lista
    for(int i = 0; i<sizeAnoPointers; i++) {
        yearsListHead[i] = create_list();
        yearsListTail[i] = NULL;
    }

    fileInput = fopen(_nomeFilePaises, "r");
    if (fileInput == NULL)
    {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    totalFileSize = getFileSize(fileInput);

    printf("Progresso: 0%%");
    while (fgets(buffer, BUFFER_SIZE, fileInput) != NULL)
    {
        removeLastCharIfMatch(buffer, '\n');
        removeLastCharIfMatch(buffer, '\r');

        dados_temp *a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^\n]",
                       &a->dt.ano,
                       &a->dt.mes,
                       &a->dt.dia,
                       &a->temp,
                       &a->incerteza,
                       a->pais);
        //TODO apenas quando não lê a incerteza devia funcionar
        if (check == 6 )
        {
            list_node_t *new_node = create_node(a);
            sortedInsert(yearsListHead[a->dt.ano], &yearsListTail[a->dt.ano], new_node);

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

//TODO ver melhor, joao almeida
    dados->headCountriesOriginal->next = yearsListHead[0]->next;
    list_node_t *headOriginalTail = dados->headCountriesOriginal->next;
    free(yearsListHead[0]); //Free da dummy node da lista para o ano 0

    for(int i = 1; i<sizeAnoPointers; i++) {
        if(dados->headCountriesOriginal->next == NULL &&
            yearsListHead[i]->next != NULL)
        {
            //Caso a lista original ainda esteja vazia, meter o head->next e o tail
            dados->headCountriesOriginal->next = yearsListHead[i]->next;
            headOriginalTail = yearsListTail[i];
        }

        if(headOriginalTail != NULL) {
            //Meter a tail atual da original a apontar para a head->next da lista deste ano
            headOriginalTail->next = yearsListHead[i]->next;
            if(yearsListHead[i]->next != NULL) {
                yearsListHead[i]->next->prev = headOriginalTail;
                headOriginalTail = yearsListTail[i]; //Se yearsListHead[i]->next != NULL também é yearsListTail[i]
            }
        }

        free(yearsListHead[i]); //Free da dummy node da lista para este ano
    }

    timeCounter = clock() - timeCounter;
    printf("\rProgresso: 100%% (%ld s)\n", timeCounter/CLOCKS_PER_SEC);

    free(yearsListHead);
    free(yearsListTail);

    fclose(fileInput);
    dados->headCountriesFiltrada = dados->headCountriesOriginal;
    dados->countriesListSize = i;
}*/

void read_file_cities(DADOS *dados, char *_nomeFileCidades)
{
    long totalFileSize = 0;
    int i = 0, check = -1;
    char buffer[BUFFER_SIZE];
    FILE * fileInput = NULL;
    char longitude_c;
    char latitude_c;
    clock_t timeCounter = clock();

    //Vetor de que contem o uma lista para cada ano
    int sizeAnoPointers = 2100; //TODO realloc
    list_node_t **yearsListHead = (list_node_t **) checkedMalloc(sizeAnoPointers * sizeof(list_node_t*));
    list_node_t **yearsListTail = (list_node_t **) checkedMalloc(sizeAnoPointers * sizeof(list_node_t*));

    printf("A ler dados das cidades...\n");

    dados->headCitiesOriginal = create_list();
    dados->citiesAnoMin = __INT32_MAX__;
    dados->citiesAnoMax = -__INT32_MAX__;
    dados->citiesTempMin = __INT32_MAX__;
    dados->citiesTempMax = -__INT32_MAX__;

    //Inicializa o vetor para todos os anos terem a sua lista
    for(int i = 0; i<sizeAnoPointers; i++) {
        yearsListHead[i] = create_list();
        yearsListTail[i] = NULL;
    }

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
    while(fgets(buffer, BUFFER_SIZE, fileInput) != NULL)
    {
        removeLastCharIfMatch(buffer, '\r');
        removeLastCharIfMatch(buffer, '\n');

        dados_temp* a = malloc(sizeof(dados_temp));
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^,],%[^,],%f%c,%f%c",
            &a->dt.ano,
            &a->dt.mes,
            &a->dt.dia,
            &a->temp,
            &a->incerteza,
            a->cidade,
            a->pais,
            &a->latitude.angular,
            &latitude_c,
            &a->longitude.angular,
            &longitude_c);

        //TODO meter nome dos paises e cidades com malloc

        if (check == 11)
        {
            list_node_t *new_node = create_node(a);
            sortedInsert(yearsListHead[a->dt.ano], &yearsListTail[a->dt.ano], new_node);

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
            if (a->temp < dados->citiesTempMin)
            {
                dados->citiesTempMin = a->temp;
            }
            if (a->temp > dados->citiesTempMax)
            {
                dados->citiesTempMax = a->temp;
            }

            a->latitude.direcao = (latitude_c == 'N') ? NORTE : SUL;
            a->longitude.direcao = (longitude_c == 'E') ? ESTE : OESTE;

            i++;
        }
        //TODO
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

    dados->headCitiesOriginal->next = yearsListHead[0]->next;
    list_node_t *headOriginalTail = dados->headCitiesOriginal->next;
    free(yearsListHead[0]); //Free da dummy node da lista para o ano 0

    for(int i = 1; i<sizeAnoPointers; i++) {
        if(dados->headCitiesOriginal->next == NULL &&
            yearsListHead[i]->next != NULL)
        {
            //Caso a lista original ainda esteja vazia, meter o head->next e o tail
            dados->headCitiesOriginal->next = yearsListHead[i]->next;
            headOriginalTail = yearsListTail[i];
        }

        if(headOriginalTail != NULL) {
            //Meter a tail atual da original a apontar para a head->next da lista deste ano
            headOriginalTail->next = yearsListHead[i]->next;
            if(yearsListHead[i]->next != NULL) {
                yearsListHead[i]->next->prev = headOriginalTail;
                headOriginalTail = yearsListTail[i]; //Se yearsListHead[i]->next != NULL também é yearsListTail[i]
            }
        }

        free(yearsListHead[i]); //Free da dummy node da lista para este ano
    }


    timeCounter = clock() - timeCounter;
    printf("\rProgresso: 100%% (%ld s)\n", timeCounter/CLOCKS_PER_SEC);

    free(yearsListHead);
    free(yearsListTail);

    fclose(fileInput);
    dados->headCitiesFiltrada = dados->headCitiesOriginal;
    dados->citiesListSize = i;
}
