#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "modografico.h"
#include "conjuntodados.h"
#include "ficheiros.h"
#include "util.h"

#define M_PI 3.14159265358979323846264338327950288
#define STRING_SIZE 100       // max size for some strings
#define HEIGHT_WINDOW_SIZE 850
#define WIDTH_WINDOW_SIZE 1390
#define WINDOW_POSX 0       // initial position of the window: x
#define WINDOW_POSY 0       // initial position of the window: y         n sei oq e
#define VECT_CIDADES_INITIAL_SIZE 50

void init_empty_cidade(dados_temp *cidade) {
    cidade->cidade[0] = '\0';
}

void coloca_ou_atualiza_node_no_vetor_cidades(list_node_t *node, dados_temp **cidades, int **cidadesNumDados, int *vecCidadesSize) {
    bool necessitaAumentarVetor = true;

    //Vai a cada elemento do vetor das cidades
    for(int i = 0; i<(*vecCidadesSize); i++) {
        if((*cidades)[i].cidade[0] == '\0')
        {
            //Se encontrar um espaço vazio no vetor, é porque a cidade do nó da lista
            // não está no vetor
            //Faz uma cópia da cidade da payload da lista para a entrada do vetor
            (*cidades)[i] = *node->payload;
            
            (*cidadesNumDados)[i]++;
            necessitaAumentarVetor = false;
            break;
        }
        else if(strcmp(node->payload->cidade, (*cidades)[i].cidade) == 0)
        {
            //Se encontrar no vector a cidades correspondente a este nó da lista
            //Atualiza a temperatura dessa cidade no vetor
            (*cidades)[i].temp += node->payload->temp;
            (*cidadesNumDados)[i]++;
            necessitaAumentarVetor = false;
            break;
        }
    }

    //Caso não tenha encontrado nenhum espaço vazio no vetor e seja necessário
    // aumentar o seu tamanho
    if(necessitaAumentarVetor) {
        //Guarda o tamanho do vetor
        int sizeAnterior = *vecCidadesSize;
        //Aumenta o tamanho do vetor
        *vecCidadesSize += VECT_CIDADES_INITIAL_SIZE;
        *cidades = (dados_temp*) checkedRealloc(*cidades, (*vecCidadesSize) * sizeof(dados_temp));
        *cidadesNumDados = (int*) checkedRealloc(*cidadesNumDados, (*vecCidadesSize) * sizeof(dados_temp));

        //Inicializa todas as entradas para cidades novas criadas no vetor
        for(int i = sizeAnterior; i<(*vecCidadesSize); i++) {    
            init_empty_cidade(&(*cidades)[i]);
            (*cidadesNumDados)[i] = 0;
        }

        //A primeira das novas entradas do vetor criadas é onde vai ser colocada a 
        // cidade correspondente ao nó da lista
        //Faz uma cópia da cidade da payload da lista para a entrada do vetor
        (*cidades)[sizeAnterior] = *node->payload;
        (*cidadesNumDados)[sizeAnterior]++;
    }
}

