#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define NORTE 0
#define SUL 1
#define ESTE 3
#define OESTE 2

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

	float tempAnterior; //Serve para o modo gráfico
} dados_temp;

typedef struct {
    char paisOuCidade[100];
    float tempMed;
    int numDados; //Numero de meses analisados
    float tempMin;
    float tempMax;
    float tempAmplitude;
} DADOS_ANALISE_POR_ANO;

typedef struct {

	int nr_de_dados_mes;
	float soma_dos_dados;
	float media_das_temp_por_mes;

} DADOS_MOVING_AVERAGE;
#endif
