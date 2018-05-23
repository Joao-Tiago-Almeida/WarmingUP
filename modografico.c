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
#include "data.h"

#define M_PI 3.14159265358979323846264338327950288
#define STRING_SIZE 100       // max size for some strings
#define HEIGHT_WINDOW_SIZE 850
#define WIDTH_WINDOW_SIZE 1390
#define WINDOW_POSX 0       // initial position of the window: x
#define WINDOW_POSY 0       // initial position of the window: y         n sei oq e
#define NORTE 0
#define SUL 1
#define ESTE 3
#define OESTE 2
#define LATITUDE_MAX 90
#define LONGITUDE_MAX 180
#define TEMP_MAX 50
#define TEMP_MIN -40
#define MAX_RGB 255
#define TEMP_REF 10
#define VECT_CIDADES_INITIAL_SIZE 50
#define BUFFER_SIZE 100
#define VELOCIDADE_MAX 10
#define DELAY 30
#define MAX_PAUSA_COUNTER 20

#define DIST_MIN_RATO_CIDADE 13


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
void mudarAno(DADOS *dados, dados_temp **cidades, int *vecCidadesSize, int *ano) {
    //Aumenta o ano
    //Se passar dos limites "dá a volta"
    (*ano)++;
    if(*ano > dados->citiesAnoMax) {
        *ano = dados->citiesAnoMin;
    }

    atualiza_cidades_ao_aumentar_ano_ou_ano_inicial(dados, cidades, vecCidadesSize, *ano);
}

