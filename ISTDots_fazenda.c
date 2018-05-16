/**
 * ISTDots
 * Autor: Miguel Fazenda IST190146
 * Projeto intermedio de unidade curricular de programacao
 * Marco-Abril 2018
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#define MAX(a,b)    (((a)>(b))?(a):(b))
#define SQR(a) (a)*(a)
#define M_PI 3.14159265
#define STRING_SIZE 100       // max size for some strings
//#define TABLE_SIZE 850        // main game space size
#define TABLE_SIZE 650        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 15      // maximum size of the board
#define MAX_COLORS 5
#define MARGIN 5
#define MAX_SELECTION_SIZE MAX_BOARD_POS*MAX_BOARD_POS


#define MAX_OBJETIVO 99
#define MAX_PLAYS 100
#define MAX_NAME_LENGHT 8
#define MAX_NAME_ARRAY_SIZE MAX_NAME_LENGHT+1
#define MAX_MOVES 99
#define MIN_COLORS 2
#define MAX_FROZEN_PLACES 5

#define STATS_RECT_MOVES_X 30
#define STATS_RECT_Y 50
#define STATS_RECT_WIDTH 80
#define STATS_RECT_HEIGHT 40
#define STATS_SPACE_FOR_GOALS TABLE_SIZE - (STATS_RECT_MOVES_X + STATS_RECT_WIDTH)

#define STATE_TEXT_RENDER_X 150
#define STATE_TEXT_RENDER_Y STATS_RECT_Y + STATS_RECT_HEIGHT - 5
#define FONT_BIG_SIZE 24

#define GAME_STATE_PLAYING 1
#define GAME_STATE_LOST 2
#define GAME_STATE_WON 3
#define GAME_STATE_EXCEDED_MAX_PLAYS 4
#define GAME_STATE_DEFINE_FROZEN_PLACES 5

#define ANIMATION_NONE 0
#define ANIMATION_SHUFFLE 1

#define CORNER_LEFT 0
#define CORNER_RIGHT 1
#define HORIZONTAL 2
#define VERTICAL 3

//Este define serve para testar a funcionalidade dos quadrados
//Quando definido, a tecla d mete o ponto em baixo do rato a azul
//E quando se cria um quadrado ele apenas apaga o que está dentro e os pontos nao caem
//#define DEBUG_POLYGON

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, TTF_Font **, TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable(int [2], int [], int, TTF_Font *, SDL_Surface **, SDL_Renderer *);
void ProcessMouseEvent(int , int , int [], int , int *, int *, bool);
void RenderPoints(int [][MAX_BOARD_POS], int [2], int [], int, SDL_Renderer *);
void RenderStats( SDL_Renderer *, TTF_Font *, int [], int , int );
void filledCircleRGBA(SDL_Renderer * , int , int , int , int , int , int );

void SetAnimation(int, int*, int*);
void GenerateFrozenPlaces(int*, int [][MAX_BOARD_POS], int [2]);
void MouseClickCreateDestroyFrozenPlaces(unsigned char, int*, int [][MAX_BOARD_POS], int, int);

void RenderGameStateText(int, TTF_Font*, SDL_Color*, SDL_Renderer*, int);

void RenderSelection(int [][2], int, int, int [], int, SDL_Renderer*);

void SuffleBoard(int [][MAX_BOARD_POS], int [2]);
bool CheckIfThereArePossiblePlays(int [][MAX_BOARD_POS], int boardDimensions[2]);
int CheckIfLostOrWonAndUpdateState(int*, int, int [], int);

int IsValidPoint(int, int, int [2]);

void ClearSelection(int [][2], int*, bool*, int*);
void DestroySelection(int [][2], int, int [][MAX_BOARD_POS]);
void DestroyAllDotsOfColor(int, int [][MAX_BOARD_POS], int [2], int [MAX_COLORS]);

bool CoordListContains(int, int, int [][2], int, int);
int CoordListIndexOf(int, int, int [][2], int, int);
int GetPrevSelectionCoord(int, int, int);
int GetNextSelectionCoord(int, int, int);
int GetSelectionMakesCornerSide(int [][2], int, int, int);
void DestroySelectionSquare(int [][2], int, int, int [][MAX_BOARD_POS], int [2], int, int []);

void UpdateAnimation(int*, int*);

void UpdateBoard(bool*, int [][MAX_BOARD_POS], int [], int, int*, int*, int*);
void ClearBoard(int [][MAX_BOARD_POS], int []);
void PreventNegativeGoals(int [], int);
bool BoardFall(int [][MAX_BOARD_POS], int [2], int, int*);

void AddToSelectionIfNotAddedYet(int [][2], int, int, int*, int*);
int IsPointNextToLastSelected(int [][2], int, int, int);

void AquireGameParameters(int [2], char [], int*, int[], int*);
void AquireParamBoardSize(int [2]);
void AquireParamName(char []);
void AquireParamNumColors(int*);
void AquireParamMoves(int*);
void AcquireParamGoals(int [], int*);

void GenerateBoard(int boardDimensions[2], int [][MAX_BOARD_POS], int);
void ResetGame(int [MAX_COLORS], int [MAX_COLORS], int*, int*, int [2], int [][MAX_BOARD_POS], int);

int RegisterPlay(int [][2], int*, int, bool);
void CountWinsAndLossesOnPlayHistory(int [][2], int, int*, int*);
int WritePlayHistory(char [MAX_NAME_ARRAY_SIZE], int [][2], int);

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Miguel Fazenda";
const char myNumber[] = "IST190146";
const int colors[3][MAX_COLORS] = {{91, 170, 54, 255, 253},{32, 240, 61, 174, 118},{94, 183, 105, 3, 144}};
SDL_Color black = { 0, 0, 0 };
SDL_Color green = { 0, 153, 0 };
SDL_Color statsRectColor = { 100, 100, 100 };
SDL_Color stateColor = { 0, 255, 0 };

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( void )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    TTF_Font *fontSans = NULL; //Font para o texto do estado
    TTF_Font *serifBig = NULL; //Font para o texto maior
    SDL_Surface *imgs[2];
    SDL_Event event;
    int delay = 100;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px = 0, board_size_px[2] = {0};
    int boardDimensions[2] = {0, 0};
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int pt_x = 0, pt_y = 0;

    bool selecting = 0; //Indica se esta a ser realizada uma selecao
    int selectionSize = 0; //Tamanho da selecao
    int selection[MAX_SELECTION_SIZE][2] = {{0}}; //Vetor que contem as coordenadas dos pontos selecionados (na verdade e uma matriz)
    int selectingColor = 0; //A cor que está a ser selecionada
    int selectionFormingSquareIndex = -1; //Indice da selecao de onde comeca o quadrado (-1 caso nao esteja a formar um quadrado)

    //Parametros do jogo
    char name[9] = "";
    int num_colors = 0;
    int goalsTotal[MAX_COLORS] = {0}; //Esta variavel nao muda durante o jogo. Serve para quando carregar no n e fazer reset ao jogo
    int goals[MAX_COLORS] = {0};
    int movesTotal = 0; //Esta variavel nao muda durante o jogo. Serve para quando carregar no n e fazer reset ao jogo
    int moves = 0;

    bool boardMoving = 0; //Caso hajam pontos a cair
    int colorSquareErased = -1; //A cor que nao deve cair apos criar um quadrado (-1 quando deve gerar todas as cores)

    int state = GAME_STATE_DEFINE_FROZEN_PLACES; //Estado do jogo

    //Contem o numero de sitios que o jogador ja escolheu para estarem congelados
    int numFrozenPlacesPlaced = 0;

    //Historico de jogos. Serve para ser escrito o stats.txt
    int playHistory[MAX_PLAYS][2] = {{0}}; //Vector com o numero de movimentos e se ganhou ou perdeu (na verdade e uma matriz)
    int playHistorySize = 0; //Numero de jogos registados

    int animation = ANIMATION_NONE;
    int animationTime = 0;

    bool nButtonState = 0; //Serve para nao gerar varios jogos enquanto o butao n estiver primido

    char buf[11 + MAX_NAME_LENGHT];
    
    //Definir a seed para gerar numeros aleatorios
    srand(1234);

    //Adquirir os parametros do jogo do terminal
    AquireGameParameters(boardDimensions, name, &num_colors, goalsTotal, &movesTotal);
    moves = movesTotal;
    memcpy(goals, goalsTotal, MAX_COLORS*sizeof(int));

    ClearBoard(board, boardDimensions);
    ClearSelection(selection, &selectionSize, &selecting, &selectionFormingSquareIndex);

    // initialize graphics
    InitEverything(width, height, &serif, &fontSans, &serifBig, imgs, &window, &renderer);

    sprintf(buf, "ISTDots - %s", name);
    SDL_SetWindowTitle(window, buf);

    while( quit == 0 )
    {
        // **** Inputs ****
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    #ifdef DEBUG_POLYGON
                    int x, y, pt_x, pt_y;
                    #endif
                    case SDLK_n:
                        if(!nButtonState) {
                            if(state == GAME_STATE_LOST || state == GAME_STATE_WON || state == GAME_STATE_DEFINE_FROZEN_PLACES) {
                                //Começa um novo jogo 
                                state = GAME_STATE_PLAYING;
                                ResetGame(goalsTotal, goals, &movesTotal, &moves, boardDimensions, board, num_colors);
                            } else if(state == GAME_STATE_PLAYING) {
                                //Jogador destistiu
                                RegisterPlay(playHistory, &playHistorySize, movesTotal-moves, 0);

                                //Verificar se ja nao pode jogar mais vezes
                                if(playHistorySize >= MAX_PLAYS) {
                                    //Muda o estado do jogo
                                    state = GAME_STATE_EXCEDED_MAX_PLAYS;
                                } else {
                                    //Começa um novo jogo
                                    ResetGame(goalsTotal, goals, &movesTotal, &moves, boardDimensions, board, num_colors);
                                }
                            }
                            nButtonState = 1;
                        }
                        break;
                    case SDLK_q:
                        quit = 1;
                        break;
                    case SDLK_r:
                        if(state == GAME_STATE_DEFINE_FROZEN_PLACES) {
                            GenerateFrozenPlaces(&numFrozenPlacesPlaced, board, boardDimensions);
                        }
                        break;
                    case SDLK_d:
                        #ifdef DEBUG_POLYGON
                        SDL_GetMouseState(&x, &y);
                        ProcessMouseEvent(x, y, board_size_px, square_size_px, &pt_x, &pt_y, 0);
                        board[pt_x][pt_y] = 1;
                        #endif
                        break;
                    default:
                        break;
                }
            } else if ( event.type == SDL_KEYUP ) {
                if(event.key.keysym.sym == SDLK_n) {
                    nButtonState = 0;
                }
            } else if ( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if(state == GAME_STATE_PLAYING) {
                    ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y, 1);
                    if(IsValidPoint(pt_x, pt_y, boardDimensions)) {
                        //Start the selection
                        selecting = 1;
                        selectingColor = board[pt_x][pt_y];
                        AddToSelectionIfNotAddedYet(selection, pt_x, pt_y, &selectionSize, &selectionFormingSquareIndex);
                    }
                } else if(state == GAME_STATE_DEFINE_FROZEN_PLACES) {
                    ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y, 0);
                    if(IsValidPoint(pt_x, pt_y, boardDimensions)) {
                        MouseClickCreateDestroyFrozenPlaces(event.button.button, &numFrozenPlacesPlaced, board, pt_x, pt_y);
                    }
                }
            }
            else if ( event.type == SDL_MOUSEBUTTONUP )
            {
                if(state == GAME_STATE_PLAYING) {
                    if(selectionSize > 1) {
                        int lostOrWon;
                        moves--;
                        boardMoving = 1;
                        
                        //Destuir os pontos
                        if(selectionFormingSquareIndex != -1) {
                            //Caso estaja a formar um poligono
                            DestroySelectionSquare(selection, selectionSize, selectingColor, board, boardDimensions, selectionFormingSquareIndex, goals);
                            colorSquareErased = selectingColor;
                            #ifndef DEBUG_POLYGON
                            DestroyAllDotsOfColor(selectingColor, board, boardDimensions, goals);
                            #endif
                            #ifdef DEBUG_POLYGON
                            boardMoving = 0;
                            #endif
                        } else {
                            goals[selectingColor] -= selectionSize;
                            DestroySelection(selection, selectionSize, board);
                        }

                        PreventNegativeGoals(goals, num_colors);

                        lostOrWon = CheckIfLostOrWonAndUpdateState(&state, moves, goals, num_colors);
                        if(lostOrWon != -1) {
                            RegisterPlay(playHistory, &playHistorySize, movesTotal-moves, lostOrWon);
                            
                            //Verificar se ja nao pode jogar mais vezes
                            if(playHistorySize >= MAX_PLAYS) {
                                state = GAME_STATE_EXCEDED_MAX_PLAYS;
                            }
                        }
                    }
                    
                    
                    ClearSelection(selection, &selectionSize, &selecting, &selectionFormingSquareIndex);
                }
            }
            else if ( event.type == SDL_MOUSEMOTION )
            {
                if(state == GAME_STATE_PLAYING) {
                    ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y, 1);
                    
                    if(selecting && IsValidPoint(pt_x, pt_y, boardDimensions)) {
                        if(board[pt_x][pt_y] == selectingColor && IsPointNextToLastSelected(selection, selectionSize, pt_x, pt_y)) {
                            //Select the dot
                            AddToSelectionIfNotAddedYet(selection, pt_x, pt_y, &selectionSize, &selectionFormingSquareIndex);
                        }
                    }
                }
            }
        }
        
        // **** Update ****
        UpdateAnimation(&animation, &animationTime);
        UpdateBoard(&boardMoving, board, boardDimensions, num_colors, &animation, &animationTime, &colorSquareErased);

        // **** Render ****
        // render game table
        square_size_px = RenderTable(boardDimensions, board_size_px, state, serif, imgs, renderer);
        // render board
        RenderPoints(board, boardDimensions, board_size_px, square_size_px, renderer);
        // render selection
        RenderSelection(selection, selectionSize, selectingColor, board_size_px, square_size_px, renderer);
        // render stats
        RenderStats(renderer, fontSans, goals, num_colors, moves);
        
        RenderGameStateText(state, serifBig, &stateColor, renderer, numFrozenPlacesPlaced);
        
        //Render animations
        if(state == GAME_STATE_PLAYING && animation == ANIMATION_SHUFFLE) {
            //Fazer com que o texto a indicar shuffle mexa um bocadinho
            RenderText(STATE_TEXT_RENDER_X+30+sin(animationTime+rand()%5/2.0f)*10, STATE_TEXT_RENDER_Y+30+cos(animationTime+rand()%5/2.0f)*10, "SHUFFLE", serifBig, &stateColor, renderer);
        }
        

        // render in the screen all changes above
        SDL_RenderPresent(renderer);
       
        // add a delay
        SDL_Delay( delay );
    }

    if(state == GAME_STATE_PLAYING) {
        //Caso tenha saido a meio de um jogo, regista esse como derrota
        RegisterPlay(playHistory, &playHistorySize, movesTotal-moves, 0);
    }


    //Escrever o ficheiro stats.txt
    if(WritePlayHistory(name, playHistory, playHistorySize) == 0) {
        //Houve um erro a escrever o ficheiro
        perror("Erro a escrever o ficheiro stats.txt");
    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    TTF_CloseFont(fontSans);
    TTF_CloseFont(serifBig);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void SetAnimation(int anim, int* animation, int* animationTime) {
    *animation = anim;
    *animationTime = 0;
}

void GenerateFrozenPlaces(int* numFrozenPlacesPlaced, int board[][MAX_BOARD_POS], int boardDimensions[2]) {
    if((*numFrozenPlacesPlaced) >= MAX_FROZEN_PLACES) {
        return;
    }
    bool placed = 0;
    while(!placed && (*numFrozenPlacesPlaced) < boardDimensions[0]*boardDimensions[1]) {
        int randX = rand()%boardDimensions[0];
        int randY = rand()%boardDimensions[1];

        if(board[randX][randY] != -2) {
            board[randX][randY] = -2;
            placed = 1;
            (*numFrozenPlacesPlaced)++;
        }
    }
}

void MouseClickCreateDestroyFrozenPlaces(unsigned char button, int *numFrozenPlacesPlaced, int board[][MAX_BOARD_POS], int pt_x, int pt_y) {
    if(button == SDL_BUTTON_LEFT) {
        //Adicionar um quadrado congelado
        //Nao adicionar se houverem demais
        if((*numFrozenPlacesPlaced) >= MAX_FROZEN_PLACES) {
            return;
        }

        if(board[pt_x][pt_y] != -2) {
            (*numFrozenPlacesPlaced)++;
            board[pt_x][pt_y] = -2;
        }
    } else if(button == SDL_BUTTON_RIGHT) {
        //Remover o quadrado congelado
        if(board[pt_x][pt_y] == -2) {
            (*numFrozenPlacesPlaced)--;
            board[pt_x][pt_y] = -1;
        }
    }
}

void SuffleBoard(int board[][MAX_BOARD_POS], int boardDimensions[2]) {
    //Faz shuffle. O loop faz com que caso o shuffle meta os pontos de forma impossivel de jogar outra vez, faz shuffle de novo
    do {
        int colors[MAX_COLORS] = {0}; //Vector que tem o numero de pontos do tabuleiro de cada cor
        int numPoints = boardDimensions[0]*boardDimensions[1];

        //Contar quantos pontos de cada cor ha, e quantos pontos existem no total
        for(int i = 0; i<boardDimensions[0]; i++) {
            for(int j = 0; j<boardDimensions[1]; j++) {
                int color = board[i][j];
                if(color >= 0) {
                    //Se for uma cor valida, conta-a
                    colors[color]++;
                }
            }
        }

        for(int i = 0; i<boardDimensions[0]; i++) {
            for(int j = 0; j<boardDimensions[1]; j++) {
                if(board[i][j] < 0) {
                    continue; //Ignorar as casas congeladas
                }

                //Gera um numero aleatorio entre 0 e o numero de pontos a que faltam atribuir cor
                int a = rand()%numPoints;

                
                //Vai a cada cor e subtrai o a por quantos pontos dessa cor ainda faltam atribuir
                //Sendo numPoints colors[0]+colors[1]+...+colors[4]
                //Se, por exemplo, a for colors[0]+colors[1]+x (sendo x<colors[2]), então é porque o a corresponde à cor 2
                for(int c = 0; c<MAX_COLORS; c++) {
                    if(colors[c] > 0) {
                        a -= colors[c];
                        if(a <= 0) {
                            colors[c]--;
                            board[i][j] = c;
                            numPoints--;
                            break;
                        }
                    }
                }
            }
        }
    } while(!CheckIfThereArePossiblePlays(board, boardDimensions));
}

/**
 * CheckIfThereArePossiblePlays devolve 1 caso ainda haja jogadas possiveis, 0 caso contrario 
 */
