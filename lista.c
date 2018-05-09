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

    new_node->payload = *_dados;

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

//Esta função cria uma nova node que tem como payload uma copia do parametro dados  ??? MIGUEL  ???
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

void print_list(list_node_t *_head)
{
    printf("function:: print_list\n");
    list_node_t *aux = _head->next;
    printf("merda\n");
    while (aux != NULL)
    {
        printf("País:: %s\n", aux->payload.pais);
        printf("Temperatura média:: %f\n", aux->payload.temp);
        printf("Incerteza:: %f\n", aux->payload.incerteza);
        printf("Data da medição (dd-mm-aaaa):: %d-%d-%d\n\n", aux->payload.dt.dia,
            aux->payload.dt.mes, aux->payload.dt.ano);

        aux = aux->next;
    }
}

int remove_nodes(list_node_t *_head)
{
    list_node_t *aux = NULL;//, *tmp = NULL;

    aux = _head->next; // start with node after dummy node

    if (aux == NULL)
    {
        return -1;
    }

    while (aux != NULL)
    {
        /*if (strlen(aux->name) == 0) // node with an empty string
        {
            tmp = aux->next;

            if (aux->next != NULL)
            {
                aux->next->prev = aux->prev;
            }
            aux->prev->next = aux->next;

            free(aux->name);
            free(aux);
            aux = tmp;
        }
        else
        {*/
        aux = aux->next;
        //}
    }
    return 0;
}
