#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

//Esta função cria uma nova node que tem como payload uma copia do parametro dados
list_node_t *create_node(dados_temp *_dados)
{
    list_node_t *new_node = NULL;
    new_node = (list_node_t *)malloc(sizeof(list_node_t));

    if (new_node == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    new_node->payload = _dados;

    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

//Cria a lista (criando uma dummy node e devolvendo o seu endereço)
list_node_t *create_list()
{
    dados_temp d;
    return create_node(&d);
}

//Esta função cria uma nova node que tem como payload uma copia do parametro dados
void insert_node(list_node_t *_head, dados_temp *_dados)
{
    list_node_t *new_node = create_node(_dados);

    // inserir na lista sempre apos o dummy node
    new_node->prev = _head;
    new_node->next = _head->next;
    _head->next = new_node;

    if (new_node->next != NULL)
    {
        new_node->next->prev = new_node;
    }
}

/**
 * Imprime a lista
**/
void print_list(list_node_t *_head)
{
    int size = 0;
    printf("function:: print_list\n");
    list_node_t *aux = _head->next;
    while (aux != NULL)
    {
        printf("%d-%d-%d,%s,%f\n", aux->payload->dt.dia,
            aux->payload->dt.mes, aux->payload->dt.ano,
            aux->payload->pais, aux->payload->temp);
        size++;
        aux = aux->next;
    }

    printf("Size: %d", size);
}

/**
 * Apaga todas as nodes da lista (excepto a dummy node)
 * freePayload: faz free dos payloads
**/
int remove_nodes(list_node_t *_head, bool freePayload)
{
    list_node_t *aux = NULL, *tmp = NULL;

    aux = _head->next; // start with node after dummy node
    if (aux == NULL)
    {
        return -1;
    }
    while (aux != NULL)
    {
        tmp = aux->next;
        if (aux->next != NULL)
        {
            aux->next->prev = aux->prev;
        }
        aux->prev->next = aux->next;
        if(freePayload) {
            free(aux->payload);
        }
        free(aux);
        aux = tmp;
    }
    return 0;
}

//Esta função cria uma nova node que tem como payload uma copia do parametro dados
//  e insere-a depois da tail, devolvendo um apontador para essa nova node, que vai ser
//  a nova tail
list_node_t *insert_node_by_tail(list_node_t *_tail, dados_temp *_dados)
{
    list_node_t *new_node = create_node(_dados);

    new_node->prev = _tail;
    new_node->next = NULL;
    _tail->next = new_node;

    return new_node;
}
