#ifndef CHESS_
#define CHESS_

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_R 'r'
#define WHITE_N 'n'
#define WHITE_Q 'q'
#define WHITE_K 'k'
#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_R 'R'
#define BLACK_N 'N'
#define BLACK_Q 'Q'
#define BLACK_K 'K'
#define EMPTY ' '
#define BLACK 'b'
#define WHITE 'w'

#define BOARD_SIZE 8
#define NUMBER_OF_PLAYERS 4

typedef char** board_t;

#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define WRONG_FILE "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
 
#define perror_message2(func_name) (perror("Error: standard function %s has failed", func_name))
#define print_message(message) (printf("%s", message));
#define is_inside(i,j) ((i)>=0 && (i)< BOARD_SIZE && (j)>=0 && (j) < BOARD_SIZE) 
#define opp_color(color) (color==WHITE?BLACK:WHITE)
#define is_king(c) (c==BLACK_K || c==WHITE_K)



unsigned extern minimax_depth;
typedef struct link
{
	char move[22];
	struct link *next;
	int prom;
} LINK;

int console_main();
void print_board_(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board_(char board[BOARD_SIZE][BOARD_SIZE]);
int game_state(char board[BOARD_SIZE][BOARD_SIZE]);
void get_moves(char board[BOARD_SIZE][BOARD_SIZE]);
extern int check_check(char board[BOARD_SIZE][BOARD_SIZE],char);
extern void get_one_moves(char board[BOARD_SIZE][BOARD_SIZE],int ,int , char);
extern int save_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],int ,int , char, char);
extern int compute_depth(char board[BOARD_SIZE][BOARD_SIZE], char );
LINK *possible_moves(char board[BOARD_SIZE][BOARD_SIZE], char );
void do_move(char board[BOARD_SIZE][BOARD_SIZE],LINK *,char);
int check_if_allowed(char board[BOARD_SIZE][BOARD_SIZE],char,LINK *,LINK *);
int minmax(char board[BOARD_SIZE][BOARD_SIZE],char,int,int ,int, int);
int check_win(char board[BOARD_SIZE][BOARD_SIZE],char);
void add_link(LINK *,LINK *);
void free_all_links(LINK *);
LINK *goto_last(LINK *head);
char player_to_color(char );
void clean_non_eating(LINK *);
void delete_after(LINK *);
void delete_n(char *);
void *scalloc(size_t ,size_t,const char *,void *);

extern void set_game_(char board[BOARD_SIZE][BOARD_SIZE]);


#endif
