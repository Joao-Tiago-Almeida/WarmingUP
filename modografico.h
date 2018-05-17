#ifndef _MODO_GRAFICO_H
#define _MODO_GRAFICO_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "conjuntodados.h"

void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer **);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int);
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
void RenderMap( SDL_Surface **, SDL_Renderer *, int width, int height);
void filledCircleRGBA(SDL_Renderer * _renderer, float _circleX, float _circleY, int _circleR, int _r, int _g, int _b);
void RenderCircle(SDL_Renderer * _renderer, int width, int height, TTF_Font *_font );
int calculo_coordenada( float coord_local, int _direcao, int _widht, int _height, bool latituede);
bool modoGrafico(char *nomeFilePaises, char *nomeFileCidades, DADOS *dados);
void RenderColor(SDL_Renderer * _renderer, int _temperatura, int _latitude, int _longitude);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);

#endif
