#include "Settings.h"

void set_game_(char board[BOARD_SIZE][BOARD_SIZE]){
	unsigned num, loaded=0;
	next_turn = WHITE;
	print_message("Enter game setting:\n");
	while(1)
	{
		char input[50]={0};
		if(fgets(input, 50, stdin)==NULL)
		{
			perror("ERROR: standard function fgets has failed\n");
			continue;
		}
		delete_n(input);
		if(game_mode == 2 && !strncmp(input,"difficulty best",15))
		{
			minmax_depth=5;
		}
		else if(game_mode == 2 && !strncmp(input,"difficulty depth ",17))
		{
			num=input[17]-'0';
			if( num < 1 || num > 4 || input[18]!='\0') {
				print_message(WRONG_MINIMAX_DEPTH);
			}
			else{
				minmax_depth = num;
			}
		}

		else if(!strncmp(input,"game_mode ",10))
		{
			num=input[10]-'0';
			if( (num != 1 && num != 2) || input[11]!='\0') {
				print_message(WRONG_GAME_MODE);
			}
			else{
				game_mode = num;
				printf("Running game in %s mode\n", game_mode == 1 ? "2 players":"player vs. AI");
			}
		}
		else if(game_mode == 2 && !strncmp(input,"user_color ",11))
		{
			if( !strcmp(input+11,"white"))
				user_color=WHITE;
			else if(!strcmp(input+11,"black"))
				user_color=BLACK;
		}
		else if(!strcmp(input,"print"))
		{
			print_board_(board);
		}
		else if(!strncmp(input,"load ",5))
		{
			char filename[50]={0};
			sprintf(filename,"%s",input+5);
			if (load_game(filename,board,&game_mode,&minmax_depth,&user_color,&next_turn))
			{
				print_board_(board);
				loaded = 1;
			}
		}
		else if(!strcmp(input,"quit"))
		{
			exit(0);
		}
		else if(!strcmp(input,"start"))
		{
			if(check_board_start(board))
			{
				if (game_mode == 1 && loaded == 0)
					user_color = WHITE;
				return;
			}
		}
		else
		{
			print_message(ILLEGAL_COMMAND);
		}
		
	}

}


/*
   	Legacy from checkers
*/
int check_board_start(char board[BOARD_SIZE][BOARD_SIZE])
{
	return 1;
}

void delete_n(char *str)
{
	while(*str!='\0' && *str!='\n')
	{
		str++;
	}
	*str='\0';
}

int save_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],int game_mode,int diff, char color, char next_turn)
{
	FILE *f = fopen(filename, "w");
	char string_diff[10];
	int i,j;
	sprintf(string_diff,"%d",diff);
	if (f == NULL)
	{
		print_message(WRONG_FILE);
		return 0;
	}
	fprintf(f,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n");
	fprintf(f,"\t<type>%d</type>\n",game_mode);
	if(game_mode==2) 
	{
		fprintf(f,"\t<difficulty>%s</difficulty>\n",diff==5?"Best":string_diff);
		fprintf(f,"\t<user_color>%s</user_color>\n",color==WHITE?"White":"Black");
	}
	fprintf(f,"\t<next_turn>%s</next_turn>\n",next_turn==WHITE?"White":"Black");
	fprintf(f,"\t<board>\n");
	for(j=BOARD_SIZE - 1;j>=0;j--)
	{
		fprintf(f,"\t\t<row_%d>",j+1);
		for(i=0;i<BOARD_SIZE;i++)
		{
			fprintf(f,"%c",board[i][j]==EMPTY?'_':board[i][j]);
		}
		fprintf(f,"</row_%d>\n",j+1);
	}
	fprintf(f,"\t</board>\n");
	fprintf(f,"</game>");

	fclose(f);
	return 1;
}

int load_game(const char filename[50],char board[BOARD_SIZE][BOARD_SIZE],unsigned *game_mode,unsigned *diff, char *color, char *next_turn)
{
	FILE *f = fopen(filename, "r");
	char buffer[1024];
	char *found;
	int i,j;
	if (f == NULL)
	{
		print_message(WRONG_FILE);
		return 0;
	}
	while(EOF != fscanf(f,"%s",buffer))
	{
		if((found = strstr(buffer,"<type>")) != NULL)
		{
			*game_mode = found[6]-'0';
		}
		if((found = strstr(buffer,"<difficulty>")) != NULL)
		{
			*diff = found[12]=='B'?5:found[12]-'0';
		}
		if((found = strstr(buffer,"<user_color>")) != NULL)
		{
			*color = found[12]=='W'?WHITE:BLACK;
		}
		if((found = strstr(buffer,"<next_turn>")) != NULL)
		{
			*next_turn = found[11]=='W'?WHITE:BLACK;
		}
		if((found = strstr(buffer,"<row_")) != NULL)
		{
			j = found[5]-'1';
			for(i=0;i<BOARD_SIZE;i++)
			{
				board[i][j]=found[7+i]=='_'?EMPTY:found[7+i];
			}
		}

	}
	if (*game_mode == 1)
	{
		*color = *next_turn;
	}
	fclose(f);
	return 1;
}
