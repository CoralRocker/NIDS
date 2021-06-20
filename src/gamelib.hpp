#ifndef GAMELIB_H
#define GAMELIB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <string>
#include <cmath>
#include <vector>
#include <memory>

//#include "object.hpp"
//#include "naomi.hpp"

const short SCREEN_WIDTH = 640;//1152;
const short SCREEN_HEIGHT = 480;//852;
extern bool DEBUG, SHOW_FPS, DRAW_GRID, FULLSCREEN; // Used to be const, but better to be on/off-able.
extern uint8_t FPS;
extern double TPF;

enum ColorCodes{
	BLACK,
	WHITE,
	RED,
	GREEN, 
	BLUE,
	PURPLE,
	YELLOW,
	LBLUE,
	PRED,
	ORANGE,
	LGREEN,

	COLOR_MAX
};
extern const char* colorNames[COLOR_MAX];
extern const SDL_Color colors[COLOR_MAX];
extern const uint8_t rgbColors[COLOR_MAX][3];

enum errCodes
{
	SUCCESS,
	INIT_FAIL,
	LOAD_FAIL
};

enum textModes{
	TXT_LEFT,
	TXT_RIGHT,
	TXT_CENTERED,
	TXT_MIDDLE
};

extern SDL_Window* win;
extern SDL_Renderer* winRenderer;
extern std::vector<void*> objects;
extern TTF_Font* fontLrg, *fontMed, *fontSml;
extern bool game_pause;

int roundTo8(int num);

void renderText(SDL_Rect position, const char* str, ColorCodes clrcd, TTF_Font* font, textModes mode=TXT_LEFT);
void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, bool filled=false);
void drawGrid(int gridsize);

float dsin(float direction);
float dcos(float direction);
bool init();
void close();

#endif
