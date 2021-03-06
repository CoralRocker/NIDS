#include "object.hpp"
#include "gamelib.hpp"
#include "menu.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cmath>

/** Hardcoded Data that tells the game what is contained in the spritesheets.
 */
uint8_t OBJ_DATA[OBJ_MAX][4] = {
	{2, 1, 64, 48}, {1, 3, 32, 48}, {1, 1, 32, 32},
	{1, 3, 32, 48}, {1, 11, 32, 80},
	{1, 6, 64, 48}, {1, 4, 32, 48}, {1, 5, 32, 64},
	{1, 3, 32, 48}, {1, 3, 64, 96}, 
	{1, 2, 32, 48}, {1, 4, 32, 32}, {1, 9, 32, 48}, {1, 10, 32, 64}, {1, 7, 64, 64},
	{1, 8, 32, 80}, {1, 21, 32, 32}, {1, 5, 64, 96}, {1, 1, 64, 96}, {1, 10, 64, 64},
	{1, 4, 80, 96}, {1, 2, 64, 48},

	{1, 1, 32, 64}, {1, 1, 32, 64},
	{4, 4, 32, 48},
	{4, 6, 48, 32}
};

/** Hardcoded bounding box data for objects.
 */
SDL_Rect BBOX_DATA[OBJ_MAX] = {
	{8, 16, 48, 24}, {0, 16, 32, 24}, {0, 0, 32, 32},
	{0, 16, 32, 32}, {0, 32, 32, 32},
	{0, 0, 64, 32}, {0, 2, 32, 45}, {0, 16, 32, 32},
	{0, 0, 32, 32}, {12, 48, 40, 32},
	{0, 31, 31, 15}, {0, 0, 31, 31}, {0, 15, 31, 31}, {0, 31, 31, 31}, {0, 32, 63, 31},
	{0, 15, 31, 48}, {2, 0, 27, 29}, {0, 0, 63, 63}, {0, 47, 63, 32}, {0, 15, 63, 32},
	{7, 31, 65, 48}, {2, 6, 57, 39},

	{0, 32, 32, 32}, {0, 0, 32, 64},
	{0, 32, 32, 16}, // {x, y, w, h}
	{0, 0, 48, 32}
};

/** Depth Used to be hardcoded based on bounding boxes,
 * but using a correction can help the game feel more real.
 */
int8_t OBJ_DEPTH_CORRECT[OBJ_MAX] = {
	0, 0, -32,
	0, 0,
	0, 0, 0,
	0, 16, 
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 
	0, 0, 

	0, 0, 
	0,
	0
};

/** Function to load textures for all the objects into memory at once.
 * The Textures are only created once, and are simply referenced by all the objects.
 */
