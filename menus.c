#include "menus.h"
#include <stdio.h>
#include <stdlib.h>

#define buffer 2

void menu_principal(int *_periodo_a_analisar, int *_ano_a_analisar)
{
  char buf[buffer];
  int alinea = -1;

  printf("\n\tMenu Principal\n\nEscolha uma das opções seguintes:\n");
  printf("1. Filtragem de dados;\n2. Histórico de Temperaturas;\n");
  printf("3. Análise da temperatura por ano;\n4. Análise da temperatura global.\n");
  printf("\n0. Sair do programa.\n");
  do
  {
  fgets(buf, buffer, stdin);
  sscanf(buf, "%d", &alinea);
  }while(alinea < 0 || 5 < alinea );
  switch (alinea) {
    case 1: menu_filtragem_de_dados(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 2: menu_historico_de_temperaturas(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 3: menu_analise_da_temperatura_por_ano(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 4: menu_analise_da_temperatura_global();
      break;
    case 0:
      printf("Quit\n");
      exit(EXIT_SUCCESS);
      break;
    default:
      printf("\nOpção inválida\n");
      return;
      break;
    }
}
void menu_filtragem_de_dados(int *_periodo_a_analisar, int *_ano_a_analisar)
{
  char buf[buffer];
  int alinea = -1;

  printf("\n\tMenu Filtragem de Dados\n\nEscolha uma das opções seguintes:\n");
  printf("1. Limpa critéritos;\n2. Escolhe intervalos para analise;\n");
  printf("3. Escolhe um mês.\n\n0. Volta ao Menu Principal.\n");
  do {
    fgets(buf, buffer, stdin);
    sscanf(buf, "%d", &alinea);
  } while(alinea < 0 || 4 < alinea);
  switch (alinea) {
    case 1: limpa_criteritos();
      break;
    case 2: escolhe_intervalos_para_analise();
      break;
    case 3: escolhe_mes();
      break;
    case 0: menu_principal(_periodo_a_analisar, _ano_a_analisar);
      break;
    default:
      printf("\nOpção inválida\n");
      return;
      break;
    }
}
void menu_historico_de_temperaturas(int *_periodo_a_analisar , int *_ano_a_analisar)
{
  char buf[buffer], ano[buffer];
  int alinea = -1;
                                  //não sei o intervalo do periodo
  printf("\n\tMenu Histórico de Temperaturas\n\nQual o periodo que pretende analisar [1-500]:\n");
  *_periodo_a_analisar = -1;      //pode vir com outros valores de fora
  do {
    fgets(ano, buffer, stdin);
    sscanf(ano, "%d", _periodo_a_analisar);
  } while(*_periodo_a_analisar < 1 || 500 < *_periodo_a_analisar);
  printf("período a analisar: %d\n", *_periodo_a_analisar);

  printf("\n\nEscolha uma das opções seguintes:\n");
  printf("1. Global: aplica-se a todos os países;\n2. Por País: aplica-se a um único país;\n");
  printf("3. Por Cidade: aplica-se a uma única cidade.\n\n0. Volta ao Menu Principal.\n");
  do{
  fgets(buf, buffer, stdin);
  sscanf(buf, "%d", &alinea);
  }while ((alinea < 0 || 3 < alinea));

  switch (alinea) {
    case 1: historico_de_temperaturas_global();
      break;
    case 2: historico_de_temperaturas_por_pais();
      break;
    case 3: historico_de_temperaturas_por_cidade();
      break;
    case 0:
      menu_principal(_periodo_a_analisar, _ano_a_analisar);
      *_periodo_a_analisar = -1;
      break;
    default:
      printf("\nOpção inválida\n");
      return;
      break;
    }

}
void menu_analise_da_temperatura_por_ano(int *_periodo_a_analisar , int *_ano_a_analisar)
{
  char buf[buffer], ano[buffer];
  int alinea = -1;

  //nao sei o intervalo dos anos
  printf("\n\tMenu Análise da Temperatura por Ano\n\nQual o ano que pretende analisar [0-2018]:\n");
  *_ano_a_analisar = -1;

  do {
    fgets(ano, buffer, stdin);
    sscanf(ano, "%d", _ano_a_analisar);
  } while(*_ano_a_analisar < 0 || 2018 < *_ano_a_analisar);

  printf("\n\nEscolha uma das opções seguintes:\n");
  printf("1. Análise por País;\n2. Análise por cidade.\n");
  printf("\n0. Volta ao Menu Principal.\n");
  do {
    fgets(buf, buffer, stdin);
    sscanf(buf, "%d", &alinea);
  } while(alinea < 0 || 2 < alinea );

  switch (alinea) {
    case 1: analise_por_pais();
      break;
    case 2: analise_por_cidade();
      break;
    case 0:
      menu_principal(_periodo_a_analisar, _ano_a_analisar);
      *_ano_a_analisar = -1;;
      break;
    default:
      printf("\nOpção inválida\n");
      return;
      break;
  }
}
void menu_analise_da_temperatura_global()
{
  printf("\n\n\t---Análise da temperuta Global---\n\n");
}
void limpa_criteritos()
{
  printf("\n\n\t---Limpa Critéritos---\n\n");
}
void escolhe_intervalos_para_analise()
{
  printf("\n\n\t---Escolhe intervalos para análise---\n\n");
}
void escolhe_mes()
{
  printf("\n\n\t---Escolhe mês---\n\n");
}
void historico_de_temperaturas_global()
{
  printf("\n\n\t---Histórico de Temperaturas Global---\n\n");
}
void historico_de_temperaturas_por_pais()
{
  printf("\n\n\t---Histórico de Temperaturas por País---\n\n");
}
void historico_de_temperaturas_por_cidade()
{
  printf("\n\n\t---Histórico de Temperaturas por Cidade---\n\n");
}
void analise_por_pais()
{
  printf("\n\n\t---Análise por País---\n\n");
}
void analise_por_cidade()
{
  printf("\n\n\t---Análise por cidade---\n\n");
}