bool CheckIfThereArePossiblePlays(int board[][MAX_BOARD_POS], int boardDimensions[2]) {
    for(int i = 0; i<boardDimensions[0]; i++) {
        for(int j = 0; j<boardDimensions[1]; j++) {
            int color = board[i][j];
            if(color == -2) {
                continue;
            }
            
            //Ver se o ponto a esquerda tem a mesma cor que este
            if(i > 0 && board[i-1][j] == color)
                return 1;

            //Direita
            if(i<boardDimensions[0]-1 && board[i+1][j] == color)
                return 1;

            //Cima
            if(j>0 && board[i][j-1] == color)
                return 1;

            //Baixo
            if(j<boardDimensions[1]-1 && board[i][j+1] == color)
                return 1;
        }
    }

    return 0;
}

/**
 * ResetGame: Esta funcao serve para quando se inicia novo jogo
 *      Volta a definir os movimentos restantes e os objetivos para os definidos nos parametros iniciais
 *      E gera o tabuleiro de novo
 */
void ResetGame(int goalsTotal[MAX_COLORS], int goals[MAX_COLORS], int* movesTotal, int* moves, int boardDimensions[2], int board[][MAX_BOARD_POS], int num_colors) {
    *moves = *movesTotal;
    memcpy(goals, goalsTotal, MAX_COLORS*sizeof(int));
    GenerateBoard(boardDimensions, board, num_colors);
}

