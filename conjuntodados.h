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
    int countriesSize;
    list_node_t* headCountriesOriginal;
    list_node_t* headCountriesFiltrada;

    //TODO colocar também a lista de cidades
    
    CRITERIOS_FILTRAGEM criterios;
} DADOS;

void dados_init(DADOS* dados);
void dados_aplicar_novos_criterios(DADOS* dados, CRITERIOS_FILTRAGEM *novos_criterios);
bool cumpre_criterios(dados_temp* valor, CRITERIOS_FILTRAGEM *criterios);

void limpar_criterios(CRITERIOS_FILTRAGEM *criterios);

#endif
