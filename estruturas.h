#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define NORTE 0
#define SUL 1
#define ESTE 2
#define OESTE 3

typedef struct {
	int dia;
	int mes;
	int ano;
} data;

typedef struct {
	float angular;
	int direcao; //Norte, sul este ou oeste
} geo_coord;

typedef struct {
	data dt;
	float temp;
	float incerteza;
	char pais[100];
	char cidade[100];
	geo_coord latitude;
	geo_coord longitude;
} dados_temp;

#endif
