#include "util.h"
#include "ficheiros.h"
#include "menus.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 100

//Aloca memoria, e faz a verificação se o return do malloc não foi NULL
void *checkedMalloc(size_t size) {
    void *mem = malloc(size);

    if(mem == NULL)
    {
        printf("Out of memory!\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}


//Aloca memoria, e faz a verificação se o return do realloc não foi NULL
void *checkedRealloc(void *ptr, size_t size) {
    void *mem = realloc(ptr, size);

    if(mem == NULL)
    {
        printf("Out of memory!\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}
/**
* Elimina o carater do parâmetro de entrada 2, se este estiver na penúltima posição da string
*/
void removeLastCharIfMatch(char* str, char c) {
    if(str[strlen(str)-1] == c)
    {
        str[strlen(str)-1] = '\0';
    }
}

/**
 * Inicia o programa ou muda para o modo textual
 * Devolve false se for para sair do programa ou true se for para mudar para o modo textual
**/
bool ModoTextual(char *nomeFilePaises, char *nomeFileCidades, DADOS *dados )
{
    if(dados->headCountriesOriginal == NULL)  read_file_countries (dados, nomeFilePaises);
    if(dados->headCitiesOriginal == NULL)  read_file_cities (dados,nomeFileCidades);

    //print_list(dados->headCountriesOriginal);
    //print_list(dados->headCitiesOriginal);

    return menu_principal(dados);
}

/**
 * Le a opção que o utilizador escolheu e devolve seu numero. Caso seja invalido devolve -1
**/
int perguntar_menu_choice()
{
    int alinea = 0;
    char buf[BUFFER_SIZE];

    fgets(buf, BUFFER_SIZE, stdin);
    if (sscanf(buf, "%d", &alinea) != 1)
    {
        alinea = -1;
    }
    return alinea;
}

/**
 * Pede ao utilizador uma valor entre os parametros _min e _max,
 *      imprimindo a string _comentario
 * Devolve o valor escolhido
**/
int perguntar_referencia_a_analisar(int _min, int _max, char *_comentario)
{
    char buf[BUFFER_SIZE];
    int _ano = 0;
    bool primeiraTentativa = true;

    do
    {
        if(!primeiraTentativa)
        {
            printf("Tem de estar dentro dos limites!\n");
        }
        printf("%s [%d-%d]:\t", _comentario, _min, _max);
        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &_ano) != 1)
        {
            _ano = -1;
        }
        primeiraTentativa = false;
    } while (_ano < _min || _max < _ano);

    printf("\n");
    return _ano;
}

/**
 * Pede ao utilizador que presione um tecla. Apenas o caracter opcao1 e o opcao2 são
 *      válidos (tanto maiusculo como minusculo)
 * Devolve a tecla escolhida
**/
char pergunta_tecla(char* comentario, char opcao1, char opcao2)
{
    char buffer[BUFFER_SIZE];
    do {
        printf("%s", comentario);
        fgets(buffer, BUFFER_SIZE, stdin);
        removeLastCharIfMatch(buffer, '\n');
    } while(strlen(buffer) > 1 || (tolower(buffer[0]) != tolower(opcao1) && tolower(buffer[0]) != tolower(opcao2)));

    return tolower(buffer[0]);
}

/**
 * Imprime no terminal o comentário e
 *      copia a string inserida no terminal para a string do parametro string
 */
void getstring(char* string, char _comentario[]) {
    char buffer[BUFFER_SIZE];

    printf("%s ", _comentario);

    fgets(buffer, BUFFER_SIZE, stdin);
    removeLastCharIfMatch(buffer, '\n');
    strcpy(string, buffer);
}
