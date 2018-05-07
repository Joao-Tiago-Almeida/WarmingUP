#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

#define NORTE 0
#define SUL 1
#define ESTE 2
#define OESTE 3

void menuA() {
	while(1) {
		printf("1 para voltar a trás\n");
		int c = getchar();
		if(c == '1') {
			return;
		} else if(c != '\r') {
			printf("opcao invalida\n");
		}
	}
}

void menuPrincipal() {
	while(1) {
		printf("1 para menu A\n");
		int c = getchar();
		if(c == '1') {
			menuA();
		} else if(c != '\r') {
			printf("opcao invalida\n");
		}
	}
}

int main(void) {
    menuPrincipal();
    return 0;
}