/**
 * RegisterPlay: Regista um jogo no historico de jogos (se ganhou ou nao e o numero de jogadas) 
 *      Devolve 0 caso nao tenha conseguido registar na matriz
 */
int RegisterPlay(int playHistory[][2], int* numPlaysRegistered, int moves, bool won) {
    if((*numPlaysRegistered) >= MAX_PLAYS) {
        return 0;
    }

    playHistory[(*numPlaysRegistered)][0] = moves;
    playHistory[(*numPlaysRegistered)][1] = (int)won;

    (*numPlaysRegistered)++;

    return 1;
}

/**
 * CountWinsAndLossesOnPlayHistory: conta o numero de ganhos e perdas no historico de jogos
 */
void CountWinsAndLossesOnPlayHistory(int playHistory[][2], int playHistorySize, int* wins, int* losses) {
    *wins = 0;
    *losses = 0;

    for(int i = 0; i<playHistorySize; i++) {
        if(playHistory[i][0] == -1) {
            //Caso chegue ao fim do vetor de historico de jogos, para o loop
            break;
        }

        //Contabilizar um ganho ou perda
        if(playHistory[i][1]) {
            (*wins)++;
        } else {
            (*losses)++;
        }
    }
}


/**
 * WritePlayHistory: Escreve o ficheiro stats.txt
 * Devolve 0 se houver erro, 1 caso contrario
 */
