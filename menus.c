#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "menus.h"
#include "estruturas.h"
#include "util.h"

#define BUFFER_SIZE 100

#define JANEIRO 1
#define DEZEMBRO 12
#define MAX_M 100 //não sei , perguntar ao stor
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
//TODO
//#define LIMITE_ANOS_STRING
//#define DOUBLECAT(a,b,c) a##0-2018##c

//Le o que o utilizador escreveu e devolve o numero da alinea. Caso seja invalido devolve -1
//TODO qual é o xxxh destas funções?
int perguntar_menu_choice()
{
    int alinea = 0;
    char buf[BUFFER_SIZE];

    fgets(buf, BUFFER_SIZE, stdin);
    if (sscanf(buf, "%d", &alinea) == 0)
    {
        //TODO veriricar se o utilizador escrever algo do genero de "2 5" (isto deveria dar invalido)
        alinea = -1;
    }
    return alinea;
}

//esta função permite selecionar um ano a analisar
//um ano apartir do qual se vai analisar
//ou um numero de meses a analisar
int perguntar_referencia_a_analisar(int _min, int _max, char *_comentario)
{
    char buf[BUFFER_SIZE];
    int _ano = 0;
    bool primeiraTentativa = true;

    do
    {
        if(!primeiraTentativa) {
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

void getstring(char* pais, char _comentario[]) {
    char buffer[BUFFER_SIZE];

    printf("%s ", _comentario);

    fgets(buffer, BUFFER_SIZE, stdin);
    removeNewLine(buffer);
    strcpy(pais, buffer);
}

// Devolve o numero de intervalos quando se separa os dados em intervalos(com um certo periodo)
int calculo_num_intervalos(int _periodo, DADOS *dados)
{
    //TODO isto apenas considera os min e max dos countries
    int intervalos = (dados->countriesAnoMax - dados->countriesAnoMin)/_periodo;
    if((dados->countriesAnoMax - dados->countriesAnoMin) % _periodo > 0) {
        //Caso os intervalor não dividam exatamente os anos, vai haver mais um intervalo com menos anos que o periodo
        intervalos++;
    }
    return intervalos;
}


void menu_principal(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;

    while (dentroDoMenu)
    {
        printf("\n\n\tMenu Principal\n\nEscolha uma das opções seguintes:\n");
        printf("1. Filtragem de dados;\n2. Histórico de Temperaturas;\n");
        printf("3. Análise da temperatura por ano;\n4. Análise da temperatura global.\n");
        printf("\n0 ou Enter. Sair do programa.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            menu_filtragem_de_dados(dados);
            break;
        case 2:
            menu_historico_de_temperaturas(dados);
            break;
        case 3:
            menu_analise_da_temperatura_por_ano(dados);
            break;
        case 4:
            menu_analise_da_temperatura_global(dados);
            break;
        case 0:
            printf("Quit\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }
}

void menu_historico_de_temperaturas(DADOS* dados)
{
    bool dentroDoMenu = false;
    int alinea = -1;
    int periodo = -1;
    int intervalo = 0;
    char comentario[] = "Qual o periodo que pretende analisar?";

    printf("\n\tMenu Histórico de Temperaturas\n\n");
    intervalo = dados->countriesAnoMax - dados->countriesAnoMin;

    periodo = perguntar_referencia_a_analisar(1, intervalo, comentario);
    //printf("período(num de anos) a analisar: %d\n", periodo);

    do
    {
        //Quando se escolhe qualquer opção ele sai logo do menu. No entanto se for opção
        // invalida ele deve fazer loop
        dentroDoMenu = false;

        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Global: aplica-se a todos os países;\n2. Por País: aplica-se a um único país;\n");
        printf("3. Por Cidade: aplica-se a uma única cidade.\n\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            historico_de_temperaturas_global(dados, periodo);
            break;
        case 2:
            historico_de_temperaturas_por_pais(dados, periodo);
            break;
        case 3:
            historico_de_temperaturas_por_cidade(dados, periodo);
            break;
        case 0:
            break;
        default:
            printf("\nOpção inválida\n");
            dentroDoMenu = true;
            break;
        }
    } while (dentroDoMenu);
}
void menu_analise_da_temperatura_por_ano(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;
    int ano;
    char comentario[] = "Qual o ano a que pretende analisar?";

    printf("\n\tMenu Análise da Temperatura por Ano\n\n");


    ano = perguntar_referencia_a_analisar(dados->countriesAnoMin, dados->countriesAnoMax, comentario);

    while (dentroDoMenu)
    {
        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Análise por País;\n2. Análise por cidade.\n");
        printf("\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            analise_por_pais(ano);
            break;
        case 2:
            analise_por_cidade(ano);
            break;
        case 0:
            dentroDoMenu = false;
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }
}

//MENU Filtragem de dados
void menu_filtragem_de_dados(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;
    bool alterouCriterios = false;

    //Copia os critérios aplicados aos dados
    CRITERIOS_FILTRAGEM novosCriterios = dados->criterios;

    while (dentroDoMenu)
    {
        printf("\n\tMenu Filtragem de Dados\n\nEscolha uma das opções seguintes:\n");
        printf("1. Limpa critéritos;\n2. Escolhe intervalos para analise;\n");
        printf("3. Escolhe um mês.\n\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            opcao_limpa_criteritos(&novosCriterios);
            alterouCriterios = true;
            break;
        case 2:
            opcao_escolhe_intervalos_para_analise(&novosCriterios, dados);
            alterouCriterios = true;
            break;
        case 3:
            opcao_escolhe_mes(&novosCriterios);
            alterouCriterios = true;
            break;
        case 0:
            dentroDoMenu = false; //Sai do menu, voltando para o anterior
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }

    //Antes de sair do menu aplica criterios
    if(alterouCriterios) {
        printf("A aplicar critérios...\n");
        dados_aplicar_novos_criterios(dados, &novosCriterios);
        printf("Concluído\n");
    }
}
void opcao_limpa_criteritos(CRITERIOS_FILTRAGEM *criterios)
{
    limpar_criterios(criterios);
}
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios, DADOS *dados)
{
    printf("\n\n\t---Escolhe intervalos para análise---\n\n");
    printf("Os dados anteriores ao periodo que inserir não serão considerados.\n");

    char buf[BUFFER_SIZE];
    int mes = 0, ano = 0;
    bool primeiraTentativa = true;
    char comentario[] = "Qual o ano a partir do qual pretende analisar?";
    //bool intervalo_valido = true;

    /*do
    {
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }

        printf("Qual o ano a partir do qual pretende analisar [%d-%d]:\t", MIN_ANO, MAX_ANO);

        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &ano) != 1)
        {
            ano = -1;
        }
        primeiraTentativa = false;
    } while (ano < MIN_ANO || MAX_ANO < ano);

    printf("\n");*/
    ano = perguntar_referencia_a_analisar(dados->countriesAnoMin,
        dados->countriesAnoMax, comentario);

    do
    {
        //Caso o utilizador já tenha inserido um mes invalido
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }

        //Caso o
        if(ano == dados->countriesAnoMin) {
            //TODO fazer com que um ano sem ser o minimo também apareca isto
            printf("Para o ano de %d só há valores apartir do mês %d !\n", ano, dados->countriesMesMin);
            //intervalo_valido = true;
        }
        printf("Qual o mes a partir do qual pretende analisar [%d-12]:\t",
            ano == dados->countriesAnoMin ? dados->countriesMesMin : 1);


        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &mes) != 1)
        {
            mes = -1;
        }

        primeiraTentativa = false;
        //printf("%d < %d || %d < %d || %d\n", criterios->intervaloMesMin, JANEIRO_NUM, DEZEMBRO_NUM,  criterios->intervaloMesMin, !intervalo_valido);

        //Volta a pedir valores ao utilizador caso sejam inseridos valores de
        // meses invalidos, ou, caso seja o menor ano, o mes escolhido
        // pode não estar disponivel
    } while ((mes < JANEIRO || DEZEMBRO < mes)
                || (ano == dados->countriesAnoMin && mes < dados->countriesMesMin));

    criterios->filtraPorIntervalo = true;
    criterios->intervaloAnoMin = ano;
    criterios->intervaloMesMin = mes;
}
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios)
{
    printf("\n\n\t---Escolhe um mês---\n\n");
    printf("Apenas serão considerados os dados do periodo delimitado");

    char buf[BUFFER_SIZE];
    int mes_i = 0, mes_f = 0;
    bool primeiraTentativa = true;

    do
    {
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }

        printf("Qual o periodo de meses que pertende analidar ao longo dos anos [%d-%d]:\t", JANEIRO, DEZEMBRO);

        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d %d", &mes_i, &mes_f) == 1)
        {
            printf("Introduza os números do intervalo separado por pelo menos um espaço\n");
        }
        if (sscanf(buf, "%d %d", &mes_i, &mes_f) !=2)
        {
            mes_i = -1;
            mes_f = mes_i;
        }
        primeiraTentativa = false;
        //TODO pode haver intervalos de 1 mês??
    } while (mes_i < JANEIRO || DEZEMBRO < mes_i || mes_f < JANEIRO || DEZEMBRO < mes_f );

    criterios->filtraPorIntervalo = true;
    criterios->mesMin = mes_i;
    criterios->mesMax = mes_f;
}

void calc_medias_de_intervalos(int numIntervalos, float *tempMed, int *numDados) {
    //Calcula as médias
    for(int i = 0; i<numIntervalos; i++) {
        if(numDados[i] > 0) {
            tempMed[i] = tempMed[i]/numDados[i];
        }
    }
}

//TODO esta funcao tem demasiados parametros
void imprime_intervalos(DADOS* dados, int numIntervalos, float *tempMin,
    float *tempMax, float *tempMed, int *numDados, int periodo) {
    //Imprime os intervalos
    for(int i = 0; i<numIntervalos; i++) {
        bool intervalorFechado = false;

        //Determina quais os anos que o intervalo compreende
        int anoMenor = dados->countriesAnoMin + i*periodo;
        int anoMaior = dados->countriesAnoMin + (i+1)*periodo;

        if(anoMaior >= dados->countriesAnoMax) {
            //Caso seja o ultimo intervalo, mostra o intervalo como um intervalo fechado
            anoMaior = dados->countriesAnoMax;
            intervalorFechado = true;
        }

        if(numDados[i] > 0) {
            if(intervalorFechado) {
                printf("[ %d , %d ]\tMin:%f Max:%f Med:%f\n", anoMenor, anoMaior,
                    tempMin[i], tempMax[i], tempMed[i]);
            } else {
                printf("[ %d , %d [\tMin:%f Max:%f Med:%f\n", anoMenor, anoMaior,
                    tempMin[i], tempMax[i], tempMed[i]);
            }
        } else {
            printf(intervalorFechado ?
                "[ %d , %d ]\tSEM DADOS\n" : "[ %d , %d [\tSEM DADOS\n",
                anoMenor, anoMaior);
        }
    }
}

void historico_de_temperaturas_global(DADOS *dados, int periodo)
{
    //TODO meter historico_de_temperaturas_global, historico_de_temperaturas_por_pais e cidade
    //TODO a usar todas a mesma funcao que tem um parametro a dizer se filtra por cidade ou pais ou se nao(global)
    float *tempMax = NULL, *tempMin = NULL, *tempMed = NULL;
    int* numDados = NULL;  //Numero de dados em cada intervalo para fazer a média
    int numIntervalos = 0;
    list_node_t *aux = NULL;

    printf("\n\n\t---Histórico de Temperaturas global---\n\n");

    numIntervalos = calculo_num_intervalos(periodo, dados);

    //Cria vetores com o numero de intervalos
    tempMax = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMin = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMed = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    numDados = (int*) checkedMalloc(sizeof(int) * numIntervalos);

    for(int i = 0; i<numIntervalos; i++) {
        //Inicializa cada intervalo
        tempMax[i] = -__FLT_MAX__;
        tempMin[i] = __FLT_MAX__;
        tempMed[i] = 0;
        numDados[i] = 0;
    }

    aux = dados->headCountriesFiltrada->next; //->next para a node a seguir à dummy node
    while(aux != NULL) {
        int intervalo = (aux->payload->dt.ano - dados->countriesAnoMin) / periodo;

        tempMax[intervalo] = MAX(tempMax[intervalo], aux->payload->temp);
        tempMin[intervalo] = MIN(tempMin[intervalo], aux->payload->temp);
        // O tempMed vai temporariamente guardar a soma das temperaturas
        tempMed[intervalo] += aux->payload->temp;
        numDados[intervalo]++;
        aux = aux->next;
    }

    calc_medias_de_intervalos(numIntervalos, tempMed, numDados);
    imprime_intervalos(dados, numIntervalos, tempMin, tempMax, tempMed, numDados, periodo);
}

void historico_de_temperaturas_por_pais(DADOS *dados, int periodo)
{
    char pais[100];
    float *tempMax = NULL, *tempMin = NULL, *tempMed = NULL;
    int* numDados = NULL;  //Numero de dados em cada intervalo para fazer a média
    int numIntervalos = 0;
    char comentario[] = "País a analisar: ";
    list_node_t *aux = NULL;

    printf("\n\n\t---Histórico de Temperaturas por País---\n\n");

    getstring(pais, comentario);

    numIntervalos = calculo_num_intervalos(periodo, dados);

    //Cria vetores com o numero de intervalos
    tempMax = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMin = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMed = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    numDados = (int*) checkedMalloc(sizeof(int) * numIntervalos);

    for(int i = 0; i<numIntervalos; i++) {
        //Inicializa cada intervalo
        tempMax[i] = -__FLT_MAX__;
        tempMin[i] = __FLT_MAX__;
        tempMed[i] = 0;
        numDados[i] = 0;
    }

    aux = dados->headCountriesFiltrada->next; //->next para a node a seguir à dummy node
    while(aux != NULL) {
        if(strstr(aux->payload->pais, pais) != NULL) {
            //Se o pais desta node coincidir com o que o utilizador escolheu
            //Obtem o intervalo a que o ano pertence
            int intervalo = (aux->payload->dt.ano - dados->countriesAnoMin) / periodo;

            tempMax[intervalo] = MAX(tempMax[intervalo], aux->payload->temp);
            tempMin[intervalo] = MIN(tempMin[intervalo], aux->payload->temp);
            // O tempMed vai temporariamente guardar a soma das temperaturas
            tempMed[intervalo] += aux->payload->temp;
            numDados[intervalo]++;
        }
        aux = aux->next;
    }

    calc_medias_de_intervalos(numIntervalos, tempMed, numDados);
    imprime_intervalos(dados, numIntervalos, tempMin, tempMax, tempMed, numDados, periodo);
}

void historico_de_temperaturas_por_cidade(DADOS *dados, int periodo)
{
    char cidade[100];
    float *tempMax = NULL, *tempMin = NULL, *tempMed = NULL;
    int* numDados = NULL;  //Numero de dados em cada intervalo para fazer a média
    int numIntervalos = 0;
    char comentario[] = "Cidade a analisar: ";
    list_node_t *aux = NULL;

    printf("\n\n\t---Histórico de Temperaturas por Cidade---\n\n");

    getstring(cidade, comentario);

    numIntervalos = calculo_num_intervalos(periodo, dados);

    //Cria vetores com o numero de intervalos
    tempMax = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMin = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    tempMed = (float*) checkedMalloc(sizeof(float) * numIntervalos);
    numDados = (int*) checkedMalloc(sizeof(int) * numIntervalos);

    for(int i = 0; i<numIntervalos; i++) {
        //Inicializa cada intervalo
        tempMax[i] = -__FLT_MAX__;
        tempMin[i] = __FLT_MAX__;
        tempMed[i] = 0;
        numDados[i] = 0;
    }

    aux = dados->headCitiesFiltrada->next; //->next para a node a seguir à dummy node
    while(aux != NULL) {
        if(strstr(aux->payload->cidade, cidade) != NULL) {
            //Se o pais desta node coincidir com o que o utilizador escolheu
            //Obtem o intervalo a que o ano pertence
            int intervalo = (aux->payload->dt.ano - dados->countriesAnoMin) / periodo;

            tempMax[intervalo] = MAX(tempMax[intervalo], aux->payload->temp);
            tempMin[intervalo] = MIN(tempMin[intervalo], aux->payload->temp);
            // O tempMed vai temporariamente guardar a soma das temperaturas
            tempMed[intervalo] += aux->payload->temp;
            numDados[intervalo]++;
        }
        aux = aux->next;
    }

    calc_medias_de_intervalos(numIntervalos, tempMed, numDados);
    imprime_intervalos(dados, numIntervalos, tempMin, tempMax, tempMed, numDados, periodo);
}

void analise_por_pais(int ano)
{
    printf("\n\n\t---Análise por País---\n\n");
}
void analise_por_cidade(int ano)
{
    printf("\n\n\t---Análise por cidade---\n\n");
}

void fgetstring(list_node_t * aux, bool string_pais, char string [BUFFER_SIZE])
{
    while(aux != NULL)
    {
        if(string_pais && (strstr(aux->payload->pais, string) != NULL))
        {
            printf("found country :: %s\n", aux->payload->pais);
        }
        else if(strstr(aux->payload->cidade,string) != NULL)
        {
            printf("found country :: %s\n", aux->payload->cidade);
        }
        aux = aux->next;
    }
}

void menu_analise_da_temperatura_global(DADOS *dados)
{
    int M = 0;
    list_node_t * tmp_countries = dados->headCountriesFiltrada;
    list_node_t * tmp_cities = dados->headCitiesFiltrada;
    char f_pais[BUFFER_SIZE];
    char f_cidade[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char comentario[] = "Quantos meses pretende utilizar no cálculo da MA (moving average)?";

    printf("\n\n\t---Análise da temperuta Global---\n\n");

    M = perguntar_referencia_a_analisar(1 ,MAX_M, comentario);

    printf("Country::\t");
    fgets(f_pais, BUFFER_SIZE, stdin);
    printf("City::\t");
    fgets(f_cidade, BUFFER_SIZE, stdin);
    removeNewLine(f_pais);
    removeNewLine(f_cidade);
    
    //printf("f_cidade:: %s <- %lu\n", f_cidade, strlen(f_cidade));

    /*while(aux != NULL)
    {
        if(strcmp(tmp_countries->payload.pais,f_pais) == 0)
        {
            printf("found country\n");
            printf(" %s \n <- %lu\n",(tmp_countries->payload.pais), strlen(tmp_countries->payload.pais));
        }
        if(strcmp(tmp_cities->payload.cidade,f_cidade) == 1)
        {
            printf("found city\n");
            printf(" %s \n <- %lu\n",(tmp_cities->payload.cidade), strlen(tmp_cities->payload.cidade));
        }
        aux = aux->next;
    }*/

    fgetstring(tmp_countries, true, f_pais);
    fgetstring(tmp_cities, false, f_cidade);

}
