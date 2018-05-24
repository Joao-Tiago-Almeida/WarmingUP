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
void RenderCircle(SDL_Renderer * _renderer, int width, int height, TTF_Font *_font , DADOS *dados);
int calculo_coordenada( float coord_local, int _direcao, int _widht, int _height, bool latituede);
bool ModoGrafico(char *nomeFileCidades, DADOS *dados);
int GetSelectedCity(dados_temp* cidades, int vecCidadesSize, int zoomPosX, int zoomPosY);
void RenderCity(SDL_Renderer * _renderer, int width, int height, dados_temp* cidade, DADOS *dados);
void RenderLegenda(SDL_Renderer * _renderer, TTF_Font *_font, DADOS *dados);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
void RenderVelocityStatus(SDL_Renderer *renderer, TTF_Font *font, int velocidade);
void RenderYear(SDL_Renderer *renderer, TTF_Font *font, int ano);
void RenderPausa(SDL_Renderer *renderer, int pausaCounter);
int CalcDistance(int x1, int y1, int x2, int y2);
void RenderSelectedCity(SDL_Renderer *renderer, TTF_Font *font, dados_temp *cidade);
SDL_Texture *CreateRenderTexture(SDL_Renderer* _renderer, int width, int height);
void RenderCities(SDL_Renderer *renderer, DADOS* dados, dados_temp* cidades, int vecCidadesSize);
void RenderZoom(SDL_Renderer* renderer, SDL_Texture* renderTexture, int zoomPosX, int zoomPosY);
void InitEmptyCidade(dados_temp *cidade);
void ColocaNodeNoVetorCidades(list_node_t *node, dados_temp **cidades, int **cidadesNumDados, int *vecCidadesSize);
void AtualizaCidades(DADOS *dados, dados_temp **cidades, int *vecCidadesSize, int ano);
void MudarAno(DADOS *dados, dados_temp **cidades, int *vecCidadesSize, int *ano);
void RenderColor(int _temperatura, DADOS *dados, int* red, int* blue, int* green);
void RenderInfo(SDL_Renderer * _renderer, TTF_Font *_font);

#endif
