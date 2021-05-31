#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <vector>

#include "gamelib.hpp"
#include "object.hpp"
#include "naomi.hpp"

extern const char *obj_names[OBJ_MAX];

enum MENU_OPTIONS{
	TOGGLE_DEBUG,
	TOGGLE_FPS,

	MENU_MAX
};

class Menu {
	public:
		Menu();
		~Menu();
	
		void input(SDL_Keycode sym);
		void draw();
		SDL_Rect stretch(float x, float y);
		SDL_Rect stretch(float factor);

		Naomi* naomi;
	private:
		std::vector<uint16_t> sel;
		bool* pause, visible;
		SDL_Texture* textBox;
		SDL_Rect txtRect;
};

#endif
