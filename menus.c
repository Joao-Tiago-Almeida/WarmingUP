#include "menus.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 100

#define MIN_ANO 0 //TODO ver se isto está correto
#define MAX_ANO 2018

#define JANEIRO_NUM 1
#define DEZEMBRO_NUM 12

//TODO
//#define LIMITE_ANOS_STRING 
//#define DOUBLECAT(a,b,c) a##0-2018##c

//Le o que o utilizador escreveu e devolve o numero da alinea. Caso seja invalido devolve -1
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

int perguntar_anos_a_analisar()
{
    char buf[BUFFER_SIZE];
    int numAnos = -1;

    printf("Qual o periodo que pretende analisar [1-500]: ");

    do
    {
        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &numAnos) == 0)
        {
            numAnos = -1;
        }
    } while (numAnos < 1 || 500 < numAnos);

    return numAnos;
}

int perguntar_ano_a_analisar()
{
    char buf[BUFFER_SIZE];
    int ano = -1;

    printf("Qual o ano que pretende analisar [0-2018]:");

    do
    {
        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &ano) == 0)
        {
            ano = -1;
        }
    } while (ano < MIN_ANO || MAX_ANO < ano);

    return ano;
}



void menu_principal(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;

    while (dentroDoMenu)
    {
        printf("\n\tMenu Principal\n\nEscolha uma das opções seguintes:\n");
        printf("1. Filtragem de dados;\n2. Histórico de Temperaturas;\n");
        printf("3. Análise da temperatura por ano;\n4. Análise da temperatura global.\n");
        printf("\n0. Sair do programa.\n");

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
            menu_analise_da_temperatura_global();
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

    printf("\n\tMenu Histórico de Temperaturas\n\n");

    periodo = perguntar_anos_a_analisar();
    printf("período(num de anos) a analisar: %d\n", periodo);

    do
    {
        //Quando se escolhe qualquer opção ele sai logo do menu. No entanto se for opção
        // invalida ele deve fazer loop
        dentroDoMenu = false;

        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Global: aplica-se a todos os países;\n2. Por País: aplica-se a um único país;\n");
        printf("3. Por Cidade: aplica-se a uma única cidade.\n\n0. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            historico_de_temperaturas_global(periodo);
            break;
        case 2:
            historico_de_temperaturas_por_pais(periodo);
            break;
        case 3:
            historico_de_temperaturas_por_cidade(periodo);
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

    while (dentroDoMenu)
    {
        printf("\n\tMenu Análise da Temperatura por Ano\n\n");
        ano = perguntar_ano_a_analisar();

        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Análise por País;\n2. Análise por cidade.\n");
        printf("\n0. Volta ao Menu Principal.\n");

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
void menu_analise_da_temperatura_global()
{
    printf("\n\n\t---Análise da temperuta Global---\n\n");
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
        printf("3. Escolhe um mês.\n\n0. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            opcao_limpa_criteritos(&novosCriterios);
            alterouCriterios = true;
            break;
        case 2:
            opcao_escolhe_intervalos_para_analise(&novosCriterios);
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
    printf("\n\n\t---Critéritos Limpos---\n\n");
    limpar_criterios(criterios);
}
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios)
{
    printf("\n\n\t---Escolhe intervalos para análise---\n");
    printf("Os dados anteriores ao periodo que inserir não serão considerados.\n");
    
    char buf[BUFFER_SIZE];
    int ano = -2;
    int mes = -2;
    bool primeiraTentativa = true;

    do
    {
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }

        printf("Qual o ano a partir do qual pretende analisar [0-2018]:");

        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &ano) == 0)
        {
            ano = -1;
        }
        primeiraTentativa = false;
    } while (ano < MIN_ANO || MAX_ANO < ano);
    
    printf("\n");

    primeiraTentativa = true;
    do
    {
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }
        printf("Qual o mes a partir do qual pretende analisar [1-12]:");

        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &mes) == 0)
        {
            mes = -1;
        }
        primeiraTentativa = false;
    } while (mes < JANEIRO_NUM || mes > DEZEMBRO_NUM);


    criterios->filtraPorIntervalo = true;
    criterios->intervaloAnoMin = ano;
    criterios->intervaloMesMin = mes;
    
    printf("\n\n\t---Critério aplicado---\n\n");
}
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios)
{
    printf("\n\n\t---Escolhe mês---\n\n");
}


//
void historico_de_temperaturas_global(int periodo)
{
    printf("\n\n\t---Histórico de Temperaturas Global---\n\n");
}
void historico_de_temperaturas_por_pais(int periodo)
{
    printf("\n\n\t---Histórico de Temperaturas por País---\n\n");
}
void historico_de_temperaturas_por_cidade(int periodo)
{
    printf("\n\n\t---Histórico de Temperaturas por Cidade---\n\n");
}
void analise_por_pais(int ano)
{
    printf("\n\n\t---Análise por País---\n\n");
}
void analise_por_cidade(int ano)
{
    printf("\n\n\t---Análise por cidade---\n\n");
}
