#include "chessgui.h"
char board[BOARD_SIZE][BOARD_SIZE];
int state = 0; /*0-main window, 1-player selection, 2-settings window, 3-load game,4- game,5- promotion,6- save*/
int from_x = -1;
int from_y = -1;
int to_x = -1;
int to_y = -1;
int game_over = 0;
int check = 0;
int not_found = 0;
SDL_Surface *pieces,*mark,*checkbmp,*matbmp,*fileerr;

int main(int argc,char *argv[])
{
	if(argc == 1 || !strcmp(argv[1],"console"))
	{
		console_main();
	}
	else
	{
		gui_main();
	}
	return 0;
}

int gui_main() {
	SDL_Event e;
	COMP *buttons,*player_buttons,*board_buttons,*settings_buttons,*load_buttons;
	SDL_Surface *w  = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0,depth,i;
	char slot_name[20];
	init_board_(board);
	buttons = Create_Button(0,0,50,50,"images/mainmenu.bmp",NULL);
	buttons = Create_Button(120,200,50,50,"images/new.bmp",buttons);
	buttons = Create_Button(120,300,50,50,"images/load.bmp",buttons);	
	buttons = Create_Button(120,400,50,50,"images/quit.bmp",buttons);
	player_buttons = Create_Button(120,100,50,50,"images/player.bmp",NULL);
	player_buttons = Create_Button(120,200,50,50,"images/pc.bmp",player_buttons);
	player_buttons = Create_Button(120,400,50,50,"images/cancel.bmp",player_buttons);
	player_buttons = Create_Button(0,0,50,50,"images/gamemode.bmp",player_buttons);
	board_buttons = Create_Button(0,0,50,50,"images/board2.bmp",NULL);
	board_buttons = Create_Button(700,200,50,50,"images/save.bmp",board_buttons);
	board_buttons = Create_Button(700,100,50,50,"images/main.bmp",board_buttons);
	board_buttons = Create_Button(700,300,50,50,"images/restart.bmp",board_buttons);
	board_buttons = Create_Button(700,400,50,50,"images/quit.bmp",board_buttons);
	settings_buttons = Create_Button(50,100,50,50,"images/black.bmp",NULL);
	settings_buttons = Create_Button(220,100,50,50,"images/white.bmp",settings_buttons);
	settings_buttons = Create_Button(135,300,50,50,"images/best.bmp",settings_buttons);
	settings_buttons = Create_Button(280,250,50,50,"images/4.bmp",settings_buttons);
	settings_buttons = Create_Button(220,250,50,50,"images/3.bmp",settings_buttons);
	settings_buttons = Create_Button(160,250,50,50,"images/2.bmp",settings_buttons);
	settings_buttons = Create_Button(100,250,50,50,"images/1.bmp",settings_buttons);
	settings_buttons = Create_Button(135,420,50,50,"images/continue.bmp",settings_buttons);
	settings_buttons = Create_Button(0,40,50,50,"images/choosemenu.bmp",settings_buttons);
	load_buttons = Create_Button(135,400,50,50,"images/cancel.bmp",NULL);
	for(i=0;i<NUM_SLOTS;i++)
	{
		sprintf(slot_name,"images/slot%d.bmp",i);
		load_buttons = Create_Button(30 + (i/5)*210,70 + (i%5)*60,50,50,slot_name,load_buttons);
	}
	pieces = SDL_LoadBMP("images/pieces2.bmp");
	if (SDL_SetColorKey(pieces, SDL_SRCCOLORKEY, SDL_MapRGB(pieces->format, 255, 255, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		quit = 1;
	}
	mark = SDL_LoadBMP("images/mark.bmp");
	if (SDL_SetColorKey(mark, SDL_SRCCOLORKEY, SDL_MapRGB(mark->format, 255, 255, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		quit = 1;
	}
	matbmp = SDL_LoadBMP("images/checkmate.bmp");
	if (SDL_SetColorKey(matbmp, SDL_SRCCOLORKEY, SDL_MapRGB(matbmp->format, 255, 255, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		quit = 1;
	}
	checkbmp = SDL_LoadBMP("images/check.bmp");
	if (SDL_SetColorKey(checkbmp, SDL_SRCCOLORKEY, SDL_MapRGB(checkbmp->format, 255, 255, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		quit = 1;
	}
	fileerr = SDL_LoadBMP("images/file.bmp");
	if (SDL_SetColorKey(fileerr, SDL_SRCCOLORKEY, SDL_MapRGB(fileerr->format, 255, 255, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		quit = 1;
	}
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		quit = 1;
	}
	atexit(SDL_Quit);

	/* Create window surface*/

	if (w == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		quit = 1;
	}

	/* Define the rects we need*/
	

	/* Load test spritesheet image*/
/*	
	if (img == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return 1;
	}

	if (SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 0, 0, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		return 1;
	}*/

	while (!quit) {
		switch (state)
		{
			case 0:
				print_main(w,buttons,WIN_W,WIN_H);
				break;
			case 1:
				print_main(w,player_buttons,WIN_W,WIN_H);
				break;
			case 2:
				print_main(w,settings_buttons,WINS_W,WINS_H);
				break;
			case 3:
			case 6:
				print_main(w,load_buttons,WINS_W,WINS_H);
				break;
			case 4:
				print_main(w,board_buttons,WING_W,WING_H);
				if(user_color==opp_color(next_turn) && game_mode == 2 && !game_over) 
				{
					if (minmax_depth == 5)
						depth = compute_depth(board,opp_color(user_color));
					else
						depth = minmax_depth;
					minmax(board,opp_color(user_color),depth,1,-2000,2000);
					if(check_win(board,opp_color(user_color)))
					{
						game_over = 1;
					}
					check = check_check(board,opp_color(user_color));
					next_turn = opp_color(next_turn);
				}
				break;
			case 5:
				print_main(w,board_buttons,WING_W,WING_H);
				break;
			default:
				break;
		}
		/* Green rectangle button
		if (SDL_FillRect(w, &rect, SDL_MapRGB(w->format, 0, 255, 0)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}

		}*/
		
		/* Poll for keyboard & mouse events*/
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					break;
				case (SDL_MOUSEBUTTONDOWN):
					quit = mouse_main(e.button.x,e.button.y);
					break;
				default:
					break;
			}
		}

		SDL_Delay(100);
	}
	Clean_Buttons(buttons);
	Clean_Buttons(player_buttons);
	Clean_Buttons(board_buttons);
	Clean_Buttons(settings_buttons);
	Clean_Buttons(load_buttons);
	SDL_FreeSurface(mark);
	SDL_FreeSurface(checkbmp);
	SDL_FreeSurface(matbmp);
	SDL_FreeSurface(fileerr);
	SDL_FreeSurface(pieces);
	return 0;
}

COMP *Create_Button(int startx,int starty,int width,int height,char *image, COMP *next)
{
	COMP *button = (COMP *)calloc(1,sizeof(COMP));
	button->imgrect.x = startx;
	button->imgrect.y = starty;
	button->imgrect.w = width;
	button->imgrect.h = height;
	button->img = SDL_LoadBMP(image);
	button->next = next;
	return button;	
}

void print_main(SDL_Surface *main,COMP *buttons, int width, int height)
{
	SDL_Rect crop={0,0,74,75},position = {0,0,0,0};
	int i,j,cou=0,condition;
	/* Clear window to WHITE*/
	main =  SDL_SetVideoMode(width , height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (SDL_FillRect(main,0,SDL_MapRGB(main->format,255,255,255)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}
	while(buttons != NULL)
	{
		condition = (state != 2) || ((cou != minmax_depth + 1) && (cou != (user_color == WHITE? 7: 8)));
		if (condition)
			if (SDL_BlitSurface(buttons->img, 0, main, &buttons->imgrect) != 0) {
				SDL_FreeSurface(buttons->img);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				break;
			}
		buttons = buttons->next;
		cou++;
	}
	if (state == 3 && not_found)
	{
		position.x = 50;
		position.y = 0;
		
		if (SDL_BlitSurface(fileerr, 0, main, &position) != 0) {
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		}
	}
	if(state == 4 || state == 5)
	{
		for (i = 0;i<BOARD_SIZE;i++)
		{
			for(j = 0;j<BOARD_SIZE;j++)
			{
				if(board[i][j] == EMPTY)
					continue;
				position.x = 18 + i * 82;
				position.y = 16 + (7-j) * 82;
				crop.y = player_to_color(board[i][j]) == WHITE ? WHITE_Y : BLACK_Y;
				switch (board[i][j])
				{
					case BLACK_P:
					case WHITE_P:
						crop.x = PAWN_X;
						break;
					case BLACK_B:
					case WHITE_B:
						crop.x = BISHOP_X;
						break;
					case BLACK_N:
					case WHITE_N:
						crop.x = KNIGHT_X;
						break;
					case BLACK_R:
					case WHITE_R:
						crop.x = ROOK_X;
						break;
					case BLACK_K:
					case WHITE_K:
						crop.x = KING_X;
						break;
					case BLACK_Q:
					case WHITE_Q:
						crop.x = QUEEN_X;
						break;
					default: 
						break;
				}
				if (SDL_BlitSurface(pieces, &crop, main, &position) != 0) {
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					break;
				}
			}
		}
		if (state == 5)
		{
			crop.y = user_color == WHITE ? WHITE_Y : BLACK_Y;
			crop.x = BISHOP_X;
			position.x = 700;
			position.y = 450;
			if (SDL_BlitSurface(pieces, &crop, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			crop.x = QUEEN_X;
			position.y = 530;
			if (SDL_BlitSurface(pieces, &crop, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			crop.x = ROOK_X;
			position.y = 610;
			if (SDL_BlitSurface(pieces, &crop, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
			crop.x = KNIGHT_X;
			position.y = 690;
			if (SDL_BlitSurface(pieces, &crop, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		if (from_x != -1)
		{
			position.x = 12 + from_x * 82;
			position.y = 12 + (7-from_y) * 82;
			if (SDL_BlitSurface(mark, 0, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		position.x = 40;
		position.y = 700;
		if(game_over)
		{
			if (SDL_BlitSurface(matbmp, 0, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
		else if(check)
		{
			if (SDL_BlitSurface(checkbmp, 0, main, &position) != 0) {
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			}
		}
	}
	/* We finished drawing*/
	if (SDL_Flip(main) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}

}

int mouse_main(int x,int y)
{
	switch (state)
	{
		case 0: /*main menu*/
			if ((x > 120) && (x < 270) && (y > 400) && (y < 440))
				return 1;
			if ((x > 120) && (x < 270) && (y > 200) && (y < 240))
				state = 1;
			if ((x > 120) && (x < 270) && (y > 300) && (y < 340))
				state = 3;
			return 0;
			break;
		case 1: /* player choosing*/
			if ((x > 120) && (x < 270) && (y > 400) && (y < 440))
				state = 0;
			if ((x > 120) && (x < 270) && (y > 100) && (y < 140))
			{
				game_mode = 1;
				state = 4;
			}
			if ((x > 120) && (x < 270) && (y > 200) && (y < 240))
			{
				game_mode = 2;
				state = 2;
			}
			return 0;
			break;
		case 2:
			if ((x > 50) && (x < 200) && (y > 100) && (y < 140))
				user_color = 'b';
			if ((x > 220) && (x < 370) && (y > 100) && (y < 140))
				user_color = 'w';
			if ((x > 135) && (x < 285) && (y > 300) && (y < 340))
				minmax_depth = 5;
			if ((x > 100) && (x < 140) && (y > 250) && (y < 290))
				minmax_depth = 1;
			if ((x > 160) && (x < 200) && (y > 250) && (y < 290))
				minmax_depth = 2;
			if ((x > 220) && (x < 260) && (y > 250) && (y < 290))
				minmax_depth = 3;
			if ((x > 280) && (x < 320) && (y > 250) && (y < 290))
				minmax_depth = 4;
			if ((x > 135) && (x < 285) && (y > 420) && (y < 460))
				state = 4;
			return 0;

			break;
		case 3:
		case 6:
			{
				int i;
				char slot_name[15];
				if ((x > 135) && (x < 285) && (y > 400) && (y < 440))
				{
					not_found = 0;
					state = state == 3?0:4;
				}
				for(i=0;i<NUM_SLOTS;i++)
				{
					if ((x > (30 + (i/5)*210)) && (x < (180 + (i/5)*210)) && (y > (70 + (i%5)*60)) && (y < (100 + (i%5)*60)))
					{
						sprintf(slot_name,"slot%d.xml",i);
						if(state == 3)
						{
							if (load_game(slot_name,board,&game_mode,&minmax_depth,&user_color,&next_turn))
							{
								state = 4;
								not_found = 0;
								game_over = 0;
								check = 0;
							}
							else
								not_found = 1;
						}
						else
						{
							if (save_game(slot_name,board,game_mode,minmax_depth,user_color,user_color))
							{
								state = 4;
								not_found = 0;
							}
							else
								not_found = 1;
						}
					}
				}
			}
			break;
		case 4:
			if ((x > 700) && (x < 850) && (y > 100) && (y < 150))
			{
				check = 0;
				game_over = 0;
				state = 0;
				user_color = WHITE;
				next_turn = WHITE;
				minmax_depth = 1;
				game_mode = 1;
				init_board_(board);
			}
			if ((x > 700) && (x < 850) && (y > 200) && (y < 250))
			{
				state = 6;
			}
			if ((x > 700) && (x < 850) && (y > 300) && (y < 350))
			{
				check = 0;
				game_over = 0;
				init_board_(board);
				if (game_mode == 1)
					user_color = WHITE;
				else
					next_turn = WHITE;
			}
			if ((x > 700) && (x < 850) && (y > 400) && (y < 450))
				return 1;
			if ((x > 18) && (x < 662) && (y > 18) && (y < 662) && !game_over)
			{
				if(from_x == -1)
				{
					from_x = (x-18)/81;
					from_y = 7 - (y-18)/81;
					if (board[from_x][from_y] == EMPTY || player_to_color(board[from_x][from_y]) != user_color)
					{
						from_x = -1;
						from_y = -1;
						break;
					}
				}
				else
				{
					LINK *move,*allowed_moves=possible_moves(board,user_color);
					move=scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
					move->next=NULL;
					sprintf(move->move,"<%c,%d> to <%c,%d>",from_x+'a',from_y+1,(x-12)/82 +'a',7 - (y-12)/82 +1);
					move->prom=check_if_allowed(board,user_color,move,allowed_moves)-1;
					if(move->prom == 1)
					{
						to_x = (x-12)/82;
						to_y = 7- (y-12)/82;
						state = 5;
						free_all_links(allowed_moves);
						free(move);
						break;
					}
					if(move->prom>-1)
					{
						do_move(board,move,user_color);
						if(check_win(board,user_color))
						{
							game_over = 1;
						}
						check = check_check(board,user_color);
						if(game_mode == 2)
						{
							next_turn = opp_color(next_turn);
						}
						else 
							user_color = opp_color(user_color);
					}
					free_all_links(allowed_moves);
					free(move);
					from_x = -1;
					from_y = -1;
				}
			}
			break;
		case 5:
			{
			int choose;
			LINK *move;
			if ((x > 700) && (x < 775) && (y > 450) && (y < 525))
			{
				choose = 1;
				state = 4;
			}
			if ((x > 700) && (x < 775) && (y > 530) && (y < 605))
			{
				choose = 2;
				state = 4;
			}
			if ((x > 700) && (x < 775) && (y > 610) && (y < 685))
			{
				choose = 3;
				state = 4;
			}
			if ((x > 700) && (x < 775) && (y > 690) && (y < 765))
			{
				choose = 4;
				state = 4;
			}
			if (state == 4)
			{
				move=scalloc(1,sizeof(LINK),__FUNCTION__,NULL);
				move->next=NULL;
				sprintf(move->move,"<%c,%d> to <%c,%d>",from_x+'a',from_y+1,to_x +'a',to_y +1);
				sprintf(move->move,"%s %s",move->move,(choose == 1?"bishop":(choose == 2?"queen":(choose == 3?"rook":"knight"))));
				move->prom = 1;
				do_move(board,move,user_color);
				if(check_win(board,user_color))
				{
					game_over = 1;
				}
				check = check_check(board,user_color);
				if(game_mode == 2)
				{
					next_turn = opp_color(next_turn);
				}
				else 
					user_color = opp_color(user_color);
				free(move);
				from_x = -1;
				from_y = -1;
			}
			}
			break;
		default:
			break;
	}
	return 0;
}

void Clean_Buttons(COMP *button) {
	COMP *next;
	while(button != NULL)
	{
		SDL_FreeSurface(button->img);
		next = button;
		button = button->next;
		free(next);
	}
}
