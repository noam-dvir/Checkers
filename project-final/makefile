all: chess

clean:
	-rm chessgui.o Moves.o Settings.o Chess.o chessprog

chess: chessgui.o Chess.o Settings.o Moves.o
	gcc  -o chessprog chessgui.o Settings.o Moves.o Chess.o -ansi -pedantic-errors -g `sdl-config --libs ` -lm -ldl

chessgui.o: chessgui.c
	gcc  -ansi -pedantic-errors -c -Wall -g chessgui.c `sdl-config --cflags ` -lm -ldl

Chess.o: Chess.c
	gcc -ansi -pedantic-errors -c -Wall -g -lm Chess.c

Settings.o: Settings.c
	gcc -ansi -pedantic-errors -c -Wall -g -lm Settings.c
	
Moves.o: Moves.c
	gcc -ansi -pedantic-errors -c -Wall -g -lm Moves.c
