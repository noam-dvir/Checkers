#include "Moves.h"

void get_moves(char board[BOARD_SIZE][BOARD_SIZE])
{
	LINK *moves,*first;
	moves=possible_moves(board,user_color);
	first=moves;
	while(moves!=NULL)
	{
		printf("%s\n", moves->move);
		moves=moves->next;
	}
	free_all_links(first);
}

void get_one_moves(char board[BOARD_SIZE][BOARD_SIZE],int i,int j, char color)
{

	LINK *moves,*first,*temp;
	moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
	strcpy(moves->move,"first");
	moves->next=NULL;
	moves->prom=-1;
	if(!is_inside(i,j)) 
	{
		print_message(WRONG_POSITION);
		return;
	}
	if(board[i][j] == EMPTY || player_to_color(board[i][j]) != color)
	{
		print_message(NO_DICS);
		return;
	}
	switch (board[i][j])
	{
		case BLACK_P:
		case WHITE_P:
			moves->next=pawn_moves(board,i,j,color);
			break;
		case BLACK_K:
		case WHITE_K:
			moves->next=king_moves(board,i,j,color);
			break;
		case BLACK_R:
		case WHITE_R:
			moves->next=rook_moves(board,i,j,color);
			break;
		case BLACK_N:
		case WHITE_N:
			moves->next=knight_moves(board,i,j,color);
			break;
		case BLACK_Q:
		case WHITE_Q:
			moves->next=rook_moves(board,i,j,color);
			add_link(moves,bishop_moves(board,i,j,color));
			break;
		case BLACK_B:
		case WHITE_B:
			moves->next=bishop_moves(board,i,j,color);
			break;
		default:
			break;
	}
	clean_not_allowed(board, moves, color);
	temp=moves;
	moves=moves->next;
	free(temp);
	first=moves;
	while(moves!=NULL)
	{
		printf("%s\n", moves->move);
		moves=moves->next;
	}
	free_all_links(first);

}

LINK *possible_moves(char board[BOARD_SIZE][BOARD_SIZE], char color)
{
	LINK *moves,*temp;
	moves=all_moves(board,color);
	clean_not_allowed(board, moves, color);
	temp=moves;
	moves=moves->next;
	free(temp);
	return moves;

}

LINK *all_moves(char board[BOARD_SIZE][BOARD_SIZE], char color)
{
	LINK *moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
	int i,j;
	strcpy(moves->move,"first");
	moves->next=NULL;
	moves->prom=-1;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if(board[i][j]==EMPTY) continue;
			if(player_to_color(board[i][j]) == color)
			{
				switch (board[i][j])
				{
					case BLACK_P:
					case WHITE_P:
						add_link(moves,pawn_moves(board,i,j,color));
						break;
					case BLACK_K:
					case WHITE_K:
						add_link(moves,king_moves(board,i,j,color));
						break;
					case BLACK_R:
					case WHITE_R:
						add_link(moves,rook_moves(board,i,j,color));
						break;
					case BLACK_N:
					case WHITE_N:
						add_link(moves,knight_moves(board,i,j,color));
						break;
					case BLACK_Q:
					case WHITE_Q:
						add_link(moves,rook_moves(board,i,j,color));
						add_link(moves,bishop_moves(board,i,j,color));
						break;
					case BLACK_B:
					case WHITE_B:
						add_link(moves,bishop_moves(board,i,j,color));
						break;
					default:
						break;
				}
			}
		}
	}
	return moves;
}


LINK *pawn_moves(char board[BOARD_SIZE][BOARD_SIZE], int i,int j,char color)
{
	int direction= color==WHITE?1:-1, dir, ii;
	LINK *moves=NULL,*first=NULL;
	if (is_inside(i,j+direction) && board[i][j+direction]==EMPTY)
	{
		moves = (LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
		first=moves;
		if ( (j + direction == 7) || (j + direction == 0)) /* Promotion */
		{
			for (ii=0; ii<4; ii++)
			{
				if(ii>0)
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);			
					moves = moves->next;
				}
				moves->prom = 1;				
				sprintf(moves->move,"<%c,%d> to <%c,%d> %s",i+'a',j+1,i+'a',j+1+direction,options[ii]);
			}
		}
		else
		{
			moves->prom=0;
			sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+'a',j+1+direction);
		}

	}
	for (dir = -1; dir <= 1; dir+=2)
	{
		if (is_inside(i+dir,j+direction) && player_to_color(board[i+dir][j+direction])==opp_color(color))
		{
			if(moves==NULL)
			{
				moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
				first=moves;
			}
			else
			{
				moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
				moves=moves->next;
			}
			if ( (j + direction == 7) || (j + direction == 0)) /* Promotion */
			{
				for (ii=0; ii<4; ii++)
				{
					if(ii>0)
					{
						moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);			
						moves = moves->next;
					}
					moves->prom = 1;				
					sprintf(moves->move,"<%c,%d> to <%c,%d> %s",i+'a',j+1,i+dir+'a',j+1+direction,options[ii]);
				}
			}
			else
			{
				moves->prom=0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+dir+'a',j+1+direction);
			}
		}
	}
	return first;
}

