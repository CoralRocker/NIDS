#ifndef SAVE_H
#define SAVE_H

#include "object.hpp"
#include "gamelib.hpp"

#include <cstdint>
#include <SDL2/SDL.h>


void loadSave(const char* savefile);
void save(const char* savefile);

typedef struct {
	uint8_t image_side, image_index, image_speed;
	uint16_t depth;
	int16_t direction;
	uint16_t id;
	SDL_Rect posRect, clip, bBox;
	AVAIL_OBJECTS type;
	bool solid, visible, moving;
	ColorCodes colmod;
} SAVE_OBJ;

#endif
