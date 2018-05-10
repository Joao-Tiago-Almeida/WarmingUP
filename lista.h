#ifndef LISTA_H
#define LISTA_H

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
int remove_nodes(list_node_t *_head);
void sortedInsert(list_node_t *_head, list_node_t *_new_node);


#endif