int WritePlayHistory(char name[MAX_NAME_ARRAY_SIZE], int playHistory[][2], int playHistorySize) {
    int wins = 0, losses = 0;
    if(playHistorySize == 0) {
        //Caso nao tenha havido nenhum jogo nao vale a pena escrever
        return 1;
    }

    FILE* file = fopen("stats.txt", "w");
    if(file == NULL) {
        return 0;
    }

    CountWinsAndLossesOnPlayHistory(playHistory, playHistorySize, &wins, &losses);

    fprintf(file, "\n%s %d %d %d\n", name, playHistorySize, wins, losses);
    for(int i = 0; i<playHistorySize; i++) {
        int moves = playHistory[i][0];
        int won = playHistory[i][1];
        if(won) {
            fprintf(file, "   %d V\n", moves);
        } else {
            fprintf(file, "   D\n");
        }
        
    }

    if(fclose(file) != 0) {
        return 0;
    }

    return 1;
}

//Devolve 0 caso tenha perdido
//Devolve 1 caso tenha ganho
//Devolve -1 caso ainda esteja a jogar
int CheckIfLostOrWonAndUpdateState(int* state, int moves, int goals[], int num_colors) {
    if(*state == GAME_STATE_PLAYING) {
        //Ver se perdeu
        if(moves < 1) {
            for(int i = 0; i<num_colors; i++) {
                if(goals[i] > 0) {
                    *state = GAME_STATE_LOST;
                    return 0; //Caso tenha perdido sai da funcao, assim nao vai sequer ver se ganhou
                }
            }
        }

        //Ver se ganhou
        bool won = 1;
        for(int i = 0; i<num_colors; i++) {
            if(goals[i] > 0) {
                won = 0;
                break;
            }
        }

        if(won) {
            *state = GAME_STATE_WON;
            return 1;
        }
    }

    return -1;
}

//Verifica se o ponto de coordenadas x,y esta dentro do tabuleiro
int IsValidPoint(int x, int y, int boardDimensions[2]) {
    return x > -1 && y > -1 && x < boardDimensions[0] && y < boardDimensions[1];
}

//Limpa a selecao de dots (metendo todas as coordenadas a -1)
void ClearSelection(int selection[][2], int* selectionSize, bool* selecting, int* selectionFormingSquareIndex) {
    *selecting = 0;
    *selectionFormingSquareIndex = -1;
    *selectionSize = 0;
    for(int i =0; i<MAX_SELECTION_SIZE; i++) {
        selection[i][0] = -1;
        selection[i][1] = -1;
    }
}

//Adiciona ou tira um ponto da selecao
void AddToSelectionIfNotAddedYet(int selection[][2], int x, int y, int* selectionSize, int* selectionFormingSquareIndex) {
    for(int i =0; i<MAX_SELECTION_SIZE; i++) {
        //Caso ja esteja na lista
        if(selection[i][0] == x && selection[i][1] == y) {
            if(i < (*selectionSize)-2) {
                //Se nao tiver a voltar a tras e porque está a formar um quadrado
                if(*selectionFormingSquareIndex == -1) {
                    selection[(*selectionSize)][0] = x;
                    selection[(*selectionSize)][1] = y;
                    (*selectionSize)++;

                    *selectionFormingSquareIndex = i;

                    return;
                }
            } else {
                //Se tiver a voltar a tras, tira da selecao
                (*selectionSize)--;
                selection[(*selectionSize)][0] = -1;
                selection[(*selectionSize)][1] = -1;

                *selectionFormingSquareIndex = -1;

                return;
            }
        }

        if((*selectionFormingSquareIndex) == -1) {
            //Quando encontra um espaco vazio na lista de dots selecionados
            //Adiciona a selecao
            if(selection[i][0] == -1) {
                selection[i][0] = x;
                selection[i][1] = y;
                (*selectionSize)++;
                return;
            }
        }
    }
}

