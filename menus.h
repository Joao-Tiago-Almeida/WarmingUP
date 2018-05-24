#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"
#include "conjuntodados.h"

int perguntar_menu_choice();
void getstring(char* string, char _comentario[]);
char pergunta_tecla(char* comentario, char opcao1, char opcao2);

bool menu_principal(DADOS* dados);
void menu_1_filtragem_de_dados(DADOS* dados);
void menu_2_historico_de_temperaturas(DADOS* dados);
void menu_3_analise_da_temperatura_por_ano(DADOS* dados);
void menu_4_analise_da_temperatura_global(DADOS* dados);

void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios, DADOS *dados);
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios);

int m2_calculo_num_intervalos(int _periodo, DADOS *dados, bool porCidade);
void m2_calc_medias_de_intervalos(int numIntervalos, DADOS_HISTORICO* temps);
void m2_imprime_intervalos(DADOS* dados, int numIntervalos, DADOS_HISTORICO* temps, int periodo, bool porCidade);
void m2_colocaPaisOuCidadeEmVetor(char ***vect, int* vectSize, int* num, char* nome);
void m2_init_dados_historico(DADOS_HISTORICO* a);
int m2_CalculaSumMaxEMin(DADOS* dados, int filtra, char paisOuCidade[100], int periodo,
    DADOS_HISTORICO *temps, char ***cidOuPaisFound);
void m2_historico_de_temperaturas(DADOS *dados, int filtra);

void m3_analise_por_pais_ou_cidade(DADOS* dados, bool porCidade);
void m3_dados_analise_por_pais_init(DADOS_ANALISE_POR_ANO* dados);
int m3_calculo_dados_por_pais_ou_cidade_num_ano(DADOS* dados, int ano, bool porCidade, DADOS_ANALISE_POR_ANO **dadosPorPais);
DADOS_ANALISE_POR_ANO **m3_calcula_topN(int n, DADOS_ANALISE_POR_ANO *dadosPorPais, int numEntries, int mode);

void m4_calculo_MA(int M, DADOS *dados, int filtra);
float M4_CalculoAumentoTemp(DADOS *dados, float *vect, int ano, bool porCidade);
float M4_MediaAno(int ano_a_analisar, int filtra, char paisOuCidade[100], int M, list_node_t *aux);
float m4_moving_average(int M, float* vect_temp, bool* vect_mes_tem_dados);

#endif