LINK *king_moves(char board[BOARD_SIZE][BOARD_SIZE], int i,int j,char color)
{
	int dir,left;
	LINK *moves=NULL,*first=NULL;
	for(dir=-1;dir<=1;dir++)
	{
		for(left=-1;left<=1;left++)
		{
			if (left == 0 && dir == 0) 
				continue;
			if(is_inside(i+left,j+dir) && player_to_color(board[i+left][j+dir])!=color)
			{
				if(moves==NULL)
				{
					moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					first=moves;
				}
				else
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					moves=moves->next;
				}
				moves->prom= 0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+left+'a',j+dir+1);
			}
		}
	}
	return first;
}

LINK *knight_moves(char board[BOARD_SIZE][BOARD_SIZE], int i,int j,char color)
{
	int dir,left,two;
	LINK *moves=NULL,*first=NULL;
	for(dir=-1;dir<=1;dir+=2)
	{
		for(left=-1;left<=1;left+=2)
		{
			for (two=1;two<=2;two++)
			{
				if(is_inside(i+left*two,j+dir*(3-two)) && player_to_color(board[i+left*two][j+dir*(3-two)])!=color)
				{
					if(moves==NULL)
					{
						moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
						first=moves;
					}
					else
					{
						moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
						moves=moves->next;
					}
					moves->prom= 0;
					sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+left*two+'a',j+dir*(3-two)+1);
				}
			}
		}
	}
	return first;
}

LINK *bishop_moves(char board[BOARD_SIZE][BOARD_SIZE], int i,int j,char color)
{
	int dir,left,cou=1;
	LINK *moves=NULL,*first=NULL;
	for(dir=-1;dir<=1;dir+=2)
	{
		for(left=-1;left<=1;left+=2)
		{
			cou=1;
			while(is_inside(i+cou*left,j+dir*cou) && board[i+cou*left][j+dir*cou]==EMPTY)
			{
				if(moves==NULL)
				{
					moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					first=moves;
				}
				else
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					moves=moves->next;
				}
				moves->prom=0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+cou*left+'a',j+1+dir*cou);
				cou++;
			}
			if(is_inside(i+cou*left,j+dir*cou) && player_to_color(board[i+cou*left][j+dir*cou])!=color)
			{
				if(moves==NULL)
				{
					moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					first=moves;
				}
				else
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					moves=moves->next;
				}
				moves->prom=0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+cou*left+'a',j+dir*cou+1);
			}
		}
	}
	return first;
}

LINK *rook_moves(char board[BOARD_SIZE][BOARD_SIZE], int i,int j,char color)
{
	int dir,left,cou=1;
	LINK *moves=NULL,*first=NULL;
	for(dir=-1;dir<=1;dir++)
	{
		for(left=-1;left<=1;left++)
		{
			if(dir != 0 && left != 0) 
				continue;
			cou=1;
			while(is_inside(i+cou*left,j+dir*cou) && board[i+cou*left][j+dir*cou]==EMPTY)
			{
				if(moves==NULL)
				{
					moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					first=moves;
				}
				else
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					moves=moves->next;
				}
				moves->prom=0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+cou*left+'a',j+1+dir*cou);
				cou++;
			}
			if(is_inside(i+cou*left,j+dir*cou) && player_to_color(board[i+cou*left][j+dir*cou])!=color)
			{
				if(moves==NULL)
				{
					moves=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					first=moves;
				}
				else
				{
					moves->next=(LINK *)scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					moves=moves->next;
				}
				moves->prom=0;
				sprintf(moves->move,"<%c,%d> to <%c,%d>",i+'a',j+1,i+cou*left+'a',j+dir*cou+1);
			}
		}
	}
	return first;
}


void do_move(char board[BOARD_SIZE][BOARD_SIZE],LINK *move,char color)
{
	int i,j,to_i,to_j;/*eats=(strlen(move->move)-9)/5;*/
	i=move->move[1]-'a';
	j=move->move[3]-'1';
	to_i=move->move[10]-'a';
	to_j=move->move[12]-'1';
	if(move->prom==0)
	{
		board[to_i][to_j]=board[i][j];
	}
	else
	{
		if(strlen(move->move) == 14)
		{
			board[to_i][to_j] = board[i][j] == WHITE_P ? WHITE_Q : BLACK_Q;
		}
		else
		{
			switch (move->move[15])
			{
				case 'q':
					board[to_i][to_j] = board[i][j] == WHITE_P ? WHITE_Q : BLACK_Q;
					break;
				case 'r':
					board[to_i][to_j] = board[i][j] == WHITE_P ? WHITE_R : BLACK_R;
					break;
				case 'k':
					board[to_i][to_j] = board[i][j] == WHITE_P ? WHITE_N : BLACK_N;
					break;
				case 'b':
					board[to_i][to_j] = board[i][j] == WHITE_P ? WHITE_B : BLACK_B;
					break;
			}
		}
	}
	board[i][j] = EMPTY;
}

