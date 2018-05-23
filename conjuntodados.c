#include "conjuntodados.h"
#include <stdio.h>
#include <stdlib.h>

#define FLOAT_MAX   10000

#define DEZEMBRO 12

void dados_init(DADOS *dados)
{
    limpar_criterios(&dados->criterios);
    dados->headCountriesOriginal = NULL;
    dados->headCountriesFiltrada = NULL;
    dados->headCitiesOriginal = NULL;
    dados->headCitiesFiltrada = NULL;
    dados->countriesAnoMin = __INT32_MAX__;
    dados->countriesAnoMax = -__INT32_MAX__;
    dados->citiesAnoMin = __INT32_MAX__;
    dados->citiesAnoMax = -__INT32_MAX__;
    dados->citiesTempMax = -FLOAT_MAX;
    dados->citiesTempMin = FLOAT_MAX ;
    printf("---DADOS INIT---\n");
}

void dados_free(DADOS* dados) {
    if(dados->headCountriesOriginal != NULL) {
        //Caso o ficheiro dos paises tenha sido lido (no modo gráfico não é)
        dados_apaga_listaFiltrada(dados);
        dados->headCountriesFiltrada = NULL;

        //Apaga a lista e os dados também
        // (true no remove_nodes faz free dos payload)
        remove_nodes(dados->headCountriesOriginal, true);
        free(dados->headCountriesOriginal);
        dados->headCountriesOriginal = NULL;
    }

    if(dados->headCitiesOriginal != NULL)
    {
        dados_apaga_listaFiltrada(dados);
        //em prol de se fazer free anteriormente , aponta pata NULL
        dados->headCitiesFiltrada = NULL;

        //Apaga a lista e os dados também
        // (true no remove_nodes faz free dos payload)
        remove_nodes(dados->headCitiesOriginal, true);
        free(dados->headCitiesOriginal);
        dados->headCitiesOriginal = NULL;
    }
}

void dados_apaga_listaFiltrada(DADOS* dados) {
    //TODO filtrar lista das cidades
    if (dados->headCountriesFiltrada != dados->headCountriesOriginal)
    {
        //Caso exista lista filtrada (a lista filtrada não aponta para a original)
        printf("[DEBUG] Apagar lista filtrada\n");
        remove_nodes(dados->headCountriesFiltrada, false);
        free(dados->headCountriesFiltrada);
        dados->headCountriesFiltrada = dados->headCountriesOriginal;
    }
    else if (dados->headCitiesFiltrada != dados->headCitiesOriginal)
    {
        //Caso exista lista filtrada (a lista filtrada não aponta para a original)
        printf("[DEBUG] Apagar lista filtrada\n");
        remove_nodes(dados->headCitiesFiltrada, false);
        free(dados->headCitiesFiltrada);
        dados->headCitiesFiltrada = dados->headCitiesOriginal;
    }
    else
    {
        printf("[DEBUG] Lista filtrada nao existe\n");
    }
}

//TODO vv nao sei se é relevante passar por referencia (perguntar) vv
//novos_criterios passados por referencia só por questoes de performance
// (para nao ter de fazer uma copia da estrutura).
void dados_aplicar_novos_criterios(DADOS *dados, CRITERIOS_FILTRAGEM *novos_criterios)
{

    //Apaga a lista filtrada
    //E coloca a listaFiltrada a apontar para a listaOriginal
    dados_apaga_listaFiltrada(dados);

    //Caso hajam criterios aplicados, cria uma nova lista
    if (novos_criterios->filtraPorIntervalo || novos_criterios->filtraPorMeses)
    {
        //Caso a lista filtrada nao exista e é para aplicar criterios

        printf("[DEBUG] Aplicar criterios\n");

        //criação da nova lista de paises
        criacao_lista_filtrada(dados, true, novos_criterios);
        //criação da nova lista de cidades
        criacao_lista_filtrada(dados, false, novos_criterios);
    }

    dados->criterios = *novos_criterios;
}

void criacao_lista_filtrada(DADOS * dados, bool lista_paises, CRITERIOS_FILTRAGEM *novos_criterios)
{
    list_node_t *aux = NULL;

    //Cria novas entradas na lista filtrada
    if(lista_paises)    dados->headCountriesFiltrada = create_list();
    else    dados->headCitiesFiltrada = create_list();

    aux = lista_paises ? dados->headCountriesOriginal->next : dados->headCitiesOriginal->next;
    while (aux != NULL)
    {
        //Caso cumpra os criterios copia para a lista filtrada
        if (cumpre_criterios(aux->payload, novos_criterios))
        {
            insert_node(lista_paises ? dados->headCountriesFiltrada : dados->headCitiesFiltrada, aux->payload);
        }
        aux = aux->next;
    }
}

//TODO confirmar isto tudo
bool cumpre_criterios(dados_temp* valor, CRITERIOS_FILTRAGEM *criterios)
{
    if (criterios->filtraPorIntervalo &&
        (valor->dt.ano < criterios->intervaloAnoMin ||
        (valor->dt.ano == criterios->intervaloAnoMin && valor->dt.mes < criterios->intervaloMesMin)))
    {
        //Exclui aqueles cujo ano é menor que o minimo ou os cujo ano é igual mas o mes menor
        return false;
    }

    if (criterios->filtraPorMeses && criterios->mesesSelecionados[valor->dt.mes])
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
    criterios->intervaloAnoMin = 0;
    criterios->intervaloMesMin = 0;
    //Mete todos os meses a não selecionados
    for(int i = 0; i<DEZEMBRO; i++) {
        criterios->mesesSelecionados[i] = false;
    }
}