int IsPointNextToLastSelected(int selection[][2], int selectionSize, int x, int y) {
    int lastSelectedX = selection[selectionSize-1][0];
    int lastSelectedY = selection[selectionSize-1][1];

    return (lastSelectedX == x-1 && lastSelectedY == y) ||
        (lastSelectedX == x+1 && lastSelectedY == y) ||
        (lastSelectedX == x && lastSelectedY == y-1) ||
        (lastSelectedX == x && lastSelectedY == y+1);
}

//Destroi os pontos selecionados do tabuleiro (mete a -1 no tabuleiro)
void DestroySelection(int selection[][2], int selectionSize, int board[][MAX_BOARD_POS]) {
    for(int i =0; i<selectionSize; i++) {
        int x = selection[i][0], y = selection[i][1];
        
        board[x][y] = -1;
    }
}

//Destroi todos os pontos de uma cor do tabuleiro
//Devolve a quantidade de pontos dessa cor destruidos
void DestroyAllDotsOfColor(int color, int board[][MAX_BOARD_POS], int boardDimensions[2], int goals[MAX_COLORS]) {
    int count = 0;
    for(int i = 0; i<boardDimensions[0]; i++) {
        for(int j =0; j<boardDimensions[1]; j++) {
            if(board[i][j] == color) {
                count++;
                board[i][j] = -1;
            }
        }
    }
    goals[color] -= count;
}

/**
 * CoordListContains checks if the list contains a point x,y
 * ignores all the selected coords before the offset index
 */
bool CoordListContains(int x, int y, int list[][2], int listSize, int offset) {
    for(int i = offset; i<listSize; i++) {
        if(list[i][0] == x && list[i][1] == y) {
            return 1;
        }
    }

    return 0;
}

/**
 * CoordListIndexOf find the index of the point x,y in the list
 * ignores all the selected coords before the offset index
 * Returns -1 if none found
 */
int CoordListIndexOf(int x, int y, int list[][2], int listSize, int offset) {
    for(int i = offset; i<listSize; i++) {
        if(list[i][0] == x && list[i][1] == y) {
            return i;
        }
    }

    return -1;
}

int GetPrevSelectionCoord(int selectionSize, int ignoreOffset, int index) {
    if(index > 0 && index > ignoreOffset) {
        return index-1;
    } else {
        //Caso seja a primeira coordenada do poligono, o ponto anterior e o que esta no final do vetor selection
        //selectionSize menos dois porque a ultima coordenada do vetor vai ser igual à primeira coordenada do poligono que a selecao forma
        //Por isso o ponto antes é o penultimo do selection
        return selectionSize-2;
    }
}

int GetNextSelectionCoord(int selectionSize, int ignoreOffset, int index) {
    if(index+1 == selectionSize) {
        return ignoreOffset+1;
    } else {
        return index+1;
    }
}

/**
 * GetSelectionMakesCornerSide: Diz caso a seleção faça um canto, para que lado ele vai
 */
int GetSelectionMakesCornerSide(int selection[][2], int currentIndex, int prevIndex, int nextIndex) {
    if(selection[currentIndex][0] != selection[prevIndex][0] && selection[currentIndex][0] != selection[nextIndex][0]) {
        return HORIZONTAL;
    }

    if(selection[currentIndex][0] > selection[prevIndex][0]) {
        return CORNER_LEFT;
    } else if(selection[currentIndex][0] < selection[prevIndex][0]) {
        return CORNER_RIGHT;
    } else {
        //Caso o ponto anterior estaja para cima ou para baixo analisa o ponto seguinte
        if(selection[currentIndex][0] > selection[nextIndex][0]) {
            return CORNER_LEFT;
        } else if(selection[currentIndex][0] < selection[nextIndex][0]) {
            return CORNER_RIGHT;
        }
    }

    return VERTICAL;
}

void DestroySelectionSquare(int selection[][2], int selectionSize, int selectingColor, int board[][MAX_BOARD_POS], int boardDimensions[2], int selectionFormingSquareIndex, int goals[]) {
    //Ideia tirada daqui https://en.wikipedia.org/wiki/Point_in_polygon#Ray_casting_algorithm
    //Comeca-se numa coluna fixa, e ao passar por uma fronteira, significa, geralmente que se entrou ou saiu do poligono
    for(int x = 0; x<boardDimensions[0]; x++) {
        //Indica se está dentro do poligono
        bool inside = 0;

        //Estas duas variaveis servem para quando numa coluna estao selecionados varios pontos de seguida
        //Se a selecao vier da uma direcao e voltar de novo para a mesma direcao os pontos em baixo pertencem na mesma à selecao
        //Ele devera considerar que os pontos em baixo fazem parte ainda da selecao (ou continuam a nao fazer).
        //Se ela vier de uma direcao e for para a oposta, considera que em baixo deixa de fazer parte da selecao(ou passa a fazer)
        int prevBorderGoesToSide = -1;

        for(int y = 0; y<boardDimensions[1]; y++) {
            //Contem o sitio do vetor selection onde as coordenadas xy estao
            const int index = CoordListIndexOf(x, y, selection, selectionSize, selectionFormingSquareIndex);

            
            if(index != -1) {
                //Se as o ponto fizer parte da fronteira(selecion)

                //Os indices dos pontos seginte e anterior da selecao do poligono
                const int prevSelectionPointIndex = GetPrevSelectionCoord(selectionSize, selectionFormingSquareIndex, index);
                const int nextSelectionPointIndex = GetNextSelectionCoord(selectionSize, selectionFormingSquareIndex, index);

                //Caso passe por uma borda da selecao vertical, nao contar como atravessar se essa borda vier e for para o mesmo lado
                //(ex: a selecao vem da esqueda e volta para a esquerda, entao nao deve ser considerado como uma fronteira)
                int side = GetSelectionMakesCornerSide(selection, index, prevSelectionPointIndex, nextSelectionPointIndex);
                if(side == CORNER_LEFT || side == CORNER_RIGHT) {
                    //Caso o ponto selecionado faca um canto
                    if(prevBorderGoesToSide != -1) {
                        if(prevBorderGoesToSide == side) {
                            prevBorderGoesToSide = -1;
                        } else {
                            inside = !inside;
                        }
                        prevBorderGoesToSide = -1;
                    } else {
                        prevBorderGoesToSide = side;
                    }
                    
                } else if(side == HORIZONTAL) {
                    //Se atravessar por uma linha horizontal, entao entrou ou saiu do poligono
                    inside = !inside;
                }
            } else {
                //Se o ponto nao fizer parte da fronteira(selection)
                //Se estiver dentro do poligono
                if(inside) {
                    //Apaga 
                    if(board[x][y] > -1) {
                        board[x][y] = -1;
                    }
                }
            }
        }
    }
}