void loadObjectTextures(){
	//objectTextures = SDL_Texture[OBJ_MAX];
	for(char i = 0; i < OBJ_MAX; i++){
		objectTextures[i] = NULL;
		SDL_Surface* loadedSpritesheet = NULL;
		switch(i){
			case NAOMI:
				loadedSpritesheet=IMG_Load("spritesheets/naomi.png");
				break;
			case TABLE:
				loadedSpritesheet=IMG_Load("spritesheets/spr_table.png");
				break;
			case MOD_TABLE:
				loadedSpritesheet=IMG_Load("spritesheets/spr_mod_table.png");
				break;
			case STOOL:
				loadedSpritesheet=IMG_Load("spritesheets/spr_stool.png");
				break;
			case CABINET:
				loadedSpritesheet=IMG_Load("spritesheets/spr_cabinet.png");
				break;
			case CABINET_TALL:
				loadedSpritesheet=IMG_Load("spritesheets/spr_tall_cabinet.png");
				break;
			case DESK:
				loadedSpritesheet=IMG_Load("spritesheets/spr_desk.png");
				break;
			case MOD_DESK:
				loadedSpritesheet=IMG_Load("spritesheets/spr_mod_desk.png");
				break;
			case DESK_CHAIR:
				loadedSpritesheet=IMG_Load("spritesheets/spr_desk_chair.png");
				break;
			case MIRROR:
				loadedSpritesheet=IMG_Load("spritesheets/spr_mirror.png");
				break;
			case STANDING_MIRROR:
				loadedSpritesheet=IMG_Load("spritesheets/spr_standing_mirror.png");
				break;
			case BONSAI:
				loadedSpritesheet=IMG_Load("spritesheets/spr_bonsai.png");
				break;
			case SMALL_PLANTS:
				loadedSpritesheet=IMG_Load("spritesheets/spr_plants_small.png");
				break;
			case MEDIUM_PLANTS:
				loadedSpritesheet=IMG_Load("spritesheets/spr_plants_medium.png");
				break;
			case LARGE_PLANTS:
				loadedSpritesheet=IMG_Load("spritesheets/spr_plants_large.png");
				break;
			case XLARGE_PLANTS:
				loadedSpritesheet=IMG_Load("spritesheets/spr_plants_xlarge.png");
				break;
			case TOILET:
				loadedSpritesheet=IMG_Load("spritesheets/spr_toilet.png");
				break;
			case BATHROOM_CLUTTER:
				loadedSpritesheet=IMG_Load("spritesheets/spr_bathroom_clutter.png");
				break;
			case BATHTUB:
				loadedSpritesheet=IMG_Load("spritesheets/spr_bathtub.png");
				break;
			case SINK_VANITY:
				loadedSpritesheet=IMG_Load("spritesheets/spr_sink_vanity.png");
				break;
			case SINK_CABINET:
				loadedSpritesheet=IMG_Load("spritesheets/spr_sink_cabinet.png");
				break;
			case BED:
				loadedSpritesheet=IMG_Load("spritesheets/spr_bed.png");
				break;
			case TV:
				loadedSpritesheet=IMG_Load("spritesheets/spr_TV.png");
				break;
			case WALL:
				loadedSpritesheet=IMG_Load("spritesheets/spr_wall.png");
				break;
			case SIDE_WALL:
				loadedSpritesheet=IMG_Load("spritesheets/spr_wall_side.png");
				break;
			case MAGGIE:
				loadedSpritesheet=IMG_Load("spritesheets/spr_maggie.png");
				break;
		}
		if(loadedSpritesheet == NULL){
			printf("Err loading object %d. ERR: %s\n", i, IMG_GetError());
			return;
		}
		objectTextures[i] = SDL_CreateTextureFromSurface(winRenderer, loadedSpritesheet);
		SDL_FreeSurface(loadedSpritesheet);
		if(objectTextures[i] == NULL){
			printf("Err creating texture from surface for object %d. ERR: %s\n", i, SDL_GetError());
			return;
		}
	

	}
}

/** Destroys all the preloaded object textures.
 */
void cleanObjectTextures(){
	for(int i = 0; i < OBJ_MAX; i++){
		SDL_DestroyTexture(objectTextures[i]);
	}
}

/** Object Constructor
 * SDL_Window* win: the SDL window for the game
 * AVAIL_OBJECTS type: An enum telling the program the type of object to load
 * uint16_t x, y: the x and y coordinates the object starts at
 * uint16_t depth: the depth at which the object is drawn
 * uint16_t id: A unique identifier for the object.
 */
Object::Object(AVAIL_OBJECTS type, uint16_t x, uint16_t y, uint16_t id){
	this->id = id;
	this->type = type;
	sprTextures = objectTextures[type];
	
	this->depth = y + OBJ_DEPTH_CORRECT[type];

	posRect.x = x;
	posRect.y = y;
	posRect.w = OBJ_DATA[type][2];
	posRect.h = OBJ_DATA[type][3];
	numImg = OBJ_DATA[type][0];
	numSubImg = OBJ_DATA[type][1];
	image_speed = 0;
	image_index = 0;
	image_side = 0;
	direction = 0;
	switch(type){
		case SMALL_PLANTS:
		case STOOL:
			solid = false;
			break;
		default:
			solid = true;
			break;
	}
	bBox = BBOX_DATA[type];
	visible = true;
	colormod = WHITE;
	getFrameClip(0, 0);
}

// Object Destructor. Doesn't actually do much.
Object::~Object(){
	if(DEBUG) printf("Object %X Destroyed\n", id);
}

/** Returns the object's current collision box,
 * updated with the correct x and y coordinates.
 * Does not modify bBox.
 */
SDL_Rect Object::colBox(){

	SDL_Rect tBox = bBox;
	tBox.x += posRect.x;
	tBox.y += posRect.y;
	return tBox;
}

/** Checks for collisions between the current object and 
 * another object. The other object only has it's collision
 * box passed, not the entire object.
 */
bool Object::collideWith(SDL_Rect other){
	SDL_Rect tBox = colBox();
	int leftA = tBox.x, leftB = other.x;
	int rightA = tBox.x + tBox.w, rightB = other.x + other.w;
	int topA = tBox.y, topB = other.y;
	int bottomA = tBox.y + tBox.h, bottomB = other.y + other.h;

	if((bottomA <= topB) || (bottomB <= topA)){
		return false;
	}
	if((rightA <= leftB) || (rightB <= leftA)){
		return false;
	}

	return true;	
}



