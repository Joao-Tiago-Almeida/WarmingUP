#include "ficheiros.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lista.h"
#include "estruturas.h"
#include "util.h"

#define STRING_SIZE 50
#define BUFFER_SIZE 100
#define MAX_STRING 500

//Obtem o tamanho do ficheiro em bytes (para fazer o progresso)
long getFileSize(FILE *file)
{
    long size = 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);

    return size;
}

//Abre um ficheiro e faz a verificação se a abertura foi feita com sucesso
FILE *checkedFopen(char* name, char* mode) {
    FILE *f = fopen(name, mode);
    if (f == NULL)
    {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }
    return f;
}

/**
 *  Devolve true se a data a for menor que a data b
**/
bool dataMenorQue(data* a, data* b) {
    return (a->ano < b->ano
        || (a->ano == b->ano && a->mes < b->mes)
        || (a->ano == b->ano && a->mes == b->mes && a->dia < b->dia));
}

/**
 * Lê o ficheiro dos paises e coloca a lista na estrutura dados
**/
void read_file_countries(DADOS *dados, char *_nomeFilePaises)
{
    clock_t timeCounter = clock();

    dados->headCountriesOriginal = create_list();
    dados->headCountriesFiltrada = dados->headCountriesOriginal;

    printf("A ler dados dos países...\n");

    dados->countriesListSize = readFileToList(_nomeFilePaises, dados, false);

    printf("\rProgresso: 100%% (A ordenar...)");
    fflush(stdout);
    mergeSort(dados->headCountriesOriginal, dados->countriesListSize);

    timeCounter = clock() - timeCounter;
    printf("\rProgresso: 100%% (%ld s)                 \n", timeCounter/CLOCKS_PER_SEC);
}

/**
 * Lê o ficheiro dos paises e coloca a lista na estrutura dados
**/
void read_file_cities(DADOS *dados, char *_nomeFileCidades)
{
    clock_t timeCounter = clock();

    dados->headCitiesOriginal = create_list();
    dados->headCitiesFiltrada = dados->headCitiesOriginal;

    printf("A ler dados das cidades...\n");

    dados->citiesListSize = readFileToList(_nomeFileCidades, dados, true);

    printf("\rProgresso: 100%% (A ordenar...)");
    fflush(stdout);
    mergeSort(dados->headCitiesOriginal, dados->citiesListSize);

    timeCounter = clock() - timeCounter;
    printf("\rProgresso: 100%% (%ld s)                 \n", timeCounter/CLOCKS_PER_SEC);
}

/**
 * Ao ler uma linha do ficheiro dos paises atualiza os valores dos maximos e minimos
**/
void readFileCountriesAtualizaMaxMin(DADOS* dados, dados_temp* novoValor) {
    if (novoValor->dt.ano == dados->countriesAnoMin &&
        novoValor->dt.mes < dados->countriesMesMin)
    {
        dados->countriesMesMin = novoValor->dt.mes;
    }
    if (novoValor->dt.ano == dados->countriesAnoMax &&
        novoValor->dt.mes > dados->countriesMesMax)
    {
        dados->countriesMesMax = novoValor->dt.mes;
    }

    //Determinar ano máx e mín
    if (novoValor->dt.ano < dados->countriesAnoMin)
    {
        dados->countriesAnoMin = novoValor->dt.ano;
        dados->countriesMesMin = novoValor->dt.mes;
    }
    if (novoValor->dt.ano > dados->countriesAnoMax)
    {
        dados->countriesAnoMax = novoValor->dt.ano;
        dados->countriesMesMax = novoValor->dt.mes;
    }
}

/**
 * Ao ler uma linha do ficheiro das cidades atualiza os valores dos maximos e minimos
**/
void readFileCitiesAtualizaMaxMin(DADOS* dados, dados_temp* novoValor) {
    if (novoValor->dt.ano == dados->citiesAnoMin &&
        novoValor->dt.mes < dados->citiesMesMin)
    {
        dados->citiesMesMin = novoValor->dt.mes;
    }
    if (novoValor->dt.ano == dados->citiesAnoMax &&
        novoValor->dt.mes > dados->citiesMesMax)
    {
        dados->citiesMesMax = novoValor->dt.mes;
    }

    //Determinar ano máx e mín
    if (novoValor->dt.ano < dados->citiesAnoMin)
    {
        dados->citiesAnoMin = novoValor->dt.ano;
        dados->citiesMesMin = novoValor->dt.mes;
    }
    if (novoValor->dt.ano > dados->citiesAnoMax)
    {
        dados->citiesAnoMax = novoValor->dt.ano;
        dados->citiesMesMax = novoValor->dt.mes;
    }
    //determinar a temperatura máxima e mínima
    if (novoValor->temp < dados->citiesTempMin)
    {
        dados->citiesTempMin = novoValor->temp;
    }
    if (novoValor->temp > dados->citiesTempMax)
    {
        dados->citiesTempMax = novoValor->temp;
    }
}

