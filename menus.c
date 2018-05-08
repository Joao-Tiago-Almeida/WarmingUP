#include "menus.h"
#include <stdio.h>
#include <stdlib.h>

void menu_principal(int *_periodo_a_analisar, int *_ano_a_analisar)
{
  char buf[2];
  int alinea = -1;

  printf("\n\tMenu Principal\n\nEscolha uma das opções seguintes:\n");
  printf("1. Filtragem de dados;\n2. Histórico de Temperaturas;\n");
  printf("3. Análise da temperatura por ano;\n4. Análise da temperatura global;\n");
  printf("5. Sair do programa.\n");
  fgets(buf, 2, stdin);
  sscanf(buf, "%d", &alinea);
  switch (alinea) {
    case 1: menu_filtragem_de_dados(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 2: menu_historico_de_temperaturas(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 3: menu_analise_da_temperatura_por_ano(_periodo_a_analisar, _ano_a_analisar);
      break;
    case 4: menu_analise_da_temperatura_global();
      break;
    case 5: exit(EXIT_SUCCESS);
      break;
    default:
      break;
    }
}
void menu_filtragem_de_dados(int *_periodo_a_analisar, int *_ano_a_analisar)
{
  char buf[2];
  int alinea = -1;

  printf("\n\tMenu Filtragem de Dados\n\nEscolha uma das opções seguintes:\n");
  printf("1. Limpa critéritos;\n2. Escolhe intervalos para analise;\n");
  printf("3. Escolhe um mês;\n4. Volta ao Menu Principal.\n");
  do {
    fgets(buf, 2, stdin);
    sscanf(buf, "%d", &alinea);
  } while(alinea < 1 );

  switch (alinea) {
    case 1: limpa_criteritos();
      break;
    case 2: escolhe_intervalos_para_analise();
      break;
    case 3: escolhe_mes();
      break;
    case 4:
    menu_principal(_periodo_a_analisar, _ano_a_analisar);
    printf("\nOpção inválida\n");
      break;
    default: return;
      break;
    }
}
void menu_historico_de_temperaturas(int *_periodo_a_analisar , int *_ano_a_analisar)
{
  char buf[2], ano[5];
  int alinea = -1;

  printf("\n\tMenu Histórico de Temperaturas\n\nQual o ano que pretende analisar:\n");
  do {
    fgets(ano, 5, stdin);
    sscanf(ano, "%d", _periodo_a_analisar);
  } while(*_periodo_a_analisar < 1);

  printf("\n\nEscolha uma das opções seguintes:\n");
  printf("4. Global: aplica-se a todos os países;\n5. Por País: aplica-se a um único país;\n");
  printf("6. Por Cidade: aplica-se a uma única cidade.\n");
  do{
  fgets(buf, 2, stdin);
  sscanf(buf, "%d", &alinea);
  }while (alinea < 1);

  switch (alinea) {
    case 4: global();
      break;
    case 5: por_pais();
      break;
    case 6: por_cidade();
      break;
    default:
    menu_principal(_periodo_a_analisar, _ano_a_analisar);
    *_periodo_a_analisar = -1;
    printf("\nOpção inválida\n");
      break;
    }

}
void menu_analise_da_temperatura_por_ano(int *_periodo_a_analisar , int *_ano_a_analisar)
{
  char buf[2], ano[5];
  int alinea = -1;

  printf("\n\tMenu Análise da Temperatura por Ano\n\nQual o ano que pretende analisar:\n");
  do {
    fgets(ano, 5, stdin);
    sscanf(ano, "%d", _ano_a_analisar);
  } while(*_ano_a_analisar < 1);

  printf("\n\nEscolha uma das opções seguintes:\n");
  printf("1. Análise por País;\n2. Análise por cidade\n");
  do {
    fgets(buf, 2, stdin);
    sscanf(buf, "%d", &alinea);
  } while(alinea < 1);

      switch (alinea) {
        case 1: analise_por_pais();
          break;
        case 2: analise_por_cidade();
          break;
        default:
          menu_principal(_periodo_a_analisar, _ano_a_analisar);
          *_ano_a_analisar = -1;
          printf("\nOpção inválida\n");
          break;
        }
}
void menu_analise_da_temperatura_global()
{

}

// vv Isto estaria noutro ficheiro vv
void limpa_criteritos()
{

}
void escolhe_intervalos_para_analise()
{

}
void escolhe_mes()
{

}
void global()
{

}
void por_pais()
{

}
void por_cidade()
{

}
void analise_por_pais()
{

}
void analise_por_cidade()
{

}