Object* Object::stretchFitX(int w){
	float factor = w/posRect.w;
	posRect.w = w;
	bBox.w = (uint16_t) nearbyint(bBox.w*factor);
	bBox.x = (uint16_t) nearbyint(bBox.x*factor);
	
	return this;
}

Object* Object::stretchFitY(int h){
	float factor = h/posRect.h;
	posRect.h = h;
	bBox.h = (uint16_t) nearbyint(bBox.h*factor);
	bBox.y = (uint16_t) nearbyint(bBox.y*factor);

	return this;
}

/** Stretch an object's x and y directions by xFactor and yFactor
 * respectively. This changes both the bounding box and the
 * SDL_Rect which defines the drawing of the object.
 */
Object* Object::stretch(float xFactor, float yFactor){
	if(game_pause) return this;

	// Change Size of Bounding Box
	SDL_Rect tBox = bBox;
	tBox.x = (uint16_t) nearbyint(tBox.x * xFactor);
	tBox.y = (uint16_t) nearbyint(tBox.y * yFactor);
	tBox.w = (uint16_t) nearbyint(tBox.w * xFactor);
	tBox.h = (uint16_t) nearbyint(tBox.h * yFactor);
	bBox = tBox;

	// Change Size of Position Rect, but not coordinates
	SDL_Rect tPos = posRect;
	tPos.w = (uint16_t) nearbyint(tPos.w * xFactor);
	tPos.h = (uint16_t) nearbyint(tPos.h * yFactor);
	posRect = tPos;

	return this;
}

/** Same as the above function, but the same factor is 
 * applied to both the x and y coordinates. 
 */
Object* Object::stretch(float factor){
	return stretch(factor, factor);
}

/** Reset an object's bounding box and position rect 
 * the their default values. This does not change an
 * object's x and y position.
 */
Object* Object::resetSize(){
	if(game_pause) return this;

	posRect = {posRect.x, posRect.y, OBJ_DATA[type][2], OBJ_DATA[type][3]};
	bBox = BBOX_DATA[type];
	return this;
}


/** Used only for maggie so far
 */
Object* Object::step(){
	if(game_pause) return this;
	if(type != MAGGIE) return this;

	short dir = rand() % (MAGGIE_MOVE * 3);
	
	if(!moving && dir == 1){
		moving = true;
		direction = 90 * (rand() % 4);
		getFrameClip(direction / 90, 0);
		correctAngle();
		image_speed = 4;
	}

	if(moving){
	
		bool success = true;

		SDL_Rect place = colBox();
		place.y -= dsin(direction)*8;
		place.x += dcos(direction)*8;
		for(std::vector<void*>::iterator it = objects.begin(); it != objects.end(); it++){
			if(!((Object*)*it)->solid) continue;
			if(*it == this) continue;	
			SDL_Rect tBox = ((Object*)*it)->colBox();
			SDL_bool res = SDL_HasIntersection(&place, &tBox);
			if(res == SDL_TRUE)
				success = false;
		}

		if(success){
			posRect.x += dcos(direction);
			posRect.y += dsin(direction);
		}else{
			moving = false;
		}

		depth = posRect.y;

		if((rand() % 3 == 0) && posRect.x % 8 == 0 && posRect.y % 8 == 0) moving = false;
	}else{
		if(image_index % 2){
			if(++image_index >= numSubImg) image_index = 0;
		}

		image_speed = 0;
		if(posRect.x%8) posRect.x -= 1;
		if(posRect.y%8) posRect.y -= 1;
	}
	


	return this;
}	

void Object::correctAngle(){
	SDL_Rect pos = posRect, box = bBox;
	
	switch(numImg){
		case 4:
			switch(direction){
				case 180:
				case 0:
					resetSize();
					break;
				case 270:
				case 90:
					double xFact = bBox.x / (double)posRect.w;
					double yFact = bBox.y / (double)posRect.h;
					bBox.x = std::nearbyint(xFact * posRect.h);
					bBox.y = std::nearbyint(yFact * posRect.w);
					bBox.w = (bBox.w/(double)posRect.w)*posRect.h;
					bBox.h = (bBox.h/(double)posRect.h)*posRect.w;
					
					posRect.w = posRect.h;
					posRect.h = pos.w;
					break;
			}
			break;
		case 2:
			switch(direction){
				case 0:
					resetSize();
					break;
				case 90:
					double xFact = bBox.x / (double)posRect.w;
					double yFact = bBox.y / (double)posRect.h;
					bBox.x = std::nearbyint(xFact * posRect.h);
					bBox.y = std::nearbyint(yFact * posRect.w);
					bBox.w = (bBox.w/(double)posRect.w)*posRect.h;
					bBox.h = (bBox.h/(double)posRect.h)*posRect.w;
					
					posRect.w = posRect.h;
					posRect.h = pos.w;
					break;
			}
			break;
	}	

}

