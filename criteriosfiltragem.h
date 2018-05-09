#ifndef _CRITERIOS_FILTRAGEM_H
#define _CRITERIOS_FILTRAGEM_H

#include <stdbool.h>

typedef struct {
    bool filtraPorIntervalo;
    int intervaloMesMin, intervaloAnoMin;

    bool filtraPorMeses;
    int mesMin, mesMax;
} criterios_filtragem;

void limpar_criterios(criterios_filtragem *criterios);

#endif