void atualiza_cidades_ao_aumentar_ano_ou_ano_inicial(DADOS *dados, dados_temp **cidades, int *vecCidadesSize, int ano) {
    //Vetor que contem o numero de valores para cada cidade, para depois fazer a média
    //No cidades[i].temp vai ser guardada a soma de todas as temperaturas desse ano
    // e depois a temperatura final vai ser calculada dividindo pelo numero de dados 
    int *cidadesNumDados = NULL;

    //Caso seja o primeiro ano ou se esteja a voltar para trás cria um vetor novo
    //Ao andar para trás ele analisa todos os anos desde o inicial até ao ano
    if(ano == dados->citiesAnoMin)
    {
        //Caso seja o primeiro ano inicializa o vetor

        //Se passar do ultimo ano de volta para o primeiro tem de apagar o vetor
        if(*cidades != NULL) {
            free(*cidades);
        }
        
        *vecCidadesSize = VECT_CIDADES_INITIAL_SIZE;
        *cidades = (dados_temp*) checkedMalloc((*vecCidadesSize) * sizeof(dados_temp));
        for(int i = 0; i<(*vecCidadesSize); i++) {
            init_empty_cidade(&(*cidades)[i]);
        }
    }

    //Fazer com que o vetor tenha o mesmo tamanho que o das cidades
    cidadesNumDados = checkedMalloc((*vecCidadesSize) * sizeof(int));
    

    //Inicializa o numDados a zero para todas as cidades
    for(int i = 0; i<(*vecCidadesSize); i++) {
        cidadesNumDados[i] = 0;
        (*cidades)[i].tempAnterior = (*cidades)[i].temp;
    }

    list_node_t *aux = dados->headCitiesOriginal->next;

    //Avança o aux até ao ano que se quer
    while(aux != NULL && aux->payload->dt.ano < ano) {
        aux = aux->next;
    }

    //Começando com o aux a apontar para o primeiro elemento da lista com dt.ano == ano
    while(aux != NULL && aux->payload->dt.ano == ano) {
        coloca_ou_atualiza_node_no_vetor_cidades(aux, cidades, &cidadesNumDados, vecCidadesSize);

        aux = aux->next;
    }

    //Calcula as médias
    for(int i = 0; i<(*vecCidadesSize); i++) {
        if((*cidades)[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }
        if(cidadesNumDados[i] > 0) {
            (*cidades)[i].temp = (*cidades)[i].temp / cidadesNumDados[i];
        } else {
            //TODO se tiver a andar para trás tira-o da lista
            // (nota: se houver varios anos consecutivos sem dados não basta ver numDados = 0
            // Tenho de ter um bool array para verificar se leu dados sobre esse?
            // mesmo assim acho que não dá. Se calhar cago só

            //Se tiver 0 dados sobre essa cidade em relacao a este ano
            // coloca a sua temperatura à temperatura anterior
            (*cidades)[i].temp = (*cidades)[i].tempAnterior;
        }
    }

    free(cidadesNumDados);
}

//TODO isto é muito lento a andar para trás
void mudarAno(DADOS *dados, dados_temp **cidades, int *vecCidadesSize, int *ano, bool forward) {
    //Aumenta ou diminui o ano
    //Se passar dos limites "dá a volta"
    if(forward) {
        (*ano)++;
        if(*ano > dados->citiesAnoMax) {
            *ano = dados->citiesAnoMin;
        }
    } else {
        (*ano)--;
        if(*ano < dados->citiesAnoMin) {
            *ano = dados->citiesAnoMax;
        }
    }

    if(forward) {
        atualiza_cidades_ao_aumentar_ano_ou_ano_inicial(dados, cidades, vecCidadesSize, *ano);
    } else {
        //Se for para andar para trás, vai voltar a criar o vetor desde o ano
        // inicial até ao ano para que se mudou
        for(int a = dados->citiesAnoMin; a<(*ano); a++) {
            atualiza_cidades_ao_aumentar_ano_ou_ano_inicial(dados, cidades, vecCidadesSize, a);
        }
    }
}

void printCenas(dados_temp *cidades, int vecCidadesSize, int anoAtual) {
    printf("\n\t--%d--\n\n", anoAtual);
    for(int i = 0; i<vecCidadesSize; i++) {
        if(cidades[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }
        printf("%s - %.2f\n", cidades[i].cidade, cidades[i].temp);
    }
}

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
void modoGrafico(char *nomeFileCidades)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *AppleGaramond = NULL;
    SDL_Surface *imgs[1];
    SDL_Event event;
    bool stay = true;
    int width = WIDTH_WINDOW_SIZE;
    int height = HEIGHT_WINDOW_SIZE;
    int delay = 0;

    DADOS dados;
    //Ano que está a ser mostrado
    int anoAtual = 0;

    //Vector com as cidades que estão a ser mostradas
    dados_temp *cidades = NULL;
    int vecCidadesSize = 0;

    dados_init(&dados);
    read_file_cities (&dados, nomeFileCidades);

    //Começa no menor ano
    anoAtual = dados.citiesAnoMin-1;

    // initialize graphics
    InitEverything(width, height, &AppleGaramond, imgs, &window, &renderer);

    while( stay )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                stay = false;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_q:
                        stay = false;
                        break;
                    case SDLK_a:
                        mudarAno(&dados, &cidades, &vecCidadesSize, &anoAtual, false);
                        printCenas(cidades, vecCidadesSize, anoAtual);
                        break;
                    case SDLK_s:
                        mudarAno(&dados, &cidades, &vecCidadesSize, &anoAtual, true);
                        printCenas(cidades, vecCidadesSize, anoAtual);
                        break;
                    default:
                        break;
                }
            }
        }
        // render game table
        RenderTable(imgs, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    dados_free(&dados);
    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(AppleGaramond);
    SDL_FreeSurface(imgs[0]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable( SDL_Surface *_img[], SDL_Renderer* _renderer )
{

    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;//, board, board_square;


    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = WIDTH_WINDOW_SIZE;
    tableDest.h = HEIGHT_WINDOW_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);


    // destroy everything
    SDL_DestroyTexture(table_texture);

}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("mapa_mundi.jpg");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("AppleGaramond.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "< Projeto Final :: WarmingUp > \tJoão Almeida 90119 \tMiguel Fazenda 90146"
                                , WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}
