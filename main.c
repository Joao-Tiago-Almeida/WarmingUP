#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "functions.h"

int main ( void )
{
  int ano_a_analisar = -1, periodo_a_analisar = -1;

  menu_principal(&periodo_a_analisar, &ano_a_analisar);

  printf("END\n");

  return EXIT_SUCCESS;
}
