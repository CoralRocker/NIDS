files= src/main.cpp src/gamelib.cpp src/naomi.cpp src/object.cpp src/menu.cpp src/save.cpp

libs=-lSDL2 -lSDL2_image -lSDL2_ttf  # `sdl2-config --cflags --libs`



naomi:
	clang++ $(files) $(libs) -o NIDS

	
