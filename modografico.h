#ifndef _MODO_GRAFICO_H
#define _MODO_GRAFICO_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer **);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int);
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
void RenderTable( SDL_Surface **, SDL_Renderer *);

void modoGrafico(char *nomeFileCidades);

#endif