int check_if_allowed(char board[BOARD_SIZE][BOARD_SIZE],char color,LINK *moveto,LINK *moves_list)
{
	int si,sj;
	si=moveto->move[1]-'a';
	sj=moveto->move[3]-'1';
	if(!is_inside(si,sj)) 
	{
		print_message(WRONG_POSITION);
		return 0;
	}
	if(player_to_color(board[si][sj])!=user_color)
	{
		print_message(NO_DICS);
		return 0;
	}
	while(moves_list!=NULL)
	{
		if(!strcmp(moves_list->move,moveto->move))
		{
			return 1 + moves_list->prom;
		}
		if(moves_list->prom)
		{
			if(!strncmp(moves_list->move,moveto->move,14) && strlen(moveto->move)==14)
			{
				return 2;
			}
		}
		moves_list=moves_list->next;
	}
	print_message(ILLEGAL_MOVE);
	return 0;
}

int check_check(char board[BOARD_SIZE][BOARD_SIZE],char color)
{
	int i,j,dir,left,two,cou = 1;
	char opp_king = color == WHITE ? BLACK_K: WHITE_K ;
	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			if (board[i][j] == opp_king)
				break;
		}
		if (j != 8 && board[i][j] == opp_king)
			break;
	}
	for(dir=-1;dir<=1;dir+=2)
	{
		for(left=-1;left<=1;left+=2)
		{
			for (two=1;two<=2;two++)
			{
				if(is_inside(i+left*two,j+dir*(3-two)) && board[i+left*two][j+dir*(3-two)]==(color==WHITE?WHITE_N:BLACK_N))
				{
					return 1;
				}
			}
		}
	}
	for(dir=-1;dir<=1;dir++)
	{
		for(left=-1;left<=1;left++)
		{
			if (left == 0 && dir == 0)
				continue;
			cou=1;
			while(is_inside(i+cou*left,j+dir*cou) && board[i+cou*left][j+dir*cou]==EMPTY)
			{
				cou++;
			}
			if(is_inside(i+cou*left,j+dir*cou) && player_to_color(board[i+cou*left][j+dir*cou])==color)
			{
				if(board[i+cou*left][j+dir*cou] == (color == WHITE? WHITE_B:BLACK_B) && dir != 0 && left != 0)
					return 1;
				if(board[i+cou*left][j+dir*cou] == (color == WHITE? WHITE_Q:BLACK_Q))
					return 1;
				if(board[i+cou*left][j+dir*cou] == (color == WHITE? WHITE_R:BLACK_R) && (dir == 0 || left == 0))
					return 1;
				if (cou == 1 && board[i+cou*left][j+dir*cou] == (color == WHITE? WHITE_K:BLACK_K))
					return 1;
				if (cou == 1 && board[i+cou*left][j+dir*cou] == (color == WHITE? WHITE_P:BLACK_P) && dir == (color==WHITE?-1:1) && left != 0)
					return 1; 
			}
		}
	}
	return 0;
}

int minmax(char board[BOARD_SIZE][BOARD_SIZE],char color,int depth,int first, int alpha, int beta)
{
	char board_copy[BOARD_SIZE][BOARD_SIZE];
	int min_node=color==opp_color(user_color)?1:-1,temp=0; 
	LINK *move,*allowed_moves,*firstmove;
	if(depth==0)
	{
		temp = score(board,color);
		return temp;
	}
	allowed_moves=possible_moves(board,color);
	firstmove=allowed_moves;
	while(allowed_moves!=NULL)
	{
		memcpy(board_copy,board,BOARD_SIZE*BOARD_SIZE*sizeof(char));
		do_move(board_copy,allowed_moves,color);
		temp=minmax(board_copy,opp_color(color),depth-1,0,alpha,beta);
		if(min_node==1)
		{
			if (temp < beta)
			{
				beta = temp;
				move = allowed_moves;
			}
		}
		else 
		{
			alpha = max(alpha,temp);
		}
		if (beta <= alpha)
		{
			break;
		}
		allowed_moves=allowed_moves->next;
	}
	if(first==1)
	{
		do_move(board,move,color);
		if(move->prom == 1 && move->move[15]=='q')
			move->move[14]='\0';
		printf("Computer: move %s\n",move->move );
	}
	free_all_links(firstmove);
	return min_node == 1 ? beta : alpha;
}


