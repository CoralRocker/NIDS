#ifndef SAVE_H
#define SAVE_H

#include "object.hpp"

#include <cstdint>
#include <SDL2/SDL.h>


void loadSave(const char* savefile);
void save(const char* savefile);

typedef struct {
	uint8_t image_side, image_index, image_speed;
	uint16_t depth, direction, id;
	SDL_Rect posRect, clip, bBox;
	AVAIL_OBJECTS type;
	bool solid, visible, moving;
} SAVE_OBJ;

#endif