/*void printCenas(dados_temp *cidades, int vecCidadesSize, int anoAtual) {
    printf("\n\t--%d--\n\n", anoAtual);
    for(int i = 0; i<vecCidadesSize; i++) {
        if(cidades[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }
        printf("%s - %.2f\n", cidades[i].cidade, cidades[i].temp);
    }
}*/

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
// Devolve false se for para sair do programa ou true se for para mudar para o modo textual
bool modoGrafico( char *nomeFilePaises, char *nomeFileCidades, DADOS *dados  )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *AppleGaramond = NULL;
    SDL_Surface *imgs[1];
    SDL_Event event;
    SDL_Texture *renderTexture; //Serve para fazer zoom
    bool stay = true;
    int width = WIDTH_WINDOW_SIZE;
    int height = HEIGHT_WINDOW_SIZE;
    bool modo_texto = false;

    //Ano que está a ser mostrado
    int anoAtual = 0;
    bool pausa = false;
    int pausaCounter = 0; //Para ir mudando a transparência do simbolo da pausa

    int velocidade = 0;
    int velocidadeCounter = 0;

    //Vector com as cidades que estão a ser mostradas
    dados_temp *cidades = NULL;
    int vecCidadesSize = 0;

    int mouseX = 0, mouseY = 0;

    int selectedCity = -1;

    int zoomPosX = -1, zoomPosY = -1;
    SDL_Rect windowRect = {0, 0, width, height};

    if(dados->headCitiesOriginal == NULL)  read_file_cities (dados,nomeFileCidades);

    //Começa no menor ano
    //TODO porquw -1, ?? pq quando chama a função incrementa???
    anoAtual = dados->citiesAnoMin-1;

    // initialize graphics
    InitEverything(width, height, &AppleGaramond, imgs, &window, &renderer);
    renderTexture = CreateRenderTexture(renderer,width, height);

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
                    case SDLK_t:
                        stay = 0;
                        modo_texto = true;
                        break;
                    case SDLK_q:
                        stay = false;
                        break;
                    case SDLK_SPACE:
                        pausa = !pausa;
                        break;
                    case SDLK_a:
                        if(velocidade > 0) {
                            velocidade--;
                            velocidadeCounter = 0;
                        }
                        break;
                    case SDLK_s:
                        if(velocidade < VELOCIDADE_MAX) {
                            velocidade++;
                            velocidadeCounter = 0;
                        }
                        break;
                    default:
                        break;
                }
            } else if(event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                if(zoomPosX == -1) {
                    zoomPosX = mouseX;
                    zoomPosY = mouseY;
                } else {
                    zoomPosX = -1;
                    zoomPosY = -1;
                }
            }
        }

        //Se não estiver na pausa avança o ano
        if(!pausa) {
            if(velocidade == 0) { //Velocidade 0 significa andar de 2 em dois anos
                mudarAno(dados, &cidades, &vecCidadesSize, &anoAtual);
                mudarAno(dados, &cidades, &vecCidadesSize, &anoAtual);
            } else {
                velocidadeCounter++;
                if(velocidadeCounter >= velocidade) {
                    velocidadeCounter = 0;
                    mudarAno(dados, &cidades, &vecCidadesSize, &anoAtual);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, renderTexture);

        //Desenha o mapa e as cidades na renderTexture
        RenderMap( imgs, renderer, width, height);
        selectedCity = RenderCities(renderer, AppleGaramond, dados, cidades, vecCidadesSize);
        SDL_RenderPresent(renderer);


        //Desenha na janela
        SDL_SetRenderTarget(renderer, NULL);

        //Desenha o mapa e os pontos na janela
        SDL_RenderCopy(renderer, renderTexture, &windowRect, &windowRect);

        //Se tiver o rato ao pé de alguma cidade mostra informações
        if(selectedCity != -1) {
            RenderSelectedCity(renderer, AppleGaramond, &cidades[selectedCity]);
        }

        RenderStatus(renderer, AppleGaramond, anoAtual, pausa, velocidade);
        if(pausa) {
            pausaCounter++;
            if(pausaCounter > MAX_PAUSA_COUNTER) {
                pausaCounter = 0;
            }
            RenderPausa(renderer, pausaCounter);
        }

        RenderLegenda( renderer ,width, height, AppleGaramond);

        if(zoomPosX != -1) {
            RenderZoom(renderer, renderTexture, zoomPosX, zoomPosY);
        }

        SDL_RenderPresent(renderer);

        // add a delay
        SDL_Delay(DELAY);
    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(AppleGaramond);
    SDL_FreeSurface(imgs[0]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return modo_texto;
}

void RenderZoom(SDL_Renderer* renderer, SDL_Texture* renderTexture, int zoomPosX, int zoomPosY) {
    SDL_Rect zoomRectSrc, zoomRectDest;

    zoomRectSrc.x = zoomPosX - 100;
    zoomRectSrc.y = zoomPosY - 100;
    zoomRectSrc.w = 200;
    zoomRectSrc.h = 200;
    zoomRectDest.x = zoomPosX - 200;
    zoomRectDest.y = zoomPosY - 200;
    zoomRectDest.w = 400;
    zoomRectDest.h = 400;

    SDL_RenderCopy(renderer, renderTexture, &zoomRectSrc, &zoomRectDest);
}

//Devolve o indice da cidade selecionada
int RenderCities(SDL_Renderer *renderer, TTF_Font *AppleGaramond, DADOS* dados, dados_temp* cidades, int vecCidadesSize) {
    int mouseX = 0, mouseY = 0;
    int selectedCity = -1;
    SDL_GetMouseState(&mouseX, &mouseY);

    for(int i = 0; i<vecCidadesSize; i++) {
        int x = 0, y = 0;
        if(cidades[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }
        RenderCity(renderer, WIDTH_WINDOW_SIZE, HEIGHT_WINDOW_SIZE, AppleGaramond, &cidades[i], dados, &x, &y);

        if(CalcDistance(x, y, mouseX, mouseY) < DIST_MIN_RATO_CIDADE) {
            selectedCity = i;
        }
    }

    return selectedCity;
}

void RenderSelectedCity(SDL_Renderer *renderer, TTF_Font *font, dados_temp *cidade) {
    char buffer[BUFFER_SIZE];
    SDL_Color color = { 255, 255, 255, 255 };

    RenderText(WIDTH_WINDOW_SIZE-100, 0, cidade->cidade, font, &color, renderer);
    sprintf(buffer, "%.2f C", cidade->temp);
    RenderText(WIDTH_WINDOW_SIZE-100, 25, buffer, font, &color, renderer);
}

int CalcDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

//recebe dois argumentos e faz o calculo entre eles
//true, se for latitude, false se longitude
//a coord_local é inicialmente a coordenada antes da transformacao , ficheiro
//cai guardar temporariamente o deslocamnetonum eixo  da coordenada (0,0) à cidade
//e dps vai ter a coordenada exata do pais
//bool:: calculo da latitude ? => true , longitude => false
int calculo_coordenada(float coord_local, int _direcao, int _width, int _height, bool latitude)
{
    //TODO escrever melhor
    //a divisão por dois justifica se pois o fator de deslocamento por pixel
    //recaí sobre uma distância total e apartir do centro da imagem
    //o deslocamento máximo para cada lado pode no máximo ser metade da distÂncia total
    float latitude_por_pixel = (float)_height/(2.0f*LATITUDE_MAX);
    float longitude_por_pixel = (float)_width/(2.0f*LONGITUDE_MAX);

    //permite defenir quando se adiciona ou se subtrai

    int indice = _direcao % 2 == 0 ? -1 : 1;

    if(latitude)
    {
        coord_local = (_height/2) + indice * (latitude_por_pixel * coord_local);
    }
    else
    {
        coord_local = (_width/2) + indice * (longitude_por_pixel * coord_local);
    }

    return coord_local;
}

//calcula a cor em função da temperatura
//TODO falta adicionar a temperatura maxima e minima do ficheiro
//TODO pode ser melhorado apenas com uma função
void RenderColor(SDL_Renderer * _renderer, int _temperatura, int x, int y, DADOS *dados)
{
    //m representa o declive que permite calcular componente verde
    int m = 0, b = 0, green = 0;

    //frio
    if(_temperatura<TEMP_REF)
    {
        //cresce à medida que a temperatura diminui
        m = MAX_RGB/(TEMP_REF-dados->citiesTempMin);
        //achar o b sabendo que se quando a temperatura fosse 10 o valor de verde seria máximo
        b = MAX_RGB - m * TEMP_REF;
        //calculo do verde em função da _temperatura
        green =  m*_temperatura + b;
        filledCircleRGBA(_renderer, x, y, 5, 0, green, 255);
    }
    //quente
    else
    {
        //decresce à medida que a temperatura diminui
        m = MAX_RGB/(TEMP_REF-dados->citiesTempMax);
        //achar o b sabendo que quando a temperatura é 10 o valor de verde é máximo
        b = MAX_RGB - m * TEMP_REF;
        //calculo do verde em função da _temperatura
        green =  m*_temperatura + b;

        filledCircleRGBA(_renderer, x, y, 5, 255, green, 0);
    }
    //printf("temperatura == %d\ngreen:: %d\n", _temperatura, green);
}

void RenderCity(SDL_Renderer * _renderer, int width, int height, TTF_Font *_font, dados_temp* cidade, DADOS *dados, int *x, int *y) {
    *x = calculo_coordenada(cidade->longitude.angular,
        cidade->longitude.direcao, width, height, false);
    *y = calculo_coordenada(cidade->latitude.angular,
        cidade->latitude.direcao, width, height, true);

    RenderColor(_renderer, cidade->temp, *x, *y, dados);
}

//TODO a cor não percisa de ser ponteiro duplo
void RenderLegenda(SDL_Renderer * _renderer, int width, int height, TTF_Font *_font)
{
    SDL_Color white = {255, 255, 255 };

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
    SDL_Rect legenda = {50, 600, 100, 60};
    SDL_RenderFillRect(_renderer, &legenda);
    char frio[] = "frio";
    char quente[] = "quente";
    RenderText(80, 615, frio, _font, &white, _renderer);
    RenderText(70, 630, quente, _font, &white, _renderer);
    filledCircleRGBA(_renderer, 60, 625, 5, 0, 0, 255);
    filledCircleRGBA(_renderer, 125, 625, 5, 0, 255, 255);
    filledCircleRGBA(_renderer, 60, 640, 5, 255, 255, 0);
    filledCircleRGBA(_renderer, 125, 640, 5, 255, 0, 0);
}
/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */
void filledCircleRGBA(SDL_Renderer * _renderer, float _circleX, float _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
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
void RenderMap( SDL_Surface *_img[], SDL_Renderer* _renderer , int width, int height)
{

    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;//, board, board_square;

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = width;
    tableDest.h = height;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);


    // destroy everything
    SDL_DestroyTexture(table_texture);
}

void RenderStatus(SDL_Renderer *renderer, TTF_Font *font, int ano, bool pausa, int velocidade) {
    char buffer[BUFFER_SIZE];
    SDL_Color anoTextColor = { 255, 255, 255 };
    
    sprintf(buffer, "%d", ano);
    RenderText(50, 50, buffer, font, &anoTextColor, renderer);

    if(velocidade == 0) {
        RenderText(50, 65, "Velocidade: 1 (2 anos)", font, &anoTextColor, renderer);
    } else if(velocidade == 1) {
        RenderText(50, 65, "Velocidade: 1", font, &anoTextColor, renderer);
    } else {
        sprintf(buffer, "Velocidade: 1/%d", velocidade);
        RenderText(50, 65, buffer, font, &anoTextColor, renderer);
    }
}

void RenderPausa(SDL_Renderer *renderer, int pausaCounter) {
    SDL_Rect rect = { 10, 10, 10, 40 };
    int alpha = 200 + 55 * abs(pausaCounter - MAX_PAUSA_COUNTER/2) / MAX_PAUSA_COUNTER/2;
    SDL_SetRenderDrawColor(renderer, 43, 168, 11, alpha);
    SDL_RenderFillRect(renderer, &rect);
    rect.x += 25;
    SDL_RenderFillRect(renderer, &rect);
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
    *_font = TTF_OpenFont("AppleGaramond.ttf", 24);
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
    window = SDL_CreateWindow( "< Projeto Final :: WarmingUp > \t|| João Almeida 90119 \tMiguel Fazenda 90146 ||"
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return renderer;
}

SDL_Texture *CreateRenderTexture(SDL_Renderer* _renderer, int width, int height) {
    SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if(texture == NULL)
    {
        printf("Failed to create render texture : %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    }
    return texture;
}
/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}