int score(char board[BOARD_SIZE][BOARD_SIZE],char color)
{
	int sum=0,i,j,pawn_score,black_k=0,white_k=0;
	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			if(board[i][j]!=EMPTY)
			{
				switch (board[i][j])
				{
					case BLACK_P:
					case WHITE_P:
						pawn_score = 1;
						break;
					case BLACK_K:
					case WHITE_K:
						white_k += board[i][j] == WHITE_K?1:0;
						black_k += board[i][j] == BLACK_K?1:0;
						pawn_score = 400;
						break;
					case BLACK_R:
					case WHITE_R:
						pawn_score = 5;
						break;
					case BLACK_N:
					case WHITE_N:
						pawn_score = 3;
						break;
					case BLACK_Q:
					case WHITE_Q:
						pawn_score = 9;
						break;
					case BLACK_B:
					case WHITE_B:
						pawn_score = 3;
						break;
					default:
						break;
				}
				sum+=player_to_color(board[i][j])==user_color?pawn_score:-1*pawn_score;
			}
		}
	}
	if (!black_k) return user_color == BLACK? -1000:1000;
	if (!white_k) return user_color == WHITE? -1000:1000;
	return sum;
}

int compute_depth(char board[BOARD_SIZE][BOARD_SIZE], char color)
{
	int depth,pawns=0,moves_count=0,pc_count=0,player_count=0,i,j,pawn_moves = 0; 
	LINK *allowed_moves=possible_moves(board,color),*firstmove;
	firstmove=allowed_moves;
	while(allowed_moves!=NULL)
	{
		moves_count++;
		allowed_moves = allowed_moves->next;
	}
	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			if(board[i][j]!=EMPTY)
			{
				switch (board[i][j])
				{
					case BLACK_P:
					case WHITE_P:
						pawn_moves = 1;
						break;
					case BLACK_K:
					case WHITE_K:
						pawn_moves = 4;
						break;
					case BLACK_R:
					case WHITE_R:
						pawn_moves = 5;
						break;
					case BLACK_N:
					case WHITE_N:
						pawn_moves = 2;
						break;
					case BLACK_Q:
					case WHITE_Q:
						pawn_moves = 10;
						break;
					case BLACK_B:
					case WHITE_B:
						pawn_moves = 5;
						break;
					default:
						break;
				}
				if(player_to_color(board[i][j]) == color)
					pc_count+=pawn_moves;
				else
					player_count+=pawn_moves;
				pawns++;
			}

		}
	}
	player_count = player_count * (64 - pawns) / 64.0;
	pc_count = pc_count * (64 - pawns) / 64.0;
	/* total_moves equation
	total_moves  = moves_count * player_count ^ (depth/2 ) * pc_count ^ ((depth - 1)/2); 
        depth/2 * log(player_count)+(depth-1/2) * log(pc_count) = log( MAX_MOVES/moves_count);
        depth * log(player_count*pc_count) = 2 * log(MAX_MOVES/moves_count)+log(pc_count);	*/
	depth = (2 * log(2 * MAX_MOVES / moves_count) + log(pc_count / 2)) / log(player_count * pc_count / 4);	
	free_all_links(firstmove);
	return depth;
}

void clean_not_allowed(char board[BOARD_SIZE][BOARD_SIZE], LINK *moves, char color)
{
	char temp_board[BOARD_SIZE][BOARD_SIZE];
	LINK *first=moves,*next_link;
	if (moves == NULL)
		return;
	next_link = first->next;
	while(next_link!=NULL)
	{
		memcpy(temp_board,board,BOARD_SIZE*BOARD_SIZE*sizeof(char));
		do_move(temp_board, next_link, color);	
		if(check_check(temp_board, opp_color(color)))
		{
			delete_after(first);
		}
		else
		{
			first = first->next;
		}
		if(first==NULL)
		{
			next_link=NULL;
		}
		else
		{
			next_link=first->next;
		}
	}
}

void add_link(LINK *l1,LINK *l2)
{
	if(l2==NULL) return;
	while(l1->next!=NULL)
	{
		l1=l1->next;
	}
	l1->next=l2;
}

void free_all_links(LINK *head)
{
	LINK *temp;
	while(head!=NULL)
	{
		temp=head;
		head=head->next;
		free(temp);
	}
}

LINK *goto_last(LINK *head)
{
	if(head==NULL) return NULL;
	while(head->next!=NULL)
	{
		head=head->next;
	}
	return head;
}

void delete_after(LINK *link)
{
	LINK *temp;
	temp=link->next->next;
	free(link->next);
	link->next=temp;
}


