files=main.cpp gamelib.cpp naomi.cpp object.cpp menu.cpp

libs=-lSDL2 -lSDL2_image -lSDL2_ttf  `sdl2-config --cflags --libs`



naomi:
	clang++ $(files) $(libs) -o game.o

	
