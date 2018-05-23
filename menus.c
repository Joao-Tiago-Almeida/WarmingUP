#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "menus.h"
#include "estruturas.h"
#include "util.h"
#include "modografico.h"

#define BUFFER_SIZE 100
#define NUM_INTERVALOS_POR_PAGINA 20
#define JANEIRO 1
#define DEZEMBRO 12
#define MAX_M 100 //não sei , perguntar ao stor
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
#define MODO_ANALISE_MAIS_QUENTE 0
#define MODO_ANALISE_MAIS_FRIO 1
#define MODO_ANALISE_EXTREMOS 2
#define NR_DE_MESES 12
#define NR_DE_ANOS_A_ESTUDAR 5
#define ANO_A_ESTUDAR_1860 1860
#define ANO_A_ESTUDAR_1910 1910
#define ANO_A_ESTUDAR_1960 1960
#define ANO_A_ESTUDAR_1990 1990
#define ANO_A_ESTUDAR_2013 2013

#define GLOBAL 0
#define POR_PAIS 1
#define POR_CIDADE 2

//TODO
//#define LIMITE_ANOS_STRING
//#define DOUBLECAT(a,b,c) a##0-2018##c

//Le o que o utilizador escreveu e devolve o numero da alinea. Caso seja invalido devolve -1
//TODO qual é o xxxh destas funções?
int perguntar_menu_choice()
{
    int alinea = 0;
    char buf[BUFFER_SIZE];

    fgets(buf, BUFFER_SIZE, stdin);
    if (sscanf(buf, "%d", &alinea) == 0)
    {
        //TODO veriricar se o utilizador escrever algo do genero de "2 5" (isto deveria dar invalido)
        alinea = -1;
    }
    return alinea;
}

//esta função permite selecionar um ano a analisar
//um ano apartir do qual se vai analisar
//ou um numero de meses a analisar
int perguntar_referencia_a_analisar(int _min, int _max, char *_comentario)
{
    char buf[BUFFER_SIZE];
    int _ano = 0;
    bool primeiraTentativa = true;

    do
    {
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }
        printf("%s [%d-%d]:\t", _comentario, _min, _max);
        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &_ano) != 1)
        {
            _ano = -1;
        }
        primeiraTentativa = false;
    } while (_ano < _min || _max < _ano);

    printf("\n");
    return _ano;
}

char pergunta_tecla(char* comentario, char opcao1, char opcao2) {
    char buffer[BUFFER_SIZE];
    do {
        printf(comentario);
        fgets(buffer, BUFFER_SIZE, stdin);
        removeLastCharIfMatch(buffer, '\n');
    } while(strlen(buffer) > 1 || (buffer[0] != opcao1 && buffer[0] != opcao2));

    return buffer[0];
}

void getstring(char* pais, char _comentario[]) {
    char buffer[BUFFER_SIZE];

    printf("%s ", _comentario);

    fgets(buffer, BUFFER_SIZE, stdin);
    removeLastCharIfMatch(buffer, '\n');
    strcpy(pais, buffer);
}

// Devolve o numero de intervalos quando se separa os dados em intervalos(com um certo periodo)
int calculo_num_intervalos(int _periodo, DADOS *dados, bool porCidade)
{
    //TODO isto apenas considera os min e max dos countries
    int intervalos = 0;
    if(porCidade) {
        intervalos = (dados->citiesAnoMax - dados->citiesAnoMin + 1)/_periodo;
    } else {
        intervalos = (dados->countriesAnoMax - dados->countriesAnoMin + 1)/_periodo;
    }

    if((porCidade && (dados->citiesAnoMax - dados->citiesAnoMin + 1) % _periodo > 0) ||
        (!porCidade && (dados->countriesAnoMax - dados->countriesAnoMin + 1) % _periodo > 0)) {
        //Caso os intervalor não dividam exatamente os anos, vai haver mais um intervalo com menos anos que o periodo
        intervalos++;
    }
    return intervalos;
}