void UpdateAnimation(int* animation, int* animationTime) {
    if(*animation != ANIMATION_NONE) {
        (*animationTime)++;
        if(*animation == ANIMATION_SHUFFLE) {
            if(*animationTime >= 10) {
                SetAnimation(ANIMATION_NONE, animation, animationTime);
            }
        }
    }
}

void UpdateBoard(bool* boardMoving, int board[][MAX_BOARD_POS], int boardDimensions[2], int num_colors, int* animation, int* animationTime, int* colorSquareErased) {
    //Caso teja pontos a cair
    if(*boardMoving) {
        *boardMoving = BoardFall(board, boardDimensions, num_colors, colorSquareErased);
        if(!(*boardMoving)) {
            //Se os pontos tiverem parado de cair

            //Na jogada seguinte ja devem cair pontos de todas as cores
            (*colorSquareErased) = -1;

            //Verificar se ha jogadas possiveis
            if(!CheckIfThereArePossiblePlays(board, boardDimensions)) {
                SetAnimation(ANIMATION_SHUFFLE, animation, animationTime);
                SuffleBoard(board, boardDimensions);
            }
        }
    }
}

void ClearBoard(int board[][MAX_BOARD_POS], int boardDimensions[2]) {
    for(int i = 0; i<boardDimensions[0]; i++) {
        for(int j = 0; j<boardDimensions[1]; j++) {
            board[i][j] = -1;
        }
    }
}

void PreventNegativeGoals(int goals[MAX_COLORS], int numColors) {
    for(int i = 0; i<numColors; i++) {
        goals[i] = MAX(goals[i], 0);
    }
}

//Returns 1 if any dot fell
bool BoardFall(int board[][MAX_BOARD_POS], int boardDimensions[2], int num_colors, int* colorSquareErased) {
    bool movement = 0;
    
    //Make so the dots fall
    for(int j = boardDimensions[1]-1; j>0; j--) {
        for(int i = 0; i<boardDimensions[0]; i++) {
            //If this place is empty and there is a dot above, move it downwards
            if(board[i][j] == -1) {
                //Find a dot above, skipping all the frozen places
                for(int a = j-1; a>=0; a--) {
                    const int aboveColor = board[i][a];
                    if(aboveColor > -1) {
                        //If it is a dot above, make it fall
                        board[i][j] = aboveColor;
                        board[i][a] = -1;
                        movement = 1;
                        break;
                    } else if(aboveColor == -1) {
                        //If it is an empty place above dont do anything
                        break;
                    }
                    
                    //If it is a frozen place aboveColor == -2, continue looking upwards to find a point to fall
                }
            }
        }
    }

    //Generate new dots on the top row
    for(int i = 0; i<boardDimensions[0]; i++) {
        //Comeca na primeira linha e para quando encontrar uma casa nao congelada
        for(int j = 0; j<boardDimensions[1]; j++) {
            //If there is an empty place, create a new dot there
            if(board[i][j] == -1) {
                //Gerar uma cor aleatoria
                int color = 0;
                if((*colorSquareErased) == -1) {
                    //Caso seja para gerar todas as cores
                    color = rand()%num_colors;
                } else {
                    //Caso tenha sido criado um quadrado e é para gerar menos uma cor
                    color = rand()%(num_colors-1);

                    if(color == (*colorSquareErased)) {
                        //Se a cor gerada for a cor que nao e suposto gerar, mudar a cor para a cor que o rand nao iria gerar
                        color = num_colors-1;
                    }
                }
                

                board[i][j] = color;
                movement = 1;
            }

            if(board[i][j] != -2) {
                break;
            }
        }
        
    }

    return movement;
}


/**
 * ProcessMouseEvent: gets the square pos based on the click positions !
 * \param _mouse_pos_x position of the click on pixel coordinates
 * \param _mouse_pos_y position of the click on pixel coordinates
 * \param _board_size_px size of the board !
 * \param _square_size_px size of each square
 * \param _pt_x square nr
 * \param _pt_y square nr
 * \param onlyConsiderInsideCircle if it's 1, it will set to -1 -1 if its outside the circle but inside square
 */
void ProcessMouseEvent(int _mouse_pos_x, int _mouse_pos_y, int _board_size_px[], int _square_size_px, 
        int *_pt_x, int *_pt_y, bool onlyConsiderInsideCircle)
{
    // corner of the board
    int x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    int y_corner = (TABLE_SIZE - _board_size_px[1] - 15);
    
    // verify if valid cordinates
    if (_mouse_pos_x < x_corner || _mouse_pos_y < y_corner || _mouse_pos_x > (x_corner + _board_size_px[0])
        || _mouse_pos_y > (y_corner + _board_size_px[1]) )
    {
        *_pt_x = -1;
        *_pt_y = -1;
        return;
    }
    
    *_pt_x = (_mouse_pos_x - x_corner) / (_square_size_px + SQUARE_SEPARATOR);
    *_pt_y = (_mouse_pos_y - y_corner) / (_square_size_px + SQUARE_SEPARATOR);

    if(onlyConsiderInsideCircle) {
        int circleX, circleY, circleR, dist;
        circleX = x_corner + ((*_pt_x)+1)*SQUARE_SEPARATOR + (*_pt_x)*(_square_size_px)+(_square_size_px>>1);
        circleY = y_corner + ((*_pt_y)+1)*SQUARE_SEPARATOR + (*_pt_y)*(_square_size_px)+(_square_size_px>>1);
        circleR = (int)(_square_size_px*0.4f);

        dist = sqrt(SQR(_mouse_pos_x-circleX) + SQR(_mouse_pos_y-circleY));

        if(dist > circleR) {
            *_pt_x = -1;
            *_pt_y = -1;
        }
    }
}