/** Set The Current Sprite
 * uint8_t side, subImg: the side number of the sprite, and the subimage number within that side
 */
Object* Object::getFrameClip(uint8_t side, uint8_t subImg){
	clip = {subImg*OBJ_DATA[type][2], side*OBJ_DATA[type][3], OBJ_DATA[type][2], OBJ_DATA[type][3]};
	image_index = subImg;
	image_side  = side;
	
	return this;
}

/** Increment the frame for animation
 */
Object* Object::nextFrame(uint32_t frame_no){
	if(game_pause) return this;

	if(image_speed == 0)
		return this;
	if(frame_no % image_speed == 0){
		this->image_index++;
		if(image_index >= numSubImg)
			this->image_index = 0;
	}
	
	getFrameClip(image_side, image_index);

	return this;
}

/** Barebones Draw Method
 */
Object* Object::draw(){
	if(!visible) return this;

	if(colormod != WHITE)
		SDL_SetTextureColorMod(sprTextures, rgbColors[colormod][0], rgbColors[colormod][1], rgbColors[colormod][2]);
	
	if(numImg == 1 || id == 0xffff) // If only one side or if is Naomi
		SDL_RenderCopy(winRenderer, sprTextures, &clip, &posRect); 
	else{
		SDL_Rect tmp;
		switch(numImg){
			case 2:
				tmp = {posRect.x+(direction!=0?clip.h:0), posRect.y, clip.w, clip.h};
				break;
			case 4:
				tmp = {posRect.x, posRect.y, clip.w, clip.h};
				switch(direction){
					case 90:
						tmp.x += clip.h;
						break;
					case 180:
						tmp.x += clip.w;
						tmp.y += clip.h;
						break;
					case 270:
						tmp.y += clip.w;
						break;
				}
				break;
		}
		SDL_Point cntr = {0,0};
		SDL_RenderCopyEx(winRenderer, sprTextures, &clip, &tmp, direction, &cntr, direction == 180? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
	}

	if(colormod != WHITE)
		SDL_SetTextureColorMod(sprTextures, 255, 255, 255);
	
	if(DEBUG){
		SDL_Rect cbox = colBox();
		SDL_RenderDrawRect(winRenderer, &cbox);
		SDL_Rect sBox = {posRect.x, posRect.y, posRect.w, posRect.h};
		SDL_RenderDrawRect(winRenderer, &sBox);
		SDL_RenderDrawLine(winRenderer, posRect.x, depth, posRect.x+clip.w, depth);
	}

	return this;
}

/** Get X and Y coordinates of the center of the object.
 * Does not return anything. Just places the x and y coords
 * into the first and second indeces of the passed array.
 */
void Object::getCxy(uint16_t (&arr)[2]){
	uint16_t cx, cy;
	cx = posRect.x + posRect.w/2;
	cy = posRect.y + posRect.h/2;
	arr[0] = cx;
	arr[1] = cy;
}

/** Method to get the depth correction for the current obj.
 * Will be useful when placing down objects and modifying their
 * depths for decorative purposes.
 */
int8_t Object::depthCorrect(){
	return OBJ_DEPTH_CORRECT[type];
}

/** These two functions either increase or decrease the image_index,
 * wrapping around in either direction.
 */
void Object::decImg(){
	int simg = image_index - 1;
	if(simg < 0) simg = numSubImg-1;
	getFrameClip(image_side, simg);
}
void Object::incImg(){
	int simg = image_index + 1;
	if(simg >= numSubImg) simg = 0;
	getFrameClip(image_side, simg);
}

/** Compare Objects by their depth
 */
bool Object::operator<(Object& other){
	return depth < other.depth;
}

/** Compare Objects by their IDs
 */
bool Object::operator==(uint16_t oid){
	return id == oid;
}

/** Prints out object information. Used for debugging objects.
 */
void Object::objDump(){
	printf("OBJ %X:\n", id);
	printf("\tType: %d: %s\n", type, obj_names[type]);
	printf("\tSides, Subimages: (%d, %d)\n", numImg, numSubImg);
	printf("\tCurrent Side/SubImg: (%d, %d)\n", image_side, image_index);
	printf("\tDirection: %d\n", direction);
	printf("\tPosition: (%d, %d)\n", posRect.x, posRect.y);
	printf("\tWidth, Height: (%d,%d\n", posRect.w, posRect.h);
	printf("\tbBox: (%d, %d, %d, %d)\n", bBox.x, bBox.y, bBox.w, bBox.h);
	printf("\tSolid: %s\n", solid?"true":"false");
	printf("\tColorMod: %s\n", colorNames[colormod]);
}
