#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "modografico.h"
#include "conjuntodados.h"
#include "ficheiros.h"
#include "util.h"
#include "data.h"
#include "math.h"

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
#define MAX_RGB 255
#define MIN_RGB 0
#define TEMP_REF 10
#define VECT_CIDADES_INITIAL_SIZE 50
#define BUFFER_SIZE 200
#define VELOCIDADE_MAX 10
#define DELAY 30
#define MAX_PAUSA_COUNTER 20

#define DIST_MIN_RATO_CIDADE 13
#define ZOOM_RECT_SIZE 400
#define ZOOM_SRC_RECT_SIZE 200

/**
 * main function: entry point of the program
 * only to invoke other functions !
 * Devolve false se for para sair do programa ou true se for para mudar para o modo textual
 */
bool ModoGrafico(char *nomeFileCidades, DADOS *dados  )
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
    bool mostra_painel_informativo = false;

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

    bool mouse_down = false;

    int zoomCounter = 0; //Conta o tempo(em ciclos) que o zoom está ativo

    list_node_t* aux = NULL;

    if(dados->headCitiesOriginal == NULL)  read_file_cities (dados,nomeFileCidades);

    vecCidadesSize = VECT_CIDADES_INITIAL_SIZE;
    cidades = (dados_temp*) checkedMalloc(vecCidadesSize * sizeof(dados_temp));

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
                    // TODO fazer legenda das teclas
                    case SDLK_t:
                        stay = 0;
                        modo_texto = true;
                        break;
                    case SDLK_q:
                        stay = false;
                        break;
                    case SDLK_i:
                        mostra_painel_informativo = !mostra_painel_informativo;
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
                    zoomCounter = 0;
                }
                mouse_down = true;
            } else if (event.type == SDL_MOUSEBUTTONUP)
            {
                mouse_down = false;
            }
        }
        
        if(zoomPosX != -1)
        {
            //Enquanto o zoom está ativo
            zoomCounter++;

            if(mouse_down)
            {
                //Enquano o botão do rato está pressionado atualiza a posição do zoom
                SDL_GetMouseState(&mouseX, &mouseY);
                zoomPosX = mouseX;
                zoomPosY = mouseY;
            }
        }

        //Se não estiver na pausa avança o ano
        if(!pausa) {
            if(velocidade == 0) { //Velocidade 0 significa andar de 2 em dois anos
                MudarAno(dados, &aux, &cidades, &vecCidadesSize, &anoAtual);
                MudarAno(dados, &aux, &cidades, &vecCidadesSize, &anoAtual);
            } else {
                velocidadeCounter++;
                if(velocidadeCounter >= velocidade) {
                    velocidadeCounter = 0;
                    MudarAno(dados, &aux, &cidades, &vecCidadesSize, &anoAtual);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, renderTexture);

        //Desenha o mapa e as cidades na renderTexture
        RenderMap( imgs, renderer, width, height);
        RenderCities(renderer, dados, cidades, vecCidadesSize);


        //Desenha na janela
        SDL_SetRenderTarget(renderer, NULL);

        //Desenha o mapa e os pontos na janela
        SDL_RenderCopy(renderer, renderTexture, &windowRect, &windowRect);

        selectedCity = GetSelectedCity(cidades, vecCidadesSize, zoomPosX, zoomPosY);
        //Se tiver o rato ao pé de alguma cidade mostra informações
        if(selectedCity != -1) {
            RenderSelectedCity(renderer, AppleGaramond, &cidades[selectedCity]);
        }

        RenderVelocityStatus(renderer, AppleGaramond, velocidade);
        RenderYear(renderer, AppleGaramond, anoAtual);
        if(pausa) {
            pausaCounter++;
            if(pausaCounter > MAX_PAUSA_COUNTER) {
                pausaCounter = 0;
            }
            RenderPausa(renderer, pausaCounter);
        }

        RenderLegenda(renderer, AppleGaramond, dados);

        if(zoomPosX != -1 && !mostra_painel_informativo) {
            RenderZoom(renderer, renderTexture, zoomPosX, zoomPosY);
        }

        if(mostra_painel_informativo)
        {
            zoomPosX = -1; //Se o painel informativo estiver ativo desativa o zoom
            zoomCounter = 0;
            RenderInfo(renderer, AppleGaramond);
        }
        if(zoomCounter > 2000/DELAY)
        {
            RenderDetective(renderer, AppleGaramond);
        }

        SDL_RenderPresent(renderer);

        // add a delay
        SDL_Delay(DELAY);
    }


    if(cidades != NULL) {
        free(cidades);
    }
    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(AppleGaramond);
    SDL_FreeSurface(imgs[0]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return modo_texto;
}

/**
*Amplia imagem de modo que utilizador possa ver com mais detalhe parte da imagem
*/
void RenderZoom(SDL_Renderer* renderer, SDL_Texture* renderTexture, int zoomPosX, int zoomPosY) {
    SDL_Rect zoomRectSrc, zoomRectDest;

    zoomRectSrc.x = zoomPosX - ZOOM_SRC_RECT_SIZE/2;
    zoomRectSrc.y = zoomPosY - ZOOM_SRC_RECT_SIZE/2;
    zoomRectSrc.w = ZOOM_SRC_RECT_SIZE;
    zoomRectSrc.h = ZOOM_SRC_RECT_SIZE;
    zoomRectDest.x = zoomPosX - ZOOM_RECT_SIZE/2;
    zoomRectDest.y = zoomPosY - ZOOM_RECT_SIZE/2;
    zoomRectDest.w = ZOOM_RECT_SIZE;
    zoomRectDest.h = ZOOM_RECT_SIZE;

    SDL_RenderCopy(renderer, renderTexture, &zoomRectSrc, &zoomRectDest);
}

/**
*Devolve o indice da cidade selecionada
*/
int GetSelectedCity(dados_temp* cidades, int vecCidadesSize, int zoomPosX, int zoomPosY) {
    int selectedCity = -1;
    int mouseX = 0, mouseY = 0;

    SDL_GetMouseState(&mouseX, &mouseY);

    //Se estiver a fazer zoom
    if(zoomPosX != -1) {

        int distX = mouseX-zoomPosX;
        int distY = mouseY-zoomPosY;
        //Se rato estiver dentro do quadrado do zoom
        if(abs(distX) <= ZOOM_RECT_SIZE/2 && abs(distY) <= ZOOM_RECT_SIZE/2) {
            //Corrige as coordenadas do rato para coincidirem com as coordenadas no mapa
            mouseX -= distX/2;
            mouseY -= distY/2;
        }
    }

    for(int i = 0; i<vecCidadesSize; i++) {
        int x = 0, y = 0;
        if(cidades[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }

        x = calculo_coordenada(cidades[i].longitude.angular,
            cidades[i].longitude.direcao, WIDTH_WINDOW_SIZE, HEIGHT_WINDOW_SIZE, false);
        y = calculo_coordenada(cidades[i].latitude.angular,
            cidades[i].latitude.direcao, WIDTH_WINDOW_SIZE, HEIGHT_WINDOW_SIZE, true);

        int dist = CalcDistance(mouseX, mouseY, x, y);
        if(dist <= DIST_MIN_RATO_CIDADE) {
            selectedCity = i;
        }
    }
    return selectedCity;
}

/**
*Desenha as várias cidades
*/
void RenderCities(SDL_Renderer *renderer, DADOS* dados, dados_temp* cidades, int vecCidadesSize) {
    for(int i = 0; i<vecCidadesSize; i++) {
        if(cidades[i].cidade[0] == '\0') {
            //Se encontrar uma entrada do vetor cidades vazia sai do loop, porque
            // o resto do vetor está vazio
            break;
        }
        RenderCity(renderer, WIDTH_WINDOW_SIZE, HEIGHT_WINDOW_SIZE, &cidades[i], dados);
    }
}

/*
*distância entre dois pontos
*/
int CalcDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

/*
recebe dois argumentos e faz o calculo entre eles
*true, se for latitude, false se longitude
*a coord_local é inicialmente a coordenada antes da transformacao , ficheiro
*  vai guardar temporariamente o deslocamnetonum eixo da coordenada (0,0) ate à cidade
*  e dps vai ter a coordenada exata do pais
*bool:: calculo da latitude ? => true , longitude => false
*/
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
/**
*calcula a cor em função da temperatura
*/
void RenderColor(int _temperatura, DADOS *dados, int* red, int* green, int* blue){
    int m = 0;  //declive da reta
    int b = 0;  // b vai ser a abcissa do ponto médio (temp_mas - temp_min), em que verde é máximo
    *red = 0;
    *green = 0;
    *blue = 0;

    float temp_min = dados->citiesTempMin;
    float temp_max = dados->citiesTempMax;

    m = ((MAX_RGB-MIN_RGB)/(temp_max-temp_min))*2;        //múltiplico por 2 pois o pico tem de ser a meio da reta
    b = temp_min + (fabsf(temp_min) + fabsf(temp_max))*0.5;

    //green
    *green = MAX_RGB - abs((_temperatura  - b) * m);

    //blue
    *blue = _temperatura < b ? 255 - (-1)*(_temperatura  - b) * m : 0;

    //red
    *red = _temperatura > b ? (_temperatura  - b) * m : 0;
}

/**
*calculo das coordenadas das cidades
*/
void RenderCity(SDL_Renderer * _renderer, int width, int height, dados_temp* cidade, DADOS *dados) {
    int red = 0, blue = 0, green = 0;
    int x = calculo_coordenada(cidade->longitude.angular,
        cidade->longitude.direcao, width, height, false);
    int y = calculo_coordenada(cidade->latitude.angular,
        cidade->latitude.direcao, width, height, true);

    RenderColor(cidade->temp, dados, &red, &green, &blue);
    filledCircleRGBA(_renderer, x, y, 5, red, green, blue);
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

/**
*Draws the map
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

/**
*Informa o utilizador sobre como varia as cores em função das temperaturas
*Informa ao utilizador das teclas necessárias para sair ou mudar para modo textual
*/
void RenderLegenda(SDL_Renderer * _renderer, TTF_Font *_font, DADOS *dados){
    SDL_Color white = {255, 255, 255, 255 };
    int red = 0, blue = 0, green = 0;
    char buffer[BUFFER_SIZE];

    //legenda com as temeraturas mínima  e máxima lidas
    sprintf(buffer, "Temp. min:\t%.2f\t Temp. max:\t%.2f\t", dados->citiesTempMin, dados->citiesTempMax);
    RenderText(50, 415, buffer, _font, &white, _renderer);

    //escala cromática
    int amplitude = fabsf(dados->citiesTempMax) + fabsf(dados->citiesTempMin);
    int tamanho = 360/amplitude;    //360 corresponde ao tamanho da legenda das temp min e max;
    SDL_Rect rect = { 40, 440, tamanho, 30 };

    for (int t = dados->citiesTempMin; t <= dados->citiesTempMax ; t++)
    {
        RenderColor( t , dados, &red, &green, &blue);
        SDL_SetRenderDrawColor(_renderer, red, green, blue, 255);
        SDL_RenderFillRect(_renderer, &rect);
        rect.x += tamanho;
    }
    RenderText(50, 600,"Para mais informacoes pressione 'i'", _font, &white, _renderer);
    RenderText(500, 630,"Pressione o rato para ampliar a imagem, e mova o rato (ZOOM em movimento)!!"
        , _font, &white, _renderer);
}

/**
*Desenha uma janela que dá informações ao utilizador sobre o funcionamento do programa
*/
void RenderInfo(SDL_Renderer * renderer, TTF_Font *font){
    SDL_Color pink = {255, 32, 122, 255 };
    SDL_Rect rect = { 500, 200, 400, 370 };
    SDL_SetRenderDrawColor(renderer, 93, 138, 168, 100);
    SDL_RenderFillRect(renderer, &rect);

    RenderText(550, 200, "-----WarmingUp-----Modo Grafico---", font, &pink, renderer);
    RenderText(500, 260, "Pressione espaco para pausa", font, &pink, renderer);
    RenderText(500, 300, "Pressione 't' para mudar para o modo Textual", font, &pink, renderer);
    RenderText(500, 340, "Pressione 'q' para sair do programa", font, &pink, renderer);
    RenderText(500, 380, "Pressione 'a' para:", font, &pink, renderer);
    RenderText(520, 410, "aumentar a velocidade de mudanca de ano", font, &pink, renderer);
    RenderText(540, 440, "(max: Velocidade: 1 (2 anos))", font, &pink, renderer);
    RenderText(500, 480, "Pressione 's' para:", font, &pink, renderer);
    RenderText(520, 510, "diminuir a velocidade de mudanca de ano", font, &pink, renderer);
    RenderText(540, 540, "(min :Velocidade: 1/10)", font, &pink, renderer);

}

/**
*Informa o utilizador que está a usar ZOOM à algum tempo
*/
void RenderDetective(SDL_Renderer * renderer, TTF_Font *font){
    SDL_Color red = {255, 0, 0, 255 };

    RenderText(540, 300, "Estas armado em detetive !??", font, &red, renderer);
}

/**
*Informa o utilizador da velocidade em que as informações sobre os anos são mostrados ao utilizador
*/
void RenderVelocityStatus(SDL_Renderer *renderer, TTF_Font *font, int velocidade) {
    char buffer[BUFFER_SIZE];
    SDL_Color white = { 255, 255, 255, 255 };

    if(velocidade == 0) {
        RenderText(50, 40, "Velocidade: 1 (2 anos)", font, &white, renderer);
    } else if(velocidade == 1) {
        RenderText(50, 40, "Velocidade: 1", font, &white, renderer);
    } else {
        sprintf(buffer, "Velocidade: 1/%d", velocidade);
        RenderText(50, 40, buffer, font, &white, renderer);
    }

}

/**
*Informa o utilizador das temperaturas do ano que estão a ser analisadas
*/
void RenderYear(SDL_Renderer *renderer, TTF_Font *font, int ano) {
    char buffer[BUFFER_SIZE];
    SDL_Color anoTextColor = { 255, 255, 255, 255 };

    sprintf(buffer, "%d", ano);
    RenderText(50, 20, buffer, font, &anoTextColor, renderer);
}

/**
*Desenha o simbolo da pausa no ecra
*/
void RenderPausa(SDL_Renderer *renderer, int pausaCounter) {
    SDL_Rect rect = { 10, 25, 10, 40 };
    int alpha = 100 + 155 * abs(pausaCounter - MAX_PAUSA_COUNTER/2) / (MAX_PAUSA_COUNTER/2);
    SDL_SetRenderDrawColor(renderer, 43, 168, 11, alpha);
    SDL_RenderFillRect(renderer, &rect);
    rect.x += 25;
    SDL_RenderFillRect(renderer, &rect);
}

/**
*Escreve o nome do País e da cidade no imagem , que corresponde à posição onde o rato se encontra
*/
void RenderSelectedCity(SDL_Renderer *renderer, TTF_Font *font, dados_temp *cidade) {
    char buffer[BUFFER_SIZE*2]; //BUFFER_SIZE*2 para tirar o warming do buffer talvez ser pequeno demais
    SDL_Color color = { 255, 255, 255, 255 };

    //nome do País e da Cidade
    sprintf(buffer, "%s, %s", cidade->pais, cidade->cidade);
    RenderText(WIDTH_WINDOW_SIZE-300, 0, buffer, font, &color, renderer);
    //temperatura
    sprintf(buffer, "%.2f C", cidade->temp);
    RenderText(WIDTH_WINDOW_SIZE-270, 25, buffer, font, &color, renderer);
}

/**
*Inicializa todas as entradas para cidades novas criadas no vetor
*/
void InitEmptyCidade(dados_temp *cidade) {
    cidade->cidade[0] = '\0';
}

/**
*Acresenta uma cidade ao vetor das cidades caso estas ainda não esteja no vetor
*/
void ColocaNodeNoVetorCidades(list_node_t *node, dados_temp **cidades, int **cidadesNumDados, int *vecCidadesSize) {
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

        for(int i = sizeAnterior; i<(*vecCidadesSize); i++) {
            InitEmptyCidade(&(*cidades)[i]);
            (*cidadesNumDados)[i] = 0;
        }

        //A primeira das novas entradas do vetor criadas é onde vai ser colocada a
        // cidade correspondente ao nó da lista
        //Faz uma cópia da cidade da payload da lista para a entrada do vetor
        (*cidades)[sizeAnterior] = *node->payload;
        (*cidadesNumDados)[sizeAnterior]++;
    }
}

/**
*Atualiza o vetor das cidades com valores de novas temperaturas
*/
void AtualizaCidades(DADOS *dados, list_node_t **aux, dados_temp **cidades, int *vecCidadesSize, int ano) {
    //Vetor que contem o numero de valores para cada cidade, para depois fazer a média
    //No cidades[i].temp vai ser guardada a soma de todas as temperaturas desse ano
    // e depois a temperatura final vai ser calculada dividindo pelo numero de dados
    int *cidadesNumDados = NULL;

    //Caso seja o primeiro ano ou se esteja a voltar para trás cria um vetor novo
    //Ao andar para trás ele analisa todos os anos desde o inicial até ao ano
    if(ano == dados->citiesAnoMin)
    {
        //Caso seja o primeiro ano inicializa o vetor

        //Se passar do ultimo ano de volta para o primeiro apaga os dados no vetor
        for(int i = 0; i<(*vecCidadesSize); i++) {
            InitEmptyCidade(&(*cidades)[i]);
        }
        *aux = dados->headCitiesOriginal->next;
    }

    //Fazer com que o vetor tenha o mesmo tamanho que o das cidades
    cidadesNumDados = checkedMalloc((*vecCidadesSize) * sizeof(int));


    //Inicializa o numDados a zero para todas as cidades
    for(int i = 0; i<(*vecCidadesSize); i++) {
        cidadesNumDados[i] = 0;
        (*cidades)[i].tempAnterior = (*cidades)[i].temp;
    }



    //Avança o aux até ao ano que se quer
    while(*aux != NULL && (*aux)->payload->dt.ano < ano) {
        *aux = (*aux)->next;
    }

    //Começando com o aux a apontar para o primeiro elemento da lista com dt.ano == ano
    while(*aux != NULL && (*aux)->payload->dt.ano == ano) {
        ColocaNodeNoVetorCidades(*aux, cidades, &cidadesNumDados, vecCidadesSize);

        *aux = (*aux)->next;
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

/**
*Incrementa o ano no vetor das cidades em uma unidade
*/
void MudarAno(DADOS *dados, list_node_t **aux, dados_temp **cidades, int *vecCidadesSize, int *ano) {
    //Aumenta o ano
    //Se passar dos limites "dá a volta"
    (*ano)++;
    if(*ano > dados->citiesAnoMax) {
        *ano = dados->citiesAnoMin;
    }
    //atualiza os valores das temperaturas no vetor das cidades
    AtualizaCidades(dados, aux, cidades, vecCidadesSize, *ano);
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

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _renderer renderer to handle all rendering in a window
 * \return pointer to the texture
 */
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
