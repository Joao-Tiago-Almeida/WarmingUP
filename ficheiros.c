#include "ficheiros.h"
#include "lista.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define CITIES_FILE "tempcities.csv"
//#define COUNTRIES_FILE "tempcountries.csv"
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

int read_file_countries(list_node_t **_head, char *_nomeFilePaises, char *_nomeFileCidades)
{
    printf("read_file_countries\n");
    int i = 0, check = -1;

    char nome_temp[MAX_STRING];
    char buffer[BUFFER];

    *_head = create_list();

    //int min = 2000, max = 1, mes_ano_min = 0, mes_ano_max = 0;


//TODO se o ficheiro estiver mal, tem de se escrever exit em vez de return
    FILE * fileInput = NULL;
    fileInput = fopen(_nomeFilePaises, "r");
    if (fileInput == NULL) {
        //Caso haja um erro imprime-o e sai do programa
        perror("Erro a abrir o ficheiro");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, BUFFER, fileInput) != NULL){

        dados_temp a;
        check = sscanf(buffer, "%d-%d-%d,%f,%f,%[^\n]",
            &a.dt.ano,
            &a.dt.mes,
            &a.dt.dia,
            &a.temp,
            &a.incerteza,
            nome_temp);

        strcpy(a.pais, nome_temp);

        if ( check == 6 )
        {
            //list_node_t* node = create_node(&a);
            //sortedInsert(*_head, node);
            insert_node(*_head, &a);
            printf("%d\t", i);
            i++;
        }
        /*if (a.dt.ano > max){
                max = a.dt.ano;
                mes_ano_max = a.dt.mes;
        }
        if (a.dt.ano < min){
                min = a.dt.ano;
                mes_ano_min = a.dt.mes;
        }*/


    }
    //printf("\nmax(mm-aaaa):: 01-%d-%d\nmin(mm-aaaa):: 01-%d-%d\n", max, mes_ano_max, min, mes_ano_min);

    fclose(fileInput);

    /*for(int i = 0 ; i< 50 ; i++)
    {
        dados_temp a;
        a.dt.ano = rand()%20 +1;
        a.dt.mes = rand()%6 +7;
            list_node_t* node = create_node(&a);
            sortedInsert(*_head, node);
            printf("%d\t", i);

    }*/

    //print_list(_head);
    return i;
}