void RenderGameStateText(int state, TTF_Font* font, SDL_Color* textColor, SDL_Renderer* renderer, int numFrozenPlacesPlaced) {
    char str[20];
    switch(state) {
        case GAME_STATE_LOST:
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE, "You lost!", font, textColor, renderer);
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+2*FONT_BIG_SIZE, "Press n to start a new game!", font, textColor, renderer);
            
            break;
        case GAME_STATE_WON:
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE, "You won!", font, textColor, renderer);
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+2*FONT_BIG_SIZE, "Press n to start a new game!", font, textColor, renderer);
            break;
        case GAME_STATE_EXCEDED_MAX_PLAYS:
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE, "Max plays exceeded. You can't play anymore!", font, textColor, renderer);
            break;
        case GAME_STATE_DEFINE_FROZEN_PLACES:
            sprintf(str, "%d", numFrozenPlacesPlaced);
            if(numFrozenPlacesPlaced < MAX_FROZEN_PLACES) {
                RenderText(STATE_TEXT_RENDER_X-30, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE, str, font, textColor, renderer);
                RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE,
                    (numFrozenPlacesPlaced == 1) ? "Frozen place defined" : "Frozen places defined", font, textColor, renderer);
            } else {
                RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+FONT_BIG_SIZE, "You can't add more frozen places!", font, textColor, renderer);
            }
            
            
            RenderText(STATE_TEXT_RENDER_X, STATE_TEXT_RENDER_Y+2*FONT_BIG_SIZE, "Press n play!", font, textColor, renderer);
            
            
            break;
    }
}



/**
 * RenderSelection: renders the connection between the selected points
 */
void RenderSelection(int selection[][2], int selectionSize, int selectingColor, int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer) {
    int x_corner, y_corner;

    SDL_SetRenderDrawColor(_renderer, colors[0][selectingColor], colors[1][selectingColor], colors[2][selectingColor], 255);

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // renders the squares where the dots will appear
    for ( int i = 1; i < selectionSize; i++ )
    {
        //The coordenates of the current point and last one
        const int xLast = selection[i-1][0];
        const int yLast = selection[i-1][1];
        const int x = selection[i][0];
        const int y = selection[i][1];

        const int smallestSize = _square_size_px/4; //Size of the smallest side of the rectangle

        const int circleX = x_corner + (xLast+1)*SQUARE_SEPARATOR + xLast*(_square_size_px)+(_square_size_px>>1);
        const int circleY = y_corner + (yLast+1)*SQUARE_SEPARATOR + yLast*(_square_size_px)+(_square_size_px>>1);

        SDL_Rect rect = {circleX - smallestSize/2, circleY - smallestSize/2, smallestSize, smallestSize};
        
        //Sets the direction of the rectangle
        if(x < xLast) {
            //Esquerda
            rect.w = -_square_size_px;
        } else if(x > xLast) {
            //Direita
            rect.w = _square_size_px;
        } else if(y < yLast) {
            //Cima
            rect.h = -_square_size_px;
        } else if(y > yLast) {
            //Baixo
            rect.h = _square_size_px;
        } else {
            printf("This should not happen!\n");
        }

        SDL_RenderFillRect(_renderer, &rect);
    }
}

/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param boardDimensions number of positions in the board in each axis
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPoints(int _board[][MAX_BOARD_POS], int boardDimensions[2],
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer )
{
    int clr, x_corner, y_corner, circleX, circleY, circleR;
    const SDL_Color dark = { 120, 110, 102 }; //Color of the frozen places

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);


    // renders the squares where the dots will appear
    for ( int i = 0; i < boardDimensions[0]; i++ )
    {
        for ( int j = 0; j < boardDimensions[1]; j++ )
        {
            // define the size and copy the image to display
            circleX = x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px)+(_square_size_px>>1);
            circleY = y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px)+(_square_size_px>>1);
            circleR = (int)(_square_size_px*0.4f);
            // draw a circle
            clr = _board[i][j];
            if(clr > -1) {
                filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
            } else if(clr == -2) {
                const SDL_Rect rect = {
                    x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px),
                    y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px),
                    _square_size_px,
                    _square_size_px
                    };
                SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, 255);
                SDL_RenderFillRect(_renderer, &rect);
            }
        }
    }
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
void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
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
 * Shows some information about the game:
 * - Goals of the game
 * - Number of moves
 * \param _renderer renderer to handle all rendering in a window
 * \param _font font to display the scores
 * \param _goals goals of the game
 * \param _ncolors number of colors
 * \param _moves number of moves remaining
 */
