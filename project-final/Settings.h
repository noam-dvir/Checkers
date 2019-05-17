#ifndef SETTINGS_
#define SETTINGS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chess.h"


extern unsigned minmax_depth;
extern unsigned game_mode;
extern char user_color;
extern char next_turn;


int check_board_start(char board[BOARD_SIZE][BOARD_SIZE]);
void set_game_(char board[BOARD_SIZE][BOARD_SIZE]);
void delete_n(char *);
int save_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],int ,int , char, char);
int load_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],unsigned * ,unsigned * , char *, char *);


#endif

