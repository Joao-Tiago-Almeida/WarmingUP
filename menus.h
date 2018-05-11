#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"
#include "conjuntodados.h"

void menu_principal(DADOS* dados);
void menu_filtragem_de_dados(DADOS* dados);
void menu_historico_de_temperaturas(DADOS* dados);
void menu_analise_da_temperatura_por_ano(DADOS* dados);
void menu_analise_da_temperatura_global();

void opcao_limpa_criteritos(CRITERIOS_FILTRAGEM *criterios);
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios);
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios);

void historico_de_temperaturas_global(int periodo);
void historico_de_temperaturas_por_pais(int periodo);
void historico_de_temperaturas_por_cidade(int periodo);
void analise_por_pais(int ano);
void analise_por_cidade(int ano);


#endif
