#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"
#include "conjuntodados.h"

void menu_principal(DADOS* dados);
void menu_filtragem_de_dados(DADOS* dados);
void menu_historico_de_temperaturas(DADOS* dados);
void menu_analise_da_temperatura_por_ano(DADOS* dados);
void menu_analise_da_temperatura_global(DADOS* dados);

void opcao_limpa_criteritos(CRITERIOS_FILTRAGEM *criterios);
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios, DADOS *dados);
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios);

void calc_medias_de_intervalos(int numIntervalos, float *tempMed, int *numDados);
void imprime_intervalos(DADOS* dados, int numIntervalos, float *tempMin,
    float *tempMax, float *tempMed, int *numDados, int periodo);
void historico_de_temperaturas(DADOS *dados, int periodo, bool porPais, bool porCidade);

void analise_por_pais_ou_cidade(DADOS* dados, int ano, bool porCidade);




#endif
