#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "lista.h"
#include "menus.h"
#include <string.h>



int main ( void )
{
  int ano_a_analisar = -1, periodo_a_analisar = -1;
  list_node_t* headListaDados = NULL;

  headListaDados = create_list();

  //Teste de como criar elementos na lista
  for(int i = 0; i<2; i++) {
      dados_temp a;
      a.dt.ano = 1;
      a.dt.mes = 1;
      a.dt.dia = 1;
      a.temp = 10.0f;
      a.incerteza = 1.0f;
      strcpy(a.pais, "Portugal");
      strcpy(a.cidade, "Santarem");
      a.latitude.angular = 1.0f;
      a.latitude.direcao = NORTE;
      insert_node(headListaDados, &a);
  }
  print_list(headListaDados);

  menu_principal(&periodo_a_analisar, &ano_a_analisar);

  printf("END\n");

  return EXIT_SUCCESS;
}
