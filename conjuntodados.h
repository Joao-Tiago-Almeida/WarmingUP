#ifndef _CONJUNTO_DADOS_H
#define _CONJUNTO_DADOS_H

#include <stdbool.h>
#include "lista.h"

typedef struct {
    bool filtraPorIntervalo;
    int intervaloMesMin, intervaloAnoMin;

    bool filtraPorMeses;
    int mesMin, mesMax;
} CRITERIOS_FILTRAGEM;

typedef struct {
    int countriesListSize;
    list_node_t* headCountriesOriginal;
    //Aponta para a dummy node de uma nova lista caso hajam critérios de filtragem
    // ou é igual a headCountriesOriginal caso não hajam criterios de filtragem
    list_node_t* headCountriesFiltrada;
    int countriesAnoMin, countriesAnoMax;
    int countriesMesMin;

    //TODO colocar também a lista de cidades
    
    CRITERIOS_FILTRAGEM criterios;
} DADOS;

void dados_init(DADOS* dados);
void dados_free(DADOS* dados);
void dados_apaga_listaFiltrada(DADOS* dados);
void dados_aplicar_novos_criterios(DADOS* dados, CRITERIOS_FILTRAGEM *novos_criterios);
int dados_get_mesmin_para_ano(DADOS* dados);

bool cumpre_criterios(dados_temp* valor, CRITERIOS_FILTRAGEM *criterios);
void limpar_criterios(CRITERIOS_FILTRAGEM *criterios);

#endif