//Coloca os dados de uma linha numa estrutura do tipo dados_temp
//Se não leu corretamente devolve NULL
dados_temp *readFileLineToStruct(char* line, bool cidades) {
    int check = -1;
    char longitude[BUFFER_SIZE] = {'\0'};
    char latitude[BUFFER_SIZE] = {'\0'};

    dados_temp *a = malloc(sizeof(dados_temp));

    if(cidades) {
        check = sscanf(line, "%d-%d-%d,%f,%f,%[^,],%[^,],%[^,],%[^,]",
            &a->dt.ano,
            &a->dt.mes,
            &a->dt.dia,
            &a->temp,
            &a->incerteza,
            a->cidade,
            a->pais,
            latitude,
            longitude);
        
        a->latitude.direcao = (latitude[strlen(latitude)-1] == 'N') ? NORTE : SUL;
        a->longitude.direcao = (longitude[strlen(longitude)-1] == 'W') ? OESTE : ESTE;
        
        //Tira o carcater da direção da string
        latitude[strlen(latitude)-1] = '\0';
        longitude[strlen(longitude)-1] = '\0';
        a->latitude.angular = atof(latitude);
        a->longitude.angular = atof(longitude);
    }
    else
    {
        check = sscanf(line, "%d-%d-%d,%f,%f,%[^\n]",
                        &a->dt.ano,
                        &a->dt.mes,
                        &a->dt.dia,
                        &a->temp,
                        &a->incerteza,
                        a->pais);
    }

    if((!cidades && check != 6) || (cidades && check != 9)) {
        //Se a linha não foi bem lida coloca o apontador para a estrutura a NULL
        free(a);
        a = NULL;
    }

    return a;
}

//Devolve o numero de linhas lidas corretamente
int readFileToList(char* fileName, DADOS* dados, bool cidades)
{
    long totalFileSize = 0;
    int i = 0;
    char buffer[BUFFER_SIZE];
    FILE *fileInput = NULL;

    list_node_t* list = cidades ? dados->headCitiesOriginal : dados->headCountriesOriginal;

    int lastPercentage = 0;

    fileInput = checkedFopen(fileName, "r");

    totalFileSize = getFileSize(fileInput);

    printf("Progresso: 0%%");

    while (fgets(buffer, BUFFER_SIZE, fileInput) != NULL)
    {
        int percentage = 0;
        removeLastCharIfMatch(buffer, '\n');
        removeLastCharIfMatch(buffer, '\r');

        dados_temp* a = readFileLineToStruct(buffer, cidades);

        if (a != NULL)
        {
            insert_node(list, a);
            cidades ? readFileCitiesAtualizaMaxMin(dados, a) : readFileCountriesAtualizaMaxMin(dados, a);
            //readFileAtualizaMaxMin(dados, a);
            i++;
        }



        //A cada 100 linhas calcula a percentagem e atualiza o progresso de leitura
        if(i % 100) {
            percentage = ftell(fileInput) * 100 / totalFileSize;
            if (lastPercentage != percentage)
            {
                printf("\rProgresso: %d%%", percentage);
                fflush(stdout);
                lastPercentage = percentage;
            }
        }

    }

    fclose(fileInput);
    return i;
}

/**
 * Ordena a lista por data atravez do merge sort
**/
void mergeSort(list_node_t* list, int listSize) {
    int n = 1;
    if(listSize < 2) {
        //Sair da funcao sem ordenar
        return;
    }
    int nMax = (listSize-1)/8 * 8;

    while(n <= nMax) {
        list_node_t* aux = list->next; //Passa da dummy node
        int b = 0;
        while(aux != NULL) {
            b++;
            int i = 0, j = 0;
            list_node_t* sortI = aux;
            list_node_t* sortJ = aux;

            dados_temp** vect = (dados_temp**) checkedMalloc(sizeof(dados_temp*) * 2*n);

            for(int i = 0; i<n; i++) {
                sortJ = sortJ->next;
                if(sortJ == NULL) {
                    break;
                }
            }

            for(int k = 0; k<(n*2); k++)
            {
                if(sortI == NULL && sortJ == NULL) break;

                if((sortJ == NULL || j == n) ||
                    (i < n && sortI != NULL && dataMenorQue(&sortI->payload->dt, &sortJ->payload->dt)))
                {
                    //Se a lado do j já tiver acabado ou o payload do i for menor que o do payload do j
                    vect[k] = sortI->payload;
                    sortI = sortI->next;
                    i++;
                }
                else
                {
                    //Se sortJ->payload for menor, ou sortI chegou ao fim, mete o resto do sortJ
                    vect[k] = sortJ->payload;
                    sortJ = sortJ->next;
                    j++;
                }
            }

            //Copia do vetor para a lista e avança o aux
            for(int a = 0; a<n*2; a++) {
                aux->payload = vect[a];
                aux = aux->next;
                if(aux == NULL) {
                    break;
                }
            }

            free(vect);
        }
        n += n;
    }
}
