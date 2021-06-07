#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdint.h>

enum AVAIL_OBJECTS {
	NAOMI,
	TABLE, MOD_TABLE, STOOL,
	CABINET, CABINET_TALL, 
	DESK, MOD_DESK, DESK_CHAIR, 
	MIRROR, STANDING_MIRROR,
	BONSAI, SMALL_PLANTS, MEDIUM_PLANTS, LARGE_PLANTS, XLARGE_PLANTS,
	TOILET, BATHROOM_CLUTTER, BATHTUB, SINK_VANITY, SINK_CABINET,
	BED, TV,
	WALL, SIDE_WALL,
	
	MAGGIE,
	OBJ_MAX // Serves to tell how many objects are possible
};

extern uint8_t OBJ_DATA[OBJ_MAX][4]; 
extern SDL_Rect BBOX_DATA[OBJ_MAX];
extern int8_t OBJ_DEPTH_CORRECT[OBJ_MAX];

static SDL_Texture* objectTextures[OBJ_MAX];
void loadObjectTextures();
void cleanObjectTextures();


class Object{
	public:
		// Object Constructor/Destructor
		Object(AVAIL_OBJECTS type=TABLE, uint16_t x=0, u_int16_t y=0, uint16_t id=0xffff);
		~Object();

		// Object Methods
		Object* draw();
		Object* step();
		bool collideWith(SDL_Rect other);
		SDL_Rect colBox();
		Object* getFrameClip(uint8_t side, uint8_t subImg);
		Object* nextFrame(uint32_t frame_no);	
		// void readCSV(std::string csv);
		Object* stretch(float xFactor, float yFactor);
		Object* stretch(float factor);
		Object* resetSize();
		void getCxy(uint16_t (&arr)[2]);
		int8_t depthCorrect();


		// Operators
		bool operator<(Object& other);
		bool operator==(uint16_t oid);

		// Object Variables
		uint8_t image_side, image_index, image_speed, numImg, numSubImg;
		uint16_t depth, id, direction;
		SDL_Rect posRect, clip, bBox;
		SDL_Texture *sprTextures;
		AVAIL_OBJECTS type;
		bool solid, visible;	

		uint8_t walk_speed, step_dis;
		bool moving, pause;
};

#endif