void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int _goals[], int _ncolors, int _moves)
{
    //int centerX = TABLE_SIZE/2;
    const int circleR = 15;
    char text[20];
    
    SDL_Rect rect;

    //Render moves left
    sprintf(text, "Moves: %d", _moves);

    rect.x = STATS_RECT_MOVES_X;
    rect.y = STATS_RECT_Y;
    rect.w = STATS_RECT_WIDTH;
    rect.h = STATS_RECT_HEIGHT;

    SDL_SetRenderDrawColor(_renderer, statsRectColor.r, statsRectColor.g, statsRectColor.b, 255);
    SDL_RenderFillRect(_renderer, &rect);
    RenderText(STATS_RECT_MOVES_X+2, STATS_RECT_Y+5, text, _font, &black, _renderer);

    //Para determinar o espaco entre cada retangulo subtrai-se o espaco no ecra disponivel
    // para os objetivos pelo o espaco ocupado por todos os retangulos de objetivos 
    const int spaceLeftForSpacing = STATS_SPACE_FOR_GOALS-(STATS_RECT_WIDTH)*(_ncolors+1);
    // Dividindo esse espaco restante pelo numero de cores obtem-se o espaco entre cada retangulo
    const int spacing = spaceLeftForSpacing/_ncolors;

    
    //Render goals
    for(int i = 0; i<_ncolors; i++) {
        SDL_Color color = _goals[i] == 0 ? green : statsRectColor;
        sprintf(text, "%d", _goals[i]);

        rect.x = (STATS_RECT_MOVES_X + STATS_RECT_WIDTH + spacing) + i*(STATS_RECT_WIDTH+spacing);
        rect.y = STATS_RECT_Y;
        rect.w = STATS_RECT_WIDTH;
        rect.h = STATS_RECT_HEIGHT;
        
        SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(_renderer, &rect);

        filledCircleRGBA(_renderer, rect.x+20, STATS_RECT_Y+20, circleR, colors[0][i], colors[1][i], colors[2][i]);
        RenderText(rect.x + 45, STATS_RECT_Y+5, text, _font, &black, _renderer);
    }
}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param boardDimensions number of positions in the board in each axis
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderTable( int boardDimensions[2], int _board_size_px[], int state, 
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color light = { 205, 193, 181 };
    SDL_Color dark = { 120, 110, 102 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);
    // this renders the student numberSDL_SetRenderDrawColor
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer) * 2;
    
    if(state == GAME_STATE_DEFINE_FROZEN_PLACES) {
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "To selected the", _font, &black, _renderer);
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "frozen places:", _font, &black, _renderer);
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "Left button: add", _font, &black, _renderer);
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "Right button: remove", _font, &black, _renderer);
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "r for random places", _font, &black, _renderer);
        height += RenderText(TABLE_SIZE+3*MARGIN, height, "Only frozen places 5", _font, &black, _renderer);
    }

    // compute and adjust the size of the table and squares
    max_pos = MAX(boardDimensions[0], boardDimensions[1]);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = boardDimensions[0]*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = boardDimensions[1]*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
    board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
    board.y = (TABLE_SIZE - _board_size_px[1] - 15);
    board.w = _board_size_px[0];
    board.h = _board_size_px[1];
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a );

    // iterate over all squares
    for ( int i = 0; i < boardDimensions[0]; i++ )
    {
        for ( int j = 0; j < boardDimensions[1]; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
            board_square.w = square_size_px;
            board_square.h = square_size_px;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
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



void AquireParamBoardSize(int boardDimensions[2]) {
    int aux = 1;
    while(aux) {
        char buf[256];
        printf("Insira o tamano do tabuleiro separado com um espaco[5-15 5-15]: ");
        
        if(fgets(buf, 256, stdin) == NULL || sscanf(buf, "%d %d", &boardDimensions[0], &boardDimensions[1]) != 2) {
            printf("Erro na leitura\n");
        } else if(boardDimensions[0] < 5 || boardDimensions[1] < 5) {
            printf("O tabuleiro nao pode ter menos que 5 casas em cada lado\n");
        } else if(boardDimensions[0] > 15 || boardDimensions[1] > 15) {
            printf("O tabuleiro nao pode ter mais que 15 casas em cada lado\n");
        } else {
            aux = 0;
        }
        
    }
}
void AquireParamName(char name[]) {
    bool aux = 1;
    while(aux) {
        char buf[256];
        printf("Insira o seu nome: ");
        char* fgetsResult = fgets(buf, 256, stdin);

        if(fgetsResult == NULL) {
            printf("Erro na leitura\n");
            continue;
        }
        
        //O fgets deixa um \n no final do texto, antes do \0
        //Isto serve para encontra-lo e se existir mesmo, substitui-lo por \n
        char* newLineCharLocation = strchr(buf, '\n');
        if(newLineCharLocation != NULL) {
            *newLineCharLocation = '\0';
        }

        if(strlen(buf) > MAX_NAME_LENGHT) {
            printf("O nome tem de ter no maximo 8 carateres\n");
        } else if(strlen(buf) < 1) {
            printf("Tem de inserir um nome!\n");
        } else {
            //Puts the name the user wrote in the name variable
            strcpy(name, buf);
            aux = 0;
        }
    }
}

void AquireParamNumColors(int* num_colors) {
    bool aux = 1;
    while(aux) {
        char buf[256];
        printf("Quantas cores quer[1-5]: ");
        if(fgets(buf, 256, stdin) == NULL || sscanf(buf, "%d", num_colors) != 1) {
            printf("Erro na leitura\n");
        } else if(*num_colors > MAX_COLORS) {
            printf("Não pode ter mais que 5 cores\n");
        } else if(*num_colors < MIN_COLORS) {
            printf("Tem de ter pelo menos duas cores\n");
        } else {
            aux = 0;
        }
    }
}
void AquireParamMoves(int* movesTotal) {
    bool aux = 1;
    while(aux) {
        char buf[256];
        printf("Qual o máximo de jogadas[1-99]: ");
        if(fgets(buf, 256, stdin) == NULL || sscanf(buf, "%d", movesTotal) != 1) {
            printf("Erro na leitura\n");
        } else if(*movesTotal > MAX_MOVES) {
            printf("Não pode ser mais que 99 jogadas\n");
        } else if(*movesTotal < 1) {
            printf("Tem de ser um número positivo\n");
        } else {
            break;
        }
    }
}
void AcquireParamGoals(int goals[], int* num_colors) {
    char buf[256];
    
    for(int i = 0; i<(*num_colors); i++) {
        bool aux = 1;
        while(aux) {
            printf("Qual o objetivo para a cor %d[0-99]: ", i);
            if(fgets(buf, 256, stdin) == NULL || sscanf(buf, "%d", goals+i) != 1) {
                printf("Erro na leitura\n");
            } else if(goals[i] > MAX_OBJETIVO) {
                printf("Não pode ser mais que 99 pontos\n");
            } else if(goals[i] < 0) {
                printf("Não pode ser um número negativo\n");
            } else {
                aux = 0;
            }
        }
    }
}

void AquireGameParameters(int boardDimensions[2], char name[], int* num_colors, int goals[], int* movesTotal) {
    AquireParamBoardSize(boardDimensions);
    AquireParamName(name);
    AquireParamNumColors(num_colors);
    AquireParamMoves(movesTotal);
    AcquireParamGoals(goals, num_colors);

    //Caso o jogador diga que quer que os objetivos de todas as cores seja 0
    // pedir para inserir novos objetivos
    bool allGoalsAreZero = 1;
    while(allGoalsAreZero) {
        for(int i = 0; i<(*num_colors); i++) {
            if(goals[i] > 0) {
                allGoalsAreZero = 0;
            }
        }

        if(allGoalsAreZero) {
            printf("Os objetivos não podem ser todos zero!\n");
            AcquireParamGoals(goals, num_colors);
        }
    }
}

void GenerateBoard(int boardDimensions[2], int board[][MAX_BOARD_POS], int num_colors) {
    for(int x = 0; x<boardDimensions[0]; x++) {
        for(int y = 0; y<boardDimensions[1]; y++) {
            if(board[x][y] != -2) {
                //Caso nao seja uma casa congelada, mete-lhe um ponto de cor aleatoria
                board[x][y] = rand()%num_colors;
            }
        }
    }
    //Verificar se ha jogadas possiveis, se nao houver, faz shuffle
    if(!CheckIfThereArePossiblePlays(board, boardDimensions)) {
        SuffleBoard(board, boardDimensions);
    }
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
void InitEverything(int width, int height, TTF_Font **_font, TTF_Font **_fontSans, TTF_Font **_fontBig, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_fontSans = TTF_OpenFont("OpenSans.ttf", 16);
    if(!*_fontSans)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_fontBig = TTF_OpenFont("OpenSans.ttf", FONT_BIG_SIZE);
    if(!*_fontBig)
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
    window = SDL_CreateWindow( "ISTDots - Miguel Fazenda IST190146", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
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
