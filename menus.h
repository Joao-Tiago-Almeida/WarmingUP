#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"
#include "conjuntodados.h"

bool menu_principal(DADOS* dados);
void menu_filtragem_de_dados(DADOS* dados);
void menu_historico_de_temperaturas(DADOS* dados);
void menu_analise_da_temperatura_por_ano(DADOS* dados);
void menu_analise_da_temperatura_global(DADOS* dados);

void opcao_limpa_criteritos(CRITERIOS_FILTRAGEM *criterios);
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios, DADOS *dados);
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios);

void m2_calc_medias_de_intervalos(int numIntervalos, DADOS_HISTORICO* temps);
void m2_imprime_intervalos(DADOS* dados, int numIntervalos, DADOS_HISTORICO* temps, int periodo, bool porCidade);
void m2_colocaPaisOuCidadeEmVetor(char ***vect, int* vectSize, int* num, char* nome);
void m2_init_dados_historico(DADOS_HISTORICO* a);
int m2_CalculaSumMaxEMin(DADOS* dados, int filtra, char paisOuCidade[100], int periodo,
    DADOS_HISTORICO *temps, char ***cidOuPaisFound);
void historico_de_temperaturas(DADOS *dados, int filtra);

void analise_por_pais_ou_cidade(DADOS* dados, bool porCidade);

void m4_calculo_MA(int M, DADOS *dados, bool porPais, bool porCidade);
float M4_CalculoAumentoTemp(DADOS *dados, float *vect, int ano, bool porCidade);
float M4_MediaAno(int ano_a_analisar, bool porPais, bool porCidade, char paisOuCidade[100], int M, list_node_t *aux);
float m4_moving_average(int M, float* vect_temp, bool* vect_mes_tem_dados);

#endif
