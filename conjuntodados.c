#include "conjuntodados.h"
#include <stdio.h>
#include <stdlib.h>

void dados_init(DADOS *dados)
{
    limpar_criterios(&dados->criterios);
    dados->headCountriesOriginal = NULL;
    dados->headCountriesFiltrada = NULL;
}

//TODO vv nao sei se é relevante passar por referencia (perguntar) vv
//novos_criterios passados por referencia só por questoes de performance
// (para nao ter de fazer uma copia da estrutura).
void dados_aplicar_novos_criterios(DADOS *dados, CRITERIOS_FILTRAGEM *novos_criterios)
{
    int debug_inseridos = 0;
    if (!novos_criterios->filtraPorIntervalo && !novos_criterios->filtraPorMeses)
    {
        //Caso deixem de haver criterios de filtragem
        //Limpa a lista headCountriesFiltrada (apaga todos os nodes e a dummy node)
        if (dados->headCountriesFiltrada != NULL)
        {
            printf("[DEBUG] Apagar lista filtrada\n");
            remove_nodes(dados->headCountriesFiltrada, false);
            free(dados->headCountriesFiltrada);
            dados->headCountriesFiltrada = NULL;
        }
        else
        {
            printf("[DEBUG] Lista filtrada nao existe\n");
        }
    }
    else if (dados->headCountriesFiltrada == NULL)
    {
        //Caso a lista filtrada nao exista e é para aplicar criterios
        list_node_t *aux = NULL;
        
        printf("[DEBUG] Aplicar criterios\n");

        //Cria novas entradas na lista filtrada
        dados->headCountriesFiltrada = create_list();
        aux = dados->headCountriesOriginal->next;
        
        while (aux != NULL)
        {
            //Caso cumpra os criterios copia para a lista filtrada
            if (cumpre_criterios(aux->payload, novos_criterios)) {
                printf("%d\t", debug_inseridos);
                sortedInsert(dados->headCountriesFiltrada, aux->payload);
                debug_inseridos++;
            }
            aux = aux->next;
        }
    }
    else
    {
        printf("!!!NAO IMPLEMENTADO!!!\n");
    }

    dados->criterios = *novos_criterios;
}

bool cumpre_criterios(dados_temp* valor, CRITERIOS_FILTRAGEM *criterios)
{
    if (criterios->filtraPorIntervalo &&
        (valor->dt.ano < criterios->intervaloAnoMin ||
        (valor->dt.ano == criterios->intervaloAnoMin && valor->dt.mes < criterios->intervaloMesMin)))
    {
        //Exclui aqueles cujo ano é menor que o minimo ou os cujo ano é igual mas o mes menor
        return false;
    }

    if (criterios->filtraPorMeses &&
        (valor->dt.mes < criterios->mesMin || valor->dt.mes > criterios->mesMax))
    {
        //Exclui aqueles cujo mes é menor que o mínimo ou maior que o máximo
        return false;
    }

    return true;
}

void limpar_criterios(CRITERIOS_FILTRAGEM *criterios)
{
    criterios->filtraPorMeses = false;
    criterios->filtraPorIntervalo = false;
}