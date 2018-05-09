#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"
#include "lista.h"
#include "menus.h"
#include "ficheiros.h"


int main ( void )
{
    int ano_a_analisar = -1, periodo_a_analisar = -1;
    int size_countries_file = 0;
    list_node_t* headListaDados = NULL;

    size_countries_file = read_file_countries (&headListaDados);
    print_list(headListaDados);

    headListaDados = insertionSort(&headListaDados);

    //print_list(headListaDados);




    menu_principal(&periodo_a_analisar, &ano_a_analisar);


    return EXIT_SUCCESS;
}