// Devolve false se for para sair do programa ou true se for para mudar para o modo textual
bool menu_principal(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;

    while (dentroDoMenu)
    {
        printf("\n\n\tMenu Principal\n\nEscolha uma das opções seguintes:\n");
        printf("1. Filtragem de dados;\n2. Histórico de Temperaturas;\n");
        printf("3. Análise da temperatura por ano;\n4. Análise da temperatura global;\n");
        printf("5. Modo gráfico.\n");
        printf("\n0 ou Enter. Sair do programa.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            menu_filtragem_de_dados(dados);
            break;
        case 2:
            menu_historico_de_temperaturas(dados);
            break;
        case 3:
            menu_analise_da_temperatura_por_ano(dados);
            break;
        case 4:
            menu_analise_da_temperatura_global(dados);
            break;
        case 5:
            return true; // devolve true se for para mudar para o modo textual
            break;
        case 0:
            printf("Quit\n");
            return false;
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }
    return false; // devolve false se for para sair do programa
}

void menu_historico_de_temperaturas(DADOS* dados)
{
    bool dentroDoMenu = false;
    int alinea = -1;

    printf("\n\tMenu Histórico de Temperaturas\n\n");
    do
    {
        //Quando se escolhe qualquer opção ele sai logo do menu. No entanto se for opção
        // invalida ele deve fazer loop
        dentroDoMenu = false;

        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Global: aplica-se a todos os países;\n2. Por País: aplica-se a um único país;\n");
        printf("3. Por Cidade: aplica-se a uma única cidade.\n\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            printf("\n\n\t---Histórico de Temperaturas global---\n\n");
            historico_de_temperaturas(dados, GLOBAL);
            break;
        case 2:
            printf("\n\n\t---Histórico de Temperaturas por País---\n\n");
            historico_de_temperaturas(dados, POR_PAIS);
            break;
        case 3:
            printf("\n\n\t---Histórico de Temperaturas por Cidade---\n\n");
            historico_de_temperaturas(dados, POR_CIDADE);
            break;
        case 0:
            break;
        default:
            printf("\nOpção inválida\n");
            dentroDoMenu = true;
            break;
        }
    } while (dentroDoMenu);
}
void menu_analise_da_temperatura_por_ano(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;
    printf("\n\tMenu Análise da Temperatura por Ano\n\n");

    while (dentroDoMenu)
    {
        printf("\n\nEscolha uma das opções seguintes:\n");
        printf("1. Análise por País;\n2. Análise por cidade.\n");
        printf("\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            analise_por_pais_ou_cidade(dados, false);
            dentroDoMenu = false;
            break;
        case 2:
            analise_por_pais_ou_cidade(dados, true);
            dentroDoMenu = false;
            break;
        case 0:
            dentroDoMenu = false;
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }
}

//MENU Filtragem de dados
void menu_filtragem_de_dados(DADOS* dados)
{
    bool dentroDoMenu = true;
    int alinea = -1;
    bool alterouCriterios = false;

    //Copia os critérios aplicados aos dados
    CRITERIOS_FILTRAGEM novosCriterios = dados->criterios;

    while (dentroDoMenu)
    {
        printf("\n\tMenu Filtragem de Dados\n\nEscolha uma das opções seguintes:\n");
        printf("1. Limpa critéritos;\n2. Escolhe intervalos para analise;\n");
        printf("3. Escolhe um mês.\n\n0 ou Enter. Volta ao Menu Principal.\n");

        alinea = perguntar_menu_choice();

        switch (alinea)
        {
        case 1:
            opcao_limpa_criteritos(&novosCriterios);
            alterouCriterios = true;
            break;
        case 2:
            opcao_escolhe_intervalos_para_analise(&novosCriterios, dados);
            alterouCriterios = true;
            break;
        case 3:
            opcao_escolhe_mes(&novosCriterios);
            alterouCriterios = true;
            break;
        case 0:
            dentroDoMenu = false; //Sai do menu, voltando para o anterior
            break;
        default:
            printf("\nOpção inválida\n");
            break;
        }
    }

    //Antes de sair do menu aplica criterios
    if(alterouCriterios) {
        printf("A aplicar critérios...\n");
        dados_aplicar_novos_criterios(dados, &novosCriterios);
        printf("Concluído\n");
    }
}
void opcao_limpa_criteritos(CRITERIOS_FILTRAGEM *criterios)
{
    limpar_criterios(criterios);
}
void opcao_escolhe_intervalos_para_analise(CRITERIOS_FILTRAGEM *criterios, DADOS *dados)
{
    printf("\n\n\t---Escolhe intervalos para análise---\n\n");
    printf("Os dados anteriores ao periodo que inserir não serão considerados.\n");

    char buf[BUFFER_SIZE];
    int mes = 0, ano = 0;
    bool primeiraTentativa = true;
    char comentario[] = "Qual o ano a partir do qual pretende analisar?";

    ano = perguntar_referencia_a_analisar(dados->countriesAnoMin,
        dados->countriesAnoMax, comentario);

    do
    {
        //Caso o utilizador já tenha inserido um mes invalido
        if(!primeiraTentativa) {
            printf("Tem de estar dentro dos limites!\n");
        }

        //Caso o utilizador escolha o primerio ano, e este não tenha todos os dados
        // assumindo que para tds os outros anos têm valores seguidos
        if(ano == dados->countriesAnoMin) {
            printf("Para o ano de %d só há valores apartir do mês %d !\n", ano, dados->countriesMesMin);
        }
        printf("Qual o mes a partir do qual pretende analisar [%d-12]:\t",
            ano == dados->countriesAnoMin ? dados->countriesMesMin : 1);


        fgets(buf, BUFFER_SIZE, stdin);
        if (sscanf(buf, "%d", &mes) != 1)
        {
            mes = -1;
        }

        primeiraTentativa = false;
        //printf("%d < %d || %d < %d || %d\n", criterios->intervaloMesMin, JANEIRO_NUM, DEZEMBRO_NUM,  criterios->intervaloMesMin, !intervalo_valido);

        //Volta a pedir valores ao utilizador caso sejam inseridos valores de
        // meses invalidos, ou, caso seja o menor ano, o mes escolhido
        // pode não estar disponivel
    } while ((mes < JANEIRO || DEZEMBRO < mes)
                || (ano == dados->countriesAnoMin && mes < dados->countriesMesMin));

    criterios->filtraPorIntervalo = true;
    criterios->intervaloAnoMin = ano;
    criterios->intervaloMesMin = mes;
}
void opcao_escolhe_mes(CRITERIOS_FILTRAGEM *criterios)
{
    printf("\n\n\t---Escolha mêses---\n\n");
    printf("Apenas serão considerados os dados dos meses (delimitado por espaços)\n");

    char buf[BUFFER_SIZE];
    char *delim = " ";
    bool invalido = false;

    do
    {
        invalido = false;

        //Mete todos os meses a não selecionados
        for(int i = 0; i<DEZEMBRO; i++) {
            criterios->mesesSelecionados[i] = false;
        }

        printf("Quais os meses que pretende analisar (entre %d-%d):\t", JANEIRO, DEZEMBRO);

        fgets(buf, BUFFER_SIZE, stdin);
        removeLastCharIfMatch(buf, '\n');

        char *token = strtok(buf, delim);

        while(token != NULL) {
            int mes = atoi(token);
            if(mes >= JANEIRO && mes <= DEZEMBRO) {
                criterios->mesesSelecionados[mes-1] = true;
            } else {
                printf("Mês inválido\n");
                invalido = true;
                break;
            }
            token = strtok(NULL, delim);
        }
        //TODO pode haver intervalos de 1 mês??
    } while (invalido);

    criterios->filtraPorMeses = true;

}

void m2_calc_medias_de_intervalos(int numIntervalos, DADOS_HISTORICO* temps) {
    //Calcula as médias
    for(int i = 0; i<numIntervalos; i++) {
        if(temps[i].numDados > 0) {
            temps[i].tempMed = temps[i].tempMed/temps[i].numDados;
        }
    }
}

//TODO esta funcao tem demasiados parametros
void m2_imprime_intervalos(DADOS* dados, int numIntervalos, DADOS_HISTORICO* temps, int periodo, bool porCidade)
{
    int numPaginas = ((numIntervalos-1) / NUM_INTERVALOS_POR_PAGINA) + 1;
    int pagina = 0;

    bool dentroDoLoop = true;
    int anoMax = porCidade ? dados->citiesAnoMax : dados->countriesAnoMax;
    int anoMin = porCidade ? dados->citiesAnoMin : dados->countriesAnoMin;

    while(dentroDoLoop) {
        printf("A mostar página %d de %d\n", pagina+1, numPaginas);
        //Imprime os intervalos
        int i = pagina*NUM_INTERVALOS_POR_PAGINA;
        while(i<(pagina+1)*NUM_INTERVALOS_POR_PAGINA && i<numIntervalos)
        {
            bool intervalorFechado = false;

            //Determina quais os anos que o intervalo compreende
            int anoMenor = anoMin + i*periodo;
            int anoMaior = anoMenor + periodo;

            if(anoMaior >= anoMax) {
                //Caso seja o ultimo intervalo, mostra o intervalo como um intervalo fechado
                anoMaior = anoMax;
                intervalorFechado = true;
            }

            if(temps[i].numDados > 0) {
                if(intervalorFechado) {
                    printf("[ %d , %d ]\tMin:%.2f Max:%.2f Med:%.2f\n", anoMenor, anoMaior,
                        temps[i].tempMin, temps[i].tempMax, temps[i].tempMed);
                } else {
                    printf("[ %d , %d [\tMin:%.2f Max:%.2f Med:%.2f\n", anoMenor, anoMaior,
                        temps[i].tempMin, temps[i].tempMax, temps[i].tempMed);
                }
            } else {
                printf(intervalorFechado ?
                    "[ %d , %d ]\tSEM DADOS\n" : "[ %d , %d [\tSEM DADOS\n",
                    anoMenor, anoMaior);
            }
            i++;
        }


        if(numPaginas == 1) {
            return;
        }
        char tecla = pergunta_tecla("a para avançar, q para sair\n", 'a', 'q');
        if(tecla == 'a') {
            if(pagina >= numPaginas-1) {
                pagina = 0;
            } else {
                pagina++;
            }
        } else if(tecla == 'q') {
            dentroDoLoop = false;
        }
    }
}

//Procura um nome num vetor de string. Se não encontrar coloca-o e aumenta o num
//Se o vetor não for grande o suficiente faz realloc
void m2_colocaPaisOuCidadeEmVetor(char ***vect, int* vectSize, int* num, char* nome) {
    int prevVectSize = *vectSize;
    for(int i = 0; i<(*vectSize); i++) {
        if((*vect)[i] == NULL) {
            (*vect)[i] = nome; //Coloca no vetor
            (*num)++;
            return;
        }
        
        if(strcmp((*vect)[i], nome) == 0) {
            //Se encontrou no vetor
            return;
        }
    }

    *vectSize += 5;
    *vect = (char**) realloc(*vect, sizeof(char*) * (*vectSize));
    for(int i = prevVectSize; i<(*vectSize); i++) {
        (*vect)[i] = NULL;
    }

    //Coloca no primeiro espaço novo criado no vetor
    (*vect)[prevVectSize] = nome;
    (*num)++;
}

void m2_init_dados_historico(DADOS_HISTORICO* a) {
    a->tempMax = -__FLT_MAX__;
    a->tempMin = __FLT_MAX__;
    a->tempMed = 0;
    a->numDados = 0;
}

//Devolve o numero de paises encontrados
int m2_CalculaSumMaxEMin(DADOS* dados, int filtra, char paisOuCidade[100], int periodo,
    DADOS_HISTORICO *temps, char ***cidOuPaisFound)
{
    list_node_t *aux = NULL;
    
    //Vetor que tem a lista de cidades ou paises que coincidem com o nome pedido ao utilizador paisOuCidade
    int numoCidOuPaisFound = 0;
    int cidOuPaisFoundSize = 5;
    *cidOuPaisFound = checkedMalloc(sizeof(char*) * cidOuPaisFoundSize);
    for(int i = 0; i<cidOuPaisFoundSize; i++) {
        (*cidOuPaisFound)[i] = NULL;
    }

    int anoMin = filtra == POR_CIDADE ? dados->citiesAnoMin : dados->countriesAnoMin;
    int intervalo = 0;

    //A lista a analisar deve ser a das cidades se for porCidade. Caso contrário deve ser a dos paises
    aux = filtra == POR_CIDADE ? dados->headCitiesFiltrada->next : dados->headCountriesFiltrada->next; //->next para a node a seguir à dummy node    

    while(aux != NULL) {
        if(filtra == GLOBAL ||
            (filtra == POR_PAIS && strstr(aux->payload->pais, paisOuCidade) != NULL) ||
            (filtra == POR_CIDADE && strstr(aux->payload->cidade, paisOuCidade) != NULL))
        {
            if(filtra != GLOBAL) {
                m2_colocaPaisOuCidadeEmVetor(cidOuPaisFound,
                    &cidOuPaisFoundSize,
                    &numoCidOuPaisFound,
                    filtra == POR_CIDADE ? aux->payload->cidade : aux->payload->pais);
            }

            //Conta este nó se não tiver a filtrar por cidade ou pais, ou se
            //  corresponder ao que se está a filtrar
            
            intervalo = (aux->payload->dt.ano - anoMin) / periodo;


            temps[intervalo].tempMax = MAX(temps[intervalo].tempMax, aux->payload->temp);
            temps[intervalo].tempMin = MIN(temps[intervalo].tempMin, aux->payload->temp);
            // O tempMed vai temporariamente guardar a soma das temperaturas
            temps[intervalo].tempMed += aux->payload->temp;
            temps[intervalo].numDados++;
        }
        aux = aux->next;
    }

    return numoCidOuPaisFound;
}

void historico_de_temperaturas(DADOS *dados, int filtra) {
    DADOS_HISTORICO *temps = NULL;
    int numIntervalos = 0;

    //Vetor que tem os nomes das cidades ou paises encontrados
    int numCidOuPaisFound = 0;
    char **cidOuPaisFound = NULL;
    
    char paisOuCidade[100];
    int periodo = 0;
    char comentario[] = "Qual o periodo que pretende analisar?";

    bool pedeDeNovo = false;

    int anoMax = filtra == POR_CIDADE ? dados->citiesAnoMax : dados->countriesAnoMax;
    int anoMin = filtra == POR_CIDADE ? dados->citiesAnoMin : dados->countriesAnoMin;

    periodo = perguntar_referencia_a_analisar(1, anoMax - anoMin, comentario);

    do {
        pedeDeNovo = false;
        if(filtra != GLOBAL) {
            //Caso seja para filtrar por país ou cidade, perguntar o país ou cidade
            getstring(paisOuCidade, filtra == POR_PAIS ? "País a analisar: " : "Cidade a analisar: ");
        }

        numIntervalos = calculo_num_intervalos(periodo, dados, filtra == POR_CIDADE);

        //Cria vetores com o numero de intervalos
        temps = (DADOS_HISTORICO*) checkedMalloc(sizeof(DADOS_HISTORICO) * numIntervalos);

        for(int i = 0; i<numIntervalos; i++) {
            //Inicializa cada intervalo
            m2_init_dados_historico(&temps[i]);
        }
        
        numCidOuPaisFound = m2_CalculaSumMaxEMin(dados, filtra, paisOuCidade, periodo,
            temps, &cidOuPaisFound);
        
        if(filtra != GLOBAL) {
            if(numCidOuPaisFound == 0) {
                printf(filtra == POR_CIDADE ? "Não foi encontrada nenhuma cidade %s" : "Não foi encontrado nenhum pais %s",
                    paisOuCidade);
                pedeDeNovo = true;
                free(temps);
                free(cidOuPaisFound);
                continue; //Não imprime o historico
            } else if(numCidOuPaisFound > 1) {
                printf(filtra == POR_CIDADE ? "Foram encontradas as seguintes cidades\n" :
                        "Foram encontrados os seguintes paises\n");

                for(int i = 0; i<numCidOuPaisFound; i++) {
                    printf("%d %s\n", i+1, cidOuPaisFound[i]);
                }
                
                char tecla = pergunta_tecla("\nQuer escolher outro nome? [s/n]", 's', 'n');
                if(tecla == 's') {
                    pedeDeNovo = true;
                    free(cidOuPaisFound);
                    free(temps);
                    continue; //Não imprime o historico
                } else {

                }
            }
        }

        free(cidOuPaisFound);

        m2_calc_medias_de_intervalos(numIntervalos, temps);
        m2_imprime_intervalos(dados, numIntervalos, temps, periodo, filtra == POR_CIDADE);
        free(temps);
        
    } while(pedeDeNovo);
}

void dados_analise_por_pais_init(DADOS_ANALISE_POR_ANO* dados) {
    dados->paisOuCidade[0] = '\0';
    dados->tempMed = 0;
    dados->numDados = 0;
    dados->tempMin = __FLT_MAX__;
    dados->tempMax = 0;
}

//Devolve um o numero de entradas do vetor dadosPorPais
// aloca memória para o vetor, que deve ser libertada de seguida
int calculo_dados_por_pais_ou_cidade_num_ano(DADOS* dados, int ano, bool porCidade, DADOS_ANALISE_POR_ANO **dadosPorPais) {
    int numEntries = 0;
    list_node_t *aux = NULL;
    int vecSize = 50;
    *dadosPorPais = (DADOS_ANALISE_POR_ANO*) checkedMalloc(vecSize * sizeof(DADOS_ANALISE_POR_ANO));

    //Inicializa o vetor colocando numDados a 0, para saber que essa
    // entrada do vetor não tem dados
    for(int i = 0; i<vecSize; i++) {
        dados_analise_por_pais_init(&(*dadosPorPais)[i]);
    }

    aux = porCidade ? dados->headCitiesFiltrada->next : dados->headCountriesFiltrada->next; //->next para a node a seguir à dummy node

    //Mete o aux a apontar para a primeira node do ano que se quer analisar (a lista está ordenada por data)
    while(aux != NULL && aux->payload->dt.ano != ano) {
        aux = aux->next;
    }


    //Enquanto se estiver a percorrer a lista e ainda é relativo ao ano
    while(aux != NULL && aux->payload->dt.ano == ano) {
        DADOS_ANALISE_POR_ANO* dadosDoPaisOuCidade = NULL;
        for(int i = 0; i<vecSize; i++) {
            if((*dadosPorPais)[i].numDados == 0)
            {
                //Caso a entrada i do vetor não corresponda estaja vazia, significa que
                // não foram encontrados dados relativos a este pais/cidade
                strcpy((*dadosPorPais)[i].paisOuCidade,
                    porCidade ? aux->payload->cidade : aux->payload->pais);
                dadosDoPaisOuCidade = &(*dadosPorPais)[i];
                numEntries++;
                break;
            }
            else if(strcmp((*dadosPorPais)[i].paisOuCidade,
                porCidade ? aux->payload->cidade : aux->payload->pais) == 0)
            {
                //Caso encontre uma entrada relativa ao aux->payload->pais
                dadosDoPaisOuCidade = &(*dadosPorPais)[i];
                break;
            }
        }

        if(dadosDoPaisOuCidade == NULL) {
            //Vetor não é grande o suficiente

            //Tem o indice onde criar a nova entrada para este pais/cidade
            int sizeAnterior = vecSize;

            vecSize += 50;
            *dadosPorPais = (DADOS_ANALISE_POR_ANO*) realloc((*dadosPorPais), vecSize * sizeof(DADOS_ANALISE_POR_ANO));
            if(*dadosPorPais == NULL) {
                printf("Memory allocation failure\n");
                exit(EXIT_FAILURE);
            }

            for(int i = sizeAnterior; i<vecSize; i++) {
                dados_analise_por_pais_init(&(*dadosPorPais)[i]);
            }

            strcpy((*dadosPorPais)[sizeAnterior].paisOuCidade,
                porCidade ? aux->payload->cidade : aux->payload->pais);
            dadosDoPaisOuCidade = &(*dadosPorPais)[sizeAnterior];
            numEntries++;
        }

        dadosDoPaisOuCidade->numDados++;
        //Soma as temperaturas no tempMed, para depois dividir pelo numDados
        dadosDoPaisOuCidade->tempMed += aux->payload->temp;
        dadosDoPaisOuCidade->tempMax = MAX(dadosDoPaisOuCidade->tempMax, aux->payload->temp);
        dadosDoPaisOuCidade->tempMin = MIN(dadosDoPaisOuCidade->tempMin, aux->payload->temp);

        aux = aux->next;
    }

    //Calcula as médias e as amplitudes
    for(int i = 0; i<vecSize; i++) {
        (*dadosPorPais)[i].tempMed = (*dadosPorPais)[i].tempMed / (*dadosPorPais)[i].numDados;
        (*dadosPorPais)[i].tempAmplitude = (*dadosPorPais)[i].tempMax - (*dadosPorPais)[i].tempMin;
    }

    return numEntries;
}

//
DADOS_ANALISE_POR_ANO **calcula_topN(int n, DADOS_ANALISE_POR_ANO *dadosPorPais, int numEntries, int mode) {
    DADOS_ANALISE_POR_ANO **topN = (DADOS_ANALISE_POR_ANO **) checkedMalloc(n*sizeof(DADOS_ANALISE_POR_ANO*));

    for(int i = 0; i<n; i++) {
        topN[i] = NULL;
    }

    for(int i = 0; i<numEntries; i++) { //Vai a cada pais
        for(int j = 0; j<n; j++) {  //Vai a cada entrada do topN
            if(topN[j] == NULL ||
                (mode == MODO_ANALISE_MAIS_QUENTE && dadosPorPais[i].tempMed > topN[j]->tempMed) ||
                (mode == MODO_ANALISE_MAIS_FRIO && dadosPorPais[i].tempMed < topN[j]->tempMed) ||
                (mode == MODO_ANALISE_EXTREMOS && dadosPorPais[i].tempAmplitude > topN[j]->tempAmplitude)) {
                //Caso encontre um sítio para meter esta entrada no vetor topN

                //Passar todas as entradas seguintes para baixo
                for(int k = n-2; k>j; k--) {
                    topN[k] = topN[k-1];
                }

                topN[j] = &dadosPorPais[i];
                break;
            }
        }
    }

    return topN;
}

void analise_por_pais_ou_cidade(DADOS* dados, bool porCidade)
{
    int n = 0;
    int numEntries = 0;
    int ano = 0;
    char comentario[] = "Qual o ano a que pretende analisar?";
    char comentario2[] = "Quantos valores quer (top N)?";

    DADOS_ANALISE_POR_ANO *dadosPorPais = NULL;
    if(porCidade) {
        printf("\n\n\t---Análise por Cidade---\n\n");
    } else {
        printf("\n\n\t---Análise por País---\n\n");
    }

    if(porCidade) {
        ano = perguntar_referencia_a_analisar(dados->citiesAnoMin, dados->citiesAnoMax, comentario);
    } else {
        ano = perguntar_referencia_a_analisar(dados->countriesAnoMin, dados->countriesAnoMax, comentario);
    }

    n = perguntar_referencia_a_analisar(1, 20, comentario2);


    numEntries = calculo_dados_por_pais_ou_cidade_num_ano(dados, ano, porCidade, &dadosPorPais);

    //Vectores de apontadores para os dados que estão no dadosPorPais
    // para não estar a fazer cópias das estruturas
    DADOS_ANALISE_POR_ANO **topNMaisQuentes = calcula_topN(n,
        dadosPorPais, numEntries, MODO_ANALISE_MAIS_QUENTE);
    DADOS_ANALISE_POR_ANO **topNMaisFrios = calcula_topN(n,
        dadosPorPais, numEntries, MODO_ANALISE_MAIS_FRIO);
    DADOS_ANALISE_POR_ANO **topNExtremos = calcula_topN(n,
        dadosPorPais, numEntries, MODO_ANALISE_EXTREMOS);


    printf("\n\tTop %d países mais quentes:\n", n);
    for(int i = 0; i<n; i++) {
        if(topNMaisQuentes[i] == NULL) {
            break;
        }
        printf("%d %s: Temp média %.2f\n", i+1, topNMaisQuentes[i]->paisOuCidade,
            topNMaisQuentes[i]->tempMed);
    }

    printf("\n\tTop %d países mais frios:\n", n);
    for(int i = 0; i<n; i++) {
        if(topNMaisFrios[i] == NULL) {
            break;
        }
        printf("%d %s: Temp média %.2f\n", i+1, topNMaisFrios[i]->paisOuCidade,
            topNMaisFrios[i]->tempMed);
    }

    printf("\n\tTop %d países com temperaturas mais extremas:\n", n);
    for(int i = 0; i<n; i++) {
        if(topNExtremos[i] == NULL) {
            break;
        }
        printf("%d %s: Temp média %.2f\tAmplitude %.2f\n", i+1, topNExtremos[i]->paisOuCidade,
            topNExtremos[i]->tempMed,
            topNExtremos[i]->tempAmplitude);
    }

    free(topNMaisQuentes);
    free(topNMaisFrios);
    free(topNExtremos);
    free(dadosPorPais);
}


//TODO não usada
//usar para procurar os strings nos ficheiro
bool fgetstring(list_node_t * aux, bool porPais, char string [BUFFER_SIZE])
{
    while(aux != NULL)
    {
        if(porPais && (strstr(aux->payload->pais, string) != NULL))
        {
            return true;
        }
        else if(strstr(aux->payload->cidade,string) != NULL)
        {
            return true;
        }
        aux = aux->next;
    }
    return false;
}

//função estrutural do cálculo do miving average
void m4_calculo_MA(int M, DADOS *dados, bool porPais, bool porCidade)
{
    int anos_a_estudar[NR_DE_ANOS_A_ESTUDAR] = {ANO_A_ESTUDAR_1860, ANO_A_ESTUDAR_1910, ANO_A_ESTUDAR_1960, ANO_A_ESTUDAR_1990, ANO_A_ESTUDAR_2013};
    float *vectMA_anos = NULL;
    char pais_a_analisar[] = "Qual o País que quer analisar?";
    char cidade_a_analisar[] = "Qual a Cidade que quer analisar?";
    char paisOuCidade[100];

    int anoMax = porCidade ? dados->citiesAnoMax : dados->countriesAnoMax;
    int anoMin = porCidade ? dados->citiesAnoMin : dados->countriesAnoMin;

    list_node_t *aux = NULL;
    if(porPais || porCidade)
    {
        aux = porCidade ? dados->headCitiesOriginal->next : dados->headCountriesOriginal->next; //->next para a node a seguir à dummy node
    }
    else
    {
        aux = dados->headCountriesOriginal->next;
    }

     vectMA_anos = (float*) checkedMalloc(sizeof(float) * (anoMax - anoMin + 1));

     if(porPais || porCidade) getstring(paisOuCidade, porPais ? pais_a_analisar : cidade_a_analisar);


    int ano_a_analisar = anoMin;
    int pos_no_vetor = 0;
    while (ano_a_analisar <= anoMax )
    {
        vectMA_anos[pos_no_vetor] = m4_media_ano (dados, ano_a_analisar, porPais, porCidade, paisOuCidade, M, aux);
        ano_a_analisar ++;
        pos_no_vetor ++;
    }

    if(fgetstring(aux, porPais, paisOuCidade) || (!porPais && !porCidade))
    {
        for (int g = 0; g < NR_DE_ANOS_A_ESTUDAR; g++)
       {
           if(anos_a_estudar[g] < anoMin)   printf("Não há dados para %d\n", anos_a_estudar[g]);
           else if(!porPais && !porCidade)
           {
               printf("Em %d, o aumento da temperatura global foi == %f graus\n", anos_a_estudar[g],
                                                       m4_calculo_aumento_temp(dados, vectMA_anos, anos_a_estudar[g], porPais, porCidade));
           }
           else
           {
               printf("Em %d, o aumento da temperatura em %s foi == %f graus\n", anos_a_estudar[g], paisOuCidade,
                                                       m4_calculo_aumento_temp(dados, vectMA_anos, anos_a_estudar[g], porPais, porCidade));
           }
       }
    }
    else
    {
        if(porPais)
        {
            printf("%s não é um país ou está mal escrito!\n", paisOuCidade);
        }
        else
        {
            printf("%s não é uma cidade ou está mal escrito!\n", paisOuCidade);
        }
        printf("Pouca cultura geral!!\tISTudasses\n");
    }

    free(vectMA_anos);
}

//TODO mudar nome
//calcula a media da temperatura de cada mês ao longo de um ano
//chama a função moving average
float m4_media_ano (DADOS *dados, int ano_a_analisar, bool porPais, bool porCidade, char paisOuCidade[100], int M, list_node_t *aux)
{
    float soma_temperatura_mes = 0;
    int mes_a_analisar = 1;


    float *vect_temp = (float*) checkedMalloc(sizeof(float) * (NR_DE_MESES));
    bool *vect_mes_tem_dados = (bool*) checkedMalloc(sizeof(float) * (NR_DE_MESES));

    for(int b = 0; b < NR_DE_MESES ; b ++)
    {
        vect_temp[b] = -__FLT_MAX__;
        vect_mes_tem_dados[b] = false;
    }

    int nr_dados = 0;
    while( aux->next != NULL)
    {
        if((!porPais && !porCidade) ||
            (porPais && strstr(aux->payload->pais, paisOuCidade) != NULL) ||
            (porCidade && strstr(aux->payload->cidade, paisOuCidade) != NULL))
        {
            //otimizar programa
            if(aux->payload->dt.ano > ano_a_analisar)   break;

            if(aux->next != NULL)
            {
                //quando ainda não chegou ao ano a estudar
                if(aux->payload->dt.ano < ano_a_analisar)  aux = aux ->next;

                //quando se encontra no anoa estudar
                else if( aux->payload->dt.ano == ano_a_analisar )
                {
                    //atualiza a contagem de dados
                    if(mes_a_analisar != aux->payload->dt.mes)
                    {
                        if(mes_a_analisar != NR_DE_MESES)   mes_a_analisar ++;
                        soma_temperatura_mes = 0;
                        nr_dados = 0;
                    }
                    //faz uma nova média sempre que um dado novo aparecer
                    if(aux->payload->dt.mes == mes_a_analisar)
                    {
                        //o vect_temp guarda temporariamente a soma das temperaturas nesse mes
                        soma_temperatura_mes += aux->payload->temp;
                        nr_dados ++;
                        vect_temp[mes_a_analisar-1] = soma_temperatura_mes / nr_dados;

                        vect_mes_tem_dados[mes_a_analisar-1] = true;
                        aux = aux->next;
                    }
                    //quando não tem dados no mês
                    else
                    {
                        aux = aux->next;
                    }
                }
            }
        }
        else    aux = aux->next;
    }

    float media_anual = m4_moving_average(M, vect_temp, vect_mes_tem_dados);

    free(vect_temp);
    free(vect_mes_tem_dados);
    return media_anual;

}
// calcula um moving average ao longo de uma ano
// e a média desse ano
float m4_moving_average(int M, float* vect_temp, bool* vect_mes_tem_dados)
{
    float soma_temp = 0;
    int m = 0;      //nr de meses do moving average
    float vect_aux[NR_DE_MESES];
    int nr_de_meses_com_dados = 0;
    int nr_meses_considerados = 0;


    for ( int a = 0; a < NR_DE_MESES; a++)
    {
        vect_aux[a] = 0.0f;
        if(vect_mes_tem_dados[a])   nr_de_meses_com_dados ++;
    }
    // moving average de cada mês
    for ( int a = 0; a < NR_DE_MESES; a++)
    {
        //se houver dados nesse mês
        if(vect_mes_tem_dados[a])
        {
            if(m<M)     m++;
            for(int y = a ; y >= 0; y--)
            {
                if(vect_mes_tem_dados[y])
                {
                    soma_temp += vect_temp[y];
                    nr_meses_considerados++;
                    if(nr_meses_considerados == m) break;
                }
            }
            vect_aux[a] = (float)soma_temp / nr_meses_considerados;
            soma_temp = 0;
            nr_meses_considerados = 0;
        }
    }
    for ( int a = 0; a < NR_DE_MESES; a++)  soma_temp += vect_aux[a];

    return (float)soma_temp/nr_de_meses_com_dados;
}

//calcula a amplitude entre o ano minimo do ficheiro e o ano que recebe como parametro de entrada
float m4_calculo_aumento_temp(DADOS *dados, float *vect, int ano,  bool porPais, bool porCidade)
{
    float max = -__FLT_MAX__;
    float min = __FLT_MAX__;
    float ATG = 0;
    int size = ano - (porCidade ? dados->citiesAnoMin: dados->countriesAnoMin);

    for (int i = 0; i < size; i++)
    {
        if(vect[i] < min) min = vect[i];
        if(vect[i] > max) max = vect[i];
    }
    ATG = max - min;
    return ATG;
}

void menu_analise_da_temperatura_global(DADOS *dados)
{
    int M = 0;
    char comentario[] = "Quantos meses pretende utilizar no cálculo da MA (moving average)?";

    printf("\n\n\t---Análise da temperuta Global---\n\n");

    M = perguntar_referencia_a_analisar(1 ,MAX_M, comentario);

    // cálculo do aumento de temperatura global
    m4_calculo_MA(M, dados, false, false);
    // cálculo do aumento de temperatura por país
    m4_calculo_MA(M, dados, true, false);
    // cálculo do aumento de temperatura por cidade
    m4_calculo_MA(M, dados, false, true);



}
