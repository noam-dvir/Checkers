#ifndef SETTINGS_
#define SETTINGS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chess.h"

#define min(x,y) ((x<y)?(x):(y))
#define max(x,y) ((x<y)?(y):(x))
#define MAX_MOVES 1000000.0

char options[4][7]={"queen","bishop","rook","knight"};

extern unsigned minmax_depth;
extern unsigned game_mode;
extern char user_color;

extern char player_to_color(char);

void get_moves(char board[BOARD_SIZE][BOARD_SIZE]);
void get_one_moves(char board[BOARD_SIZE][BOARD_SIZE],int ,int ,char );
void do_move(char board[BOARD_SIZE][BOARD_SIZE],LINK *,char);
int check_if_allowed(char board[BOARD_SIZE][BOARD_SIZE],char ,LINK *,LINK *);
int check_win(char board[BOARD_SIZE][BOARD_SIZE],char);
int check_check(char board[BOARD_SIZE][BOARD_SIZE],char);
int score(char board[BOARD_SIZE][BOARD_SIZE],char); 
LINK *possible_moves(char board[BOARD_SIZE][BOARD_SIZE], char);
LINK *all_moves(char board[BOARD_SIZE][BOARD_SIZE], char);
LINK *pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], int ,int ,char);
LINK *king_moves(char board[BOARD_SIZE][BOARD_SIZE], int ,int ,char );
LINK *knight_moves(char board[BOARD_SIZE][BOARD_SIZE], int ,int ,char );
LINK *bishop_moves(char board[BOARD_SIZE][BOARD_SIZE], int ,int ,char );
LINK *rook_moves(char board[BOARD_SIZE][BOARD_SIZE], int ,int ,char );
void clean_not_allowed(char board[BOARD_SIZE][BOARD_SIZE],LINK *,char );
int compute_depth(char board[BOARD_SIZE][BOARD_SIZE], char );
void add_link(LINK *,LINK *);
void free_all_links(LINK *);
LINK *goto_last(LINK *);
void delete_after(LINK *);


#endif
