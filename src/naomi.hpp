#ifndef NAOMI_H
#define NAOMI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdint.h>

#include "object.hpp"
#include "gamelib.hpp"

class Naomi : public Object {
	public:
		Naomi(uint16_t x=0, uint16_t y=0, uint16_t id=0xffff);
		
		void step();
		void move(SDL_Keycode sym);
		bool placeFree(SDL_Rect place);
		void input(SDL_Keycode sym);
		void objectPlace();
		void findNearest();

		const uint8_t* keyboard;
		
		bool* quit;
		uint8_t walk_speed, step_dist;
		bool moving, ghost;
		ColorCodes objcolmod;
		AVAIL_OBJECTS objtype;
		Object* heldObject;
};

#endif
