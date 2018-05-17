#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "modografico.h"
#include "data.h"

#define M_PI 3.14159265358979323846264338327950288
#define STRING_SIZE 100       // max size for some strings
#define HEIGHT_WINDOW_SIZE 850
#define WIDTH_WINDOW_SIZE 1390
#define LATITUDE_0 425      //latitude de coordenada 0,  latitude de referencia
#define LONGITUDE_0 695     //longitude de coordenada 0, longitude de referncia
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
    bool stay = true;
    int width = WIDTH_WINDOW_SIZE;
    int height = HEIGHT_WINDOW_SIZE;
    int delay = 0;
    bool modo_texto = false;

    if(dados->headCitiesOriginal == NULL)  read_file_cities (dados,nomeFileCidades);

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
                    case SDLK_t:
                        stay = 0;
                        modo_texto = true;
                        break;
                    case SDLK_q:
                        stay = false;
                    default:
                        break;
                }
            }
        }
        // render game table
        RenderMap( imgs, renderer, width, height);
        // render a circle around the country
        RenderCircle( renderer ,width, height, AppleGaramond);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(AppleGaramond);
    SDL_FreeSurface(imgs[0]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return modo_texto;
}

//recebe dois argumentos e faz o calculo entre eles
//true, se for latitude, false se longitude
//a coord_local é inicialmente a coordenada antes da transformacao , ficheiro
//cai guardar temporariamente o deslocamnetonum eixo  da coordenada (0,0) à cidade
//e dps vai ter a coordenada exata do pais
//bool:: calculo da latitude ? => true , longitude => false
int calculo_coordenada(float coord_local, int _direcao, int _widht, int _height, bool latitude)
{
    //TODO escrever melhor
    //a divisão por dois justifica se pois o fator de deslocamento por pixel
    //recaí sobre uma distância total e apartir do centro da imagem
    //o deslocamento máximo para cada lado pode no máximo ser metade da distÂncia total
    float latitude_por_pixel = (float)_height/(2.0f*LATITUDE_MAX);
    float longitude_por_pixel = (float)_widht/(2.0f*LONGITUDE_MAX);

    //permite defenir quando se adiciona ou se subtrai

    int indice = _direcao % 2 == 0 ? 1 : -1;

    if(latitude)
    {
        coord_local = LATITUDE_0 + indice * (latitude_por_pixel * coord_local);
    }
    else
    {
        coord_local = LONGITUDE_0 + indice * (longitude_por_pixel * coord_local);
    }

    return coord_local;
}

//calcula a cor em função da temperatura
//TODO falta adicionar a temperatura maxima e minima do ficheiro
//TODO pode ser melhorado apenas com uma função
void RenderColor(SDL_Renderer * _renderer, int _temperatura, int _latitude, int _longitude)
{
    //m representa o declive que permite calcular componente verde
    int m = 0, b = 0, green = 0;

    //frio
    if(_temperatura<TEMP_REF)
    {
        //cresce à medida que a temperatura diminui
        m = MAX_RGB/(TEMP_REF-TEMP_MIN);
        //achar o b sabendo que se quando a temperatura fosse 10 o valor de verde seria máximo
        b = MAX_RGB - m * TEMP_REF;
        //calculo do verde em função da _temperatura
        green = m*_temperatura + b;
        filledCircleRGBA(_renderer, _latitude, _longitude, 5, 0, green, 255);
    }
    //quente
    else
    {
        //decresce à medida que a temperatura diminui
        m = MAX_RGB/(TEMP_REF-TEMP_MAX);
        //achar o b sabendo que quando a temperatura é 10 o valor de verde é máximo
        b = MAX_RGB - m * TEMP_REF;
        //calculo do verde em função da _temperatura
        green = m*_temperatura + b;

        filledCircleRGBA(_renderer, _latitude, _longitude, 5, 255, green, 0);
    }
    //printf("temperatura == %d\ngreen:: %d\n", _temperatura, green);
}

//TODO a cor não percisa de ser ponteiro duplo
void RenderCircle(SDL_Renderer * _renderer, int width, int height, TTF_Font *_font )
{
    float latitude = 0.0f;
    float longitude = 0.0f;
    SDL_Color white = {255, 255, 255 };

    //UNITED KINGDOM    ?? LONDON
    latitude = calculo_coordenada(54.0 , NORTE , width, height, true);
    longitude = calculo_coordenada(2.30, OESTE, width, height, false);
    //filledCircleRGBA(_renderer, latitude, longitude, 5, 0, 0, 255);
    RenderColor(_renderer, -40, latitude, longitude);

    //LISBON
    latitude = calculo_coordenada(38.43 , NORTE , width, height, true);
    longitude = calculo_coordenada(9.10, OESTE, width, height, false);
    //filledCircleRGBA(_renderer, latitude, longitude, 5, 0, 255, 0);
    RenderColor(_renderer, 10, latitude, longitude);

    //SIDNEY
    latitude = calculo_coordenada(33.51 , SUL , width, height, true);
    longitude = calculo_coordenada(151.12, ESTE, width, height, false);
    //filledCircleRGBA(_renderer, latitude, longitude, 5, 255, 255, 0);
    RenderColor(_renderer, 50, latitude, longitude);


    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
    SDL_Rect legenda = {50, 600, 100, 60};
    SDL_RenderFillRect(_renderer, &legenda);
    char frio[] = "frio";
    char quente[] = "quente";
    RenderText(80, 615, frio, _font, &white, _renderer);
    RenderText(70, 630, quente, _font, &white, _renderer);
    filledCircleRGBA(_renderer, 625, 60, 5, 0, 0, 255);
    filledCircleRGBA(_renderer, 625, 125, 5, 0, 255, 255);
    filledCircleRGBA(_renderer, 640, 60, 5, 255, 255, 0);
    filledCircleRGBA(_renderer, 640, 125, 5, 255, 0, 0);


    //filledCircleRGBA(_renderer, LATITUDE_0, LONGITUDE_0, 5, 255, 0, 0);
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
void filledCircleRGBA(SDL_Renderer * _renderer, float _circleY, float _circleX, int _circleR, int _r, int _g, int _b)
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


    // clear the window
    SDL_RenderClear( _renderer );

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

    return renderer;
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
