#ifndef CHESS_GUI_
#define CHESS_GUI_

#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Chess.h"

#define WIN_W 400
#define WIN_H 480
#define WINS_W 420
#define WINS_H 480
#define WING_W 900
#define WING_H 800
#define WHITE_Y 120
#define BLACK_Y 20
#define PAWN_X 412
#define ROOK_X 11
#define KNIGHT_X 329
#define KING_X 246
#define QUEEN_X 165
#define BISHOP_X 84
#define NUM_SLOTS 5

typedef struct gui_comp
{
	SDL_Rect imgrect;
	SDL_Surface *img;
	struct gui_comp *next;
} COMP;

int gui_main(); 
COMP *Create_Button(int,int ,int,int, char *,COMP *);
void print_main(SDL_Surface * ,COMP *,int,int);
int mouse_main(int,int);
void Clean_Buttons(COMP *);
extern int load_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],unsigned *,unsigned *, char *, char *);
extern int save_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],int ,int , char , char );

extern unsigned minmax_depth;
extern unsigned game_mode;
extern char next_turn;
extern char user_color;

#endif
