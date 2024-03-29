#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

#include "estruturas.h"

typedef struct list_node
{
    dados_temp* payload;
    struct list_node *next;
    struct list_node *prev;
} list_node_t;

list_node_t *create_node(dados_temp *dados);
list_node_t *create_list();
void insert_node(list_node_t *_head, dados_temp *_dados);
void print_list(list_node_t *_head);
int remove_nodes(list_node_t *_head, bool freePayload);
list_node_t *insert_node_by_tail(list_node_t *_tail, dados_temp *_dados);


#endif
