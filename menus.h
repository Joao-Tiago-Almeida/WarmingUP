#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"
#include "criteriosfiltragem.h"

void menu_principal(criterios_filtragem *criterios);
void menu_filtragem_de_dados(criterios_filtragem *criterios);
void menu_historico_de_temperaturas(criterios_filtragem *criterios);
void menu_analise_da_temperatura_por_ano(criterios_filtragem *criterios);
void menu_analise_da_temperatura_global();

void opcao_limpa_criteritos(criterios_filtragem *criterios);
void opcao_escolhe_intervalos_para_analise(criterios_filtragem *criterios);
void opcao_escolhe_mes(criterios_filtragem *criterios);

void historico_de_temperaturas_global(int periodo);
void historico_de_temperaturas_por_pais(int periodo);
void historico_de_temperaturas_por_cidade(int periodo);
void analise_por_pais(int ano);
void analise_por_cidade(int ano);


#endif
