#ifndef _MENUS_H
#define _MENUS_H

#include "estruturas.h"

void menu_principal(int*, int*);
void menu_filtragem_de_dados(int*, int* );
void menu_historico_de_temperaturas(int*, int*);
void menu_analise_da_temperatura_por_ano(int*, int*);
void menu_analise_da_temperatura_global();
void limpa_criteritos();
void escolhe_intervalos_para_analise();
void escolhe_mes();
void historico_de_temperaturas_global(int periodo);
void historico_de_temperaturas_por_pais(int periodo);
void historico_de_temperaturas_por_cidade(int periodo);
void analise_por_pais(int ano);
void analise_por_cidade(int ano);


#endif
