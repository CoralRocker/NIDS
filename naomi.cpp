#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <cstdint>
#include <algorithm>
#include <cmath>

#include "gamelib.hpp"
#include "naomi.hpp"
#include "object.hpp"

/** Naomi Object constructor. This is essentially the same
 * as the default object constructor, but it can only create
 * Naomi objects (duh). It initializes the default walking
 * speed and step distance.
 */
Naomi::Naomi(SDL_Window* win, uint16_t x, u_int16_t y, uint16_t id) : Object(win, NAOMI, x, y, id){
	moving = false;
	walk_speed = 4;
	step_dist = 16;
	keyboard = SDL_GetKeyboardState(NULL);
	objtype = OBJ_MAX;
	heldObject = NULL;
}

/** The step function runs every frame of the game. 
 * It moves the naomi object according to the keys
 * given to the Naomi::move function and is in charge
 * of ensuring that the object stays snapped to a
 * grid that is set by step_dist. 
 */
void Naomi::step(){
	if(game_pause) return;

	if(moving){
		if(image_speed == 0) image_speed = walk_speed;
		posRect.y -= dsin(direction);
		posRect.x += dcos(direction);
		
		depth = posRect.y;

		if(posRect.y % step_dist == 0 && posRect.x % step_dist == 0) moving = false;
		if(heldObject){
			heldObject->posRect.x += dcos(direction);
			heldObject->posRect.y -= dsin(direction);
			heldObject->depth = heldObject->posRect.y;
		}
	
	}else{
		if(image_index == 1 || image_index == 2)
			image_index = 2;
		else	
			image_index = 0;
		image_speed = 0;
		if(posRect.x%step_dist) posRect.x -= 1;
		if(posRect.y%step_dist) posRect.y -= 1;
	}
}

/** This function takes in a keycode from the main game 
 * loop and decides which direction to move Naomi, if any.
 *
 */
void Naomi::move(SDL_Keycode sym){
	// Check if naomi is already moving. Exit if yes.
	if(posRect.x%step_dist or posRect.y%step_dist or moving){
		return;
	}
	switch(sym){
		case SDLK_RIGHT:
			direction = 0;
			break;
		case SDLK_UP:
			direction = 90;
			break;
		case SDLK_LEFT:
			direction = 180;
			break;
		case SDLK_DOWN:
			direction = 270;
			break;
		default:
			return;
	}

	if(heldObject){
		switch(direction){
			case 0:
				heldObject->posRect.x = posRect.x + 32;
				heldObject->posRect.y = posRect.y + 16;
				break;
			case 90:
				heldObject->posRect.x = posRect.x - heldObject->posRect.w/2 + 16;
				heldObject->posRect.y = posRect.y - heldObject->posRect.h + 16;
				break;
			case 180:
				heldObject->posRect.x = posRect.x - heldObject->posRect.w;
				heldObject->posRect.y = posRect.y + 16;
				break;
			case 270:
				heldObject->posRect.x = posRect.x - heldObject->posRect.w/2 + 16;
				heldObject->posRect.y = posRect.y + 48;
				break;
		}
	}

	uint8_t side = direction/90; // 2/pi * theta(radians) gives the proper image index. Simplified to degrees/90.
	getFrameClip(side, image_index);
	if(keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT])
		return;

	// Check that the spot to move to is free.
	SDL_Rect tBox = colBox();
	tBox.y -= dsin(direction)*step_dist;
	tBox.x += dcos(direction)*step_dist;
	SDL_Point p = {tBox.x, tBox.y};
	if(!placeFree(p))
		return;

	moving = true;
}

void Naomi::findNearest(){
	if(heldObject) return;

	uint16_t sx, sy;
	switch(direction){
		case 0:
			sx = posRect.x + 32;
			sy = posRect.y + 16;
			break;
		case 90:
			sx = posRect.x + 16;
			sy = posRect.y;
			break;
		case 180:
			sx = posRect.x;
			sy = posRect.y + 16;
			break;
		case 270:
			sx = posRect.x + 16;
			sy = posRect.y - posRect.h;
			break;
	}

	// std::sort(objects.begin(), objects.end(), [](void* obj1, void* obj2){ return ((Object*)obj1)->depth < ((Object*)obj2)->depth;});

	
	Object* closest;
	float dist = SCREEN_WIDTH+SCREEN_HEIGHT; 
	for(auto& x: objects){
		Object* tmp = (Object*)x;
		if(tmp->type == WALL || tmp->type == SIDE_WALL || tmp->id == id) continue;
		float tdist;
		uint16_t pos[2];
		tmp->getCxy(pos);
		tdist = std::hypot((sx - pos[0]), (sy - pos[1]));
		// if((dist - tdist) > std::sqrt(2)){
		if(tdist < dist){
			dist = tdist;
			closest = tmp;
		}
	}

	heldObject = closest;
	if(objtype == OBJ_MAX) objtype = closest->type;
}

/** Handle input from the keyboard and decide what to do with it.
 *
 */
void Naomi::input(SDL_Keycode sym){
	if(game_pause) return;

	switch(sym){
		// Handle Movement Keys
		case SDLK_RIGHT:
		case SDLK_LEFT:
		case SDLK_UP:
		case SDLK_DOWN:
			move(sym);
			break;
		case SDLK_q:
			*quit = true;
			break;
		case SDLK_SPACE:
			if(objtype == OBJ_MAX) break;
			if(heldObject) heldObject = NULL; else objectPlace(); // Release grasp of object if we're holding one
			break;
		case SDLK_ESCAPE:
			if(heldObject && objtype != OBJ_MAX) objectPlace();
			break;
		case SDLK_f:
			findNearest();				
			break;
		default:
			break;
	}
}

/** Handles the placement and controlling of of Objects
 */
void Naomi::objectPlace(){
	if(heldObject == NULL){
		uint8_t w = OBJ_DATA[objtype][2], h = OBJ_DATA[objtype][3];	
		uint16_t x, y;
		switch(direction){
			case 0:
				x = posRect.x + 32;
				y = posRect.y + 16;
				break;
			case 90:
				x = posRect.x - heldObject->posRect.w/2 + 16;
				y = posRect.y - h + 16;
				break;
			case 180:
				x = posRect.x - w;
				y = posRect.y + 16;
				break;
			case 270:
				x = posRect.x - heldObject->posRect.w/2 + 16;
				y = posRect.y + 48;
				break;
		}
		Object* tObj = new Object(win, objtype, x, y, objects.size());
		heldObject = tObj;
		objects.push_back(tObj);
	}else{
		objects.erase(std::find(objects.begin(), objects.end(), heldObject));
		delete heldObject;
		heldObject = NULL;
	}
}

/** Checks if the place pointed to by `place` is free of any 
 * objects. This is used when deciding whether or not to move 
 * the Naomi object.
 */
bool Naomi::placeFree(SDL_Point place){
	for(std::vector<void*>::iterator it = objects.begin(); it != objects.end(); it++){
		SDL_Rect tBox = ((Object*)*it)->colBox();
		SDL_bool res = SDL_PointInRect(&place, &tBox);
		if(res == SDL_TRUE)
			return false;
	}
	return true;
}
