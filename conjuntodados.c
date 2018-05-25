#include "conjuntodados.h"
#include <stdio.h>
#include <stdlib.h>

#define FLOAT_MAX   10000

#define DEZEMBRO 12

/**
 * Inicializa os dados do programa
 */
void dados_init(DADOS *dados)
{
    limpar_criterios(&dados->criterios);
    dados->headCountriesOriginal = NULL;
    dados->headCountriesFiltrada = NULL;
    dados->tailCountriesFiltrada = NULL;
    dados->headCitiesOriginal = NULL;
    dados->headCitiesFiltrada = NULL;
    dados->tailCitiesFiltrada = NULL;
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = -__INT32_MAX__;
    dados->citiesAnoMin = __INT32_MAX__;
    dados->citiesAnoMax = -__INT32_MAX__;
    dados->citiesTempMax = -FLOAT_MAX;
    dados->citiesTempMin = FLOAT_MAX;
}

/**
 * Limpa os dados do programa (incluindo as listas)
 */
void dados_free(DADOS* dados) {
    bool apagaListaCountries = dados->headCountriesOriginal != NULL;
    bool apagaListaCities = dados->headCitiesOriginal != NULL;

    //Apaga as duas listas
    dados_apaga_listaFiltrada(dados);
    dados->headCountriesFiltrada = NULL;
    dados->headCitiesFiltrada = NULL;

    if(apagaListaCountries) {
        //Caso o ficheiro dos paises tenha sido lido (no modo gráfico não é)
        
        //Apaga a lista e os dados também
        // (true no remove_nodes faz free dos payload)
        remove_nodes(dados->headCountriesOriginal, true);
        free(dados->headCountriesOriginal);
        dados->headCountriesOriginal = NULL;
    }

    if(apagaListaCities)
    {
        //Apaga a lista e os dados também
        // (true no remove_nodes faz free dos payload)
        remove_nodes(dados->headCitiesOriginal, true);
        free(dados->headCitiesOriginal);
        dados->headCitiesOriginal = NULL;
    }
}

/**
 * Apaga as listas filtradas (cidades e paises)
**/
void dados_apaga_listaFiltrada(DADOS* dados) {
    if (dados->headCountriesFiltrada != dados->headCountriesOriginal)
    {
        //Caso exista lista filtrada (a lista filtrada não aponta para a original)
        remove_nodes(dados->headCountriesFiltrada, false);
        free(dados->headCountriesFiltrada); //Free da dummy node
        dados->headCountriesFiltrada = dados->headCountriesOriginal;
        dados->tailCountriesFiltrada = NULL;
    }
    
    if (dados->headCitiesFiltrada != dados->headCitiesOriginal)
    {
        //Caso exista lista filtrada (a lista filtrada não aponta para a original)
        remove_nodes(dados->headCitiesFiltrada, false);
        free(dados->headCitiesFiltrada); //Free da dummy node
        dados->headCitiesFiltrada = dados->headCitiesOriginal;
        dados->tailCitiesFiltrada = NULL;
    }
}

/**
 * Apaga as listas filtradas(cidades e paises) e, caso se esteja
 *      a filtrar por novos critérios cria novas listas filtradas
**/
void dados_aplicar_novos_criterios(DADOS *dados, CRITERIOS_FILTRAGEM *novos_criterios)
{
    //Apaga a lista filtrada
    //E coloca a listaFiltrada a apontar para a listaOriginal
    dados_apaga_listaFiltrada(dados);

    //Caso hajam criterios aplicados, cria uma nova lista
    if (novos_criterios->filtraPorIntervalo || novos_criterios->filtraPorMeses)
    {
        //Caso a lista filtrada nao exista e é para aplicar criterios

        //criação da nova lista de paises
        criacao_lista_filtrada(dados, true, novos_criterios);
        //criação da nova lista de cidades
        criacao_lista_filtrada(dados, false, novos_criterios);
    }

    dados->criterios = *novos_criterios;
}

/**
 * Cria uma lista com os payloads da lista original que cumprem os critérios de filtragem
 * lista_paises: se a lista a criar é relativa à lista dos paises, ou à lista das cidades
**/
void criacao_lista_filtrada(DADOS * dados, bool lista_paises, CRITERIOS_FILTRAGEM *novos_criterios)
{
    list_node_t *aux = NULL;

    //Cria novas entradas na lista filtrada
    if(lista_paises)
    {
        dados->headCountriesFiltrada = create_list();
        dados->tailCountriesFiltrada = dados->headCountriesFiltrada;
    }
    else
    {
        dados->headCitiesFiltrada = create_list();
        dados->tailCitiesFiltrada = dados->headCitiesFiltrada;
    }

    aux = lista_paises ? dados->headCountriesOriginal->next : dados->headCitiesOriginal->next;
    while (aux != NULL)
    {
        //Caso cumpra os criterios copia para a lista filtrada
        if (cumpre_criterios(aux->payload, novos_criterios))
        {
            list_node_t *new_tail = insert_node_by_tail(lista_paises ? dados->tailCountriesFiltrada : dados->tailCitiesFiltrada, aux->payload);
            if(lista_paises)
            {
                dados->tailCountriesFiltrada = new_tail;
            }
            else
            {
                dados->tailCitiesFiltrada = new_tail;
            }
        }
        aux = aux->next;
    }
}

/**
 * Devolve true se um dados_temp cumpre com os critérios de filtragem
**/
bool cumpre_criterios(dados_temp* valor, CRITERIOS_FILTRAGEM *criterios)
{
    if (criterios->filtraPorIntervalo &&
        (valor->dt.ano < criterios->intervaloAnoMin ||
        (valor->dt.ano == criterios->intervaloAnoMin && valor->dt.mes < criterios->intervaloMesMin)))
    {
        //Exclui aqueles cujo ano é menor que o minimo ou os cujo ano é igual mas o mes menor
        return false;
    }

    if (criterios->filtraPorMeses && !criterios->mesesSelecionados[valor->dt.mes-1])
    {
        //Exclui pelo mes
        return false;
    }

    return true;
}

/**
 * Limpa os critérios de filtragem
**/
void limpar_criterios(CRITERIOS_FILTRAGEM *criterios)
{
    criterios->filtraPorMeses = false;
    criterios->filtraPorIntervalo = false;
    criterios->intervaloAnoMin = 0;
    criterios->intervaloMesMin = 0;
    //Mete todos os meses a não selecionados
    for(int i = 0; i<DEZEMBRO; i++) {
        criterios->mesesSelecionados[i] = false;
    }
}
