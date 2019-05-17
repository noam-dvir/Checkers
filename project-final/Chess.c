#include "Chess.h"

unsigned minmax_depth = 1;
unsigned game_mode = 1;
char next_turn = WHITE;
char user_color = WHITE ;
LINK *allocated = NULL;

int console_main()
{
	char board[BOARD_SIZE][BOARD_SIZE];
	int restart=0,first=1;
	do{
		init_board_(board);
		if(first) 
		{
			first=0;
		}
		set_game_(board);
		restart=game_state(board); 
	} while(restart==1);
	return 0;
}


void print_line(){
	int i;
	printf(" |");
		for (i = 1; i < BOARD_SIZE*4; i++){
			printf("-");
		}
		printf("|\n");
}

void print_board_(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i,j;
		print_line();
		for (j = BOARD_SIZE-1; j >= 0 ; j--)
		{
			printf("%d", j+1);
			for (i = 0; i < BOARD_SIZE; i++){
				printf("| %c ", board[i][j]);
			}
			printf("|\n");
			print_line();
		}
		printf("  ");
		for (j = 0; j < BOARD_SIZE; j++){
			printf(" %c  ", (char)('a' + j));
		}
		printf("\n");
}



void init_board_(char board[BOARD_SIZE][BOARD_SIZE]){
	int i,j;
		for (i = 0; i < BOARD_SIZE; i++){
			for (j = 0; j < BOARD_SIZE; j++){
				if (j == 1){
					board[i][j] = WHITE_P;
				}
				else if (j == 6){
					board[i][j] = BLACK_P;
				}
				else if (j == 0 && (i == 0 || i == 7)){
					board[i][j] = WHITE_R;
				}
				else if (j == 0 && (i == 1 || i == 6)){
					board[i][j] = WHITE_N;
				}
				else if (j == 0 && (i == 2 || i == 5)){
					board[i][j] = WHITE_B;
				}
				else if (j == 0 && i == 3){
					board[i][j] = WHITE_Q;
				}
				else if (j == 0 && i == 4){
					board[i][j] = WHITE_K;
				}
				else if (j == 7 && (i == 0 || i == 7)){
					board[i][j] = BLACK_R;
				}
				else if (j == 7 && (i == 1 || i == 6)){
					board[i][j] = BLACK_N;
				}
				else if (j == 7 && (i == 2 || i == 5)){
					board[i][j] = BLACK_B;
				}
				else if (j == 7 && i == 3){
					board[i][j] = BLACK_Q;
				}
				else if (j == 7 && i == 4){
					board[i][j] = BLACK_K;
				}
				else{
					board[i][j] = EMPTY;
				}	
			}
		}
	}

char player_to_color(char player)
{
	if (player == EMPTY) return '\0';
	return isupper(player)?BLACK:WHITE;
}

void *scalloc(size_t nitems,size_t size,const char *func_name,void *ptr)
{
	void *point=calloc(nitems,size);
	if(point==NULL)
	{
		if(ptr!=NULL)
		{
			free(ptr);
		}
		perror("ERROR: standard function calloc has failed\n");
		exit(1);
	}
	return point;
}



int game_state(char board[BOARD_SIZE][BOARD_SIZE])
{
	char input[100]={0};
	int depth;
	if(user_color==opp_color(next_turn) && game_mode == 2) 
	{
		if (minmax_depth == 5)
			depth = compute_depth(board,opp_color(user_color));
		else
			depth = minmax_depth;
		minmax(board,opp_color(user_color),depth,1,-2000,2000);
		print_board_(board);
		if(check_win(board,opp_color(user_color))) 
		{
			return 0;
		}
		if (check_check(board,opp_color(user_color)))
			print_message("Check!\n");
	}
	while(1)
	{
		printf("%s player - enter your move:\n", user_color==WHITE?"White":"Black");
		if(fgets(input, 100, stdin)==NULL)
		{
			perror("Error: standard function fgets has failed\n");
			continue;
		}
		delete_n(input);
		if(!strncmp(input,"get_moves ",10))
		{
			get_one_moves(board, input[11]-'a',input[13]-'1', user_color);	
		}
		else if(!strncmp(input,"move ",5))
		{
			LINK *move,*allowed_moves=possible_moves(board,user_color);
			move=scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
			move->next=NULL;
			sprintf(move->move,"%s",input+5);
			move->prom=check_if_allowed(board,user_color,move,allowed_moves)-1;
			if(move->prom>-1)
			{
				do_move(board,move,user_color);
				print_board_(board);
				if(check_win(board,user_color))
				{
					free(move);
					free_all_links(allowed_moves);
					break;
				}
				if (check_check(board,user_color))
					print_message("Check!\n");
				if(game_mode == 2)
				{
					if (minmax_depth == 5)
						depth = compute_depth(board,opp_color(user_color));
					else
						depth = minmax_depth;
					minmax(board,opp_color(user_color),depth,1,-2000,2000);
					print_board_(board);
					if(check_win(board,opp_color(user_color))) 
					{
						free(move);
						free_all_links(allowed_moves);
						break;
					}
					if (check_check(board,opp_color(user_color)))
						print_message("Check!\n");
				}
				else
				{
					user_color = opp_color(user_color);
				}
			}
			free(move);
			free_all_links(allowed_moves);
		}
		else if(!strncmp(input,"save ",5))
		{
			char filename[50]={0};
			sprintf(filename,"%s",input+5);
			save_game(filename,board,game_mode,minmax_depth,user_color,user_color);
		}
		else if(!strcmp(input,"quit"))
		{
			break;
		}
		else if(!strcmp(input,"restart"))
		{
			return 1;
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
		}
	}
	return 0;
	
}

int check_win(char board[BOARD_SIZE][BOARD_SIZE],char color)
{
	LINK *moves_allowed=possible_moves(board,opp_color(color));
	if(moves_allowed==NULL)
	{
		printf("Mate! %s player wins the game!\n",color==BLACK?"Black":"White");
		return 1;
	}
	free_all_links(moves_allowed);
	return 0;
}

