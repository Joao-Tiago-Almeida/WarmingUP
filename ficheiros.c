#include "ficheiros.h"
#include "lista.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CITIES_FILE "tempcities.csv"
#define COUNTRIES_FILE "tempcountries.csv"
#define STRING_SIZE 50
#define BUFFER 100
#define MAX_STRING 500

/*list_node_t* headListaDados = NULL;

headListaDados = create_list();

//Teste de como criar elementos na lista
for(int i = 0; i<3; i++) {
    dados_temp a;
    a.dt.ano = 1;
    a.dt.mes = 1;
    a.dt.dia = 1;
    a.temp = 10.0f;
    a.incerteza = 1.0f;
    strcpy(a.pais, "Portugal");
    strcpy(a.cidade, "Santarém");
    a.latitude.angular = 1.0f;
    a.latitude.direcao = NORTE;
    insert_node(headListaDados, &a);
}*/

//size_countries_file = read_file_countries();

/*int read_file_countries(list_node_t *_head)
{
    int i = 0;

    char nome_temp[MAX_STRING];
    char buffer[BUFFER];

    _head = create_list();



    FILE * fileInput = NULL;
    fileInput = fopen(COUNTRIES_FILE, "r");
    if (fileInput == NULL) {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        return EXIT_FAILURE;
    }

    while(fgets(buffer, BUFFER, fileInput) != NULL){

        dados_temp a;
        sscanf(buffer, "%d-%d-%d,%f,%f,%[^\n]",
        &a.dt.ano,
        &a.dt.mes,
        &a.dt.dia,
        &a.temp,
        &a.incerteza,
        nome_temp
        );
        strcpy(a.pais, nome_temp);
        insert_node(_head, &a);
        i++;
    }

    fclose(fileInput);
    return i;
}*/
