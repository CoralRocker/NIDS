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

#include "gamelib.hpp"
#include "object.hpp"
#include "naomi.hpp"

SDL_Window* win = NULL;
SDL_Renderer* winRenderer = NULL;
TTF_Font *fontLrg = NULL, *fontMed = NULL, *fontSml = NULL;

bool game_pause = false, DEBUG=false, SHOW_FPS=false, DRAW_GRID=false;

uint8_t FPS = 60;
double TPF = 1000.f/FPS;

std::vector<void*> objects;

const SDL_Color colors[COLOR_MAX] = {
	{0,0,0,255}, {255, 255, 255, 255},
	{255, 0, 0, 255}, 
	{0, 128, 0, 255},
	{0, 0, 255, 255},
	{128, 0, 128, 255},
	{249, 214, 45, 0},
	{0, 219, 221, 255},
	{253, 134, 134, 255},
	{255, 167, 124, 255},
	{181, 212, 102, 255}
};

void renderText(SDL_Rect position, const char* str, ColorCodes clrcd, TTF_Font* font, textModes mode){
	SDL_Color clr = colors[clrcd];

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, str, clr);
	if(textSurface == NULL){
		printf("Err could not create surface from text. ERR: %s\n", TTF_GetError());
		return;
	}
	SDL_Texture* sTexture = SDL_CreateTextureFromSurface(winRenderer, textSurface);
	if(sTexture == NULL){
		printf("Err could not create texture from text surface. ERR: %s\n", SDL_GetError());
		return;
	}
	
	position.w = textSurface->w;
	position.h = textSurface->h;
	
	switch(mode){
		case TXT_LEFT:
			break;
		case TXT_RIGHT:
			position.x -= position.w;
			break;
		case TXT_CENTERED:
			position.x -= position.w/2;
			break;
		case TXT_MIDDLE:
			position.x -= position.w/2;
			position.y -= position.h/2;
			break;
	}
	SDL_RenderCopy(winRenderer, sTexture, NULL, &position);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(sTexture);
}

void drawGrid(int gridsize){
	int numCols = SCREEN_WIDTH/gridsize, numRows = SCREEN_HEIGHT/gridsize;
	for(int i = 1; i <= numCols; i++){
		SDL_RenderDrawLine(winRenderer, i*gridsize, 0, i*gridsize, SCREEN_HEIGHT);
	}
	for(int i = 1; i <= numRows; i++){
		SDL_RenderDrawLine(winRenderer, 0, i*gridsize, SCREEN_WIDTH, i*gridsize);
	}
}

void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, bool filled){
	double PI = 3.14159265;
	double PIH = PI/2.f;

	uint16_t x, y, x1, y1;
	double theta;


	uint8_t precision = 30; // Precision with which to draw each of the quadrants. 
	// The lower the precision, the faster this'll draw. 
	
	double step = PIH/(double)precision;
	for(uint16_t rad = filled ? 1 : r; rad <= r; rad++){	
		x = (float)rad * cos(theta);
		y = (float)rad * sin(theta);
		x1 = x; y1 = y;

		for(theta = step; theta <= PIH; theta += step){
			x1 = (float)rad * cos(theta) + 0.5;
			y1 = (float)rad * sin(theta) + 0.5;

			if(x != x1 || y != y1){
				SDL_RenderDrawLine(winRenderer, x0+x, y0-y, x0+x1, y0-y1); // Quad 1
				SDL_RenderDrawLine(winRenderer, x0-x, y0-y, x0-x1, y0-y1); // Quad 2
				SDL_RenderDrawLine(winRenderer, x0-x, y0+y, x0-x1, y0+y1); // Quad 3
				SDL_RenderDrawLine(winRenderer, x0+x, y0+y, x0+x1, y0+y1); // Quad 4
			}
			x = x1; y = y1;
		}
		if(x!=0){
			x=0;
			SDL_RenderDrawLine(winRenderer, x0+x, y0-y, x0+x1, y0-y1); // Quad 1
			SDL_RenderDrawLine(winRenderer, x0-x, y0-y, x0-x1, y0-y1); // Quad 2
			SDL_RenderDrawLine(winRenderer, x0-x, y0+y, x0-x1, y0+y1); // Quad 3
			SDL_RenderDrawLine(winRenderer, x0+x, y0+y, x0+x1, y0+y1); // Quad 4
		}
	}
}	

float dsin(float direction){
	float result, PI = 3.14159265;
	result = sin(direction*PI/180);
	return result;
}

float dcos(float direction){
	float result, PI = 3.14159265;
	result = cos(direction*PI/180);
	return result;
}

int roundTo8(int num){
	return 8*nearbyint(num/8);
}

void close(){
	SDL_DestroyRenderer(winRenderer);
	SDL_DestroyWindow(win);
	TTF_CloseFont(fontLrg);
	TTF_CloseFont(fontMed);
	TTF_CloseFont(fontSml);
	win = NULL;
	winRenderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool init(){

	bool success = true;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL failed initialization! ERR: %s\n", SDL_GetError());
		success = false;
	}else{
		// Create Window
		win = SDL_CreateWindow("Naomi Interior Design Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);//SDL_WINDOW_FULLSCREEN);
		
		if(win == NULL){
			printf("Error creating window. ERR: %s\n", SDL_GetError());
			success = false;
		}else{
			// Create Renderer
			winRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
			if(winRenderer == NULL){
				printf("Error creating renderer. ERR: %s\n", SDL_GetError());
				success = false;
			}else{
				
				SDL_SetRenderDrawColor(winRenderer, 0x00, 0x00, 0x00, 0xFF);

				// Initialize SDL_Image
				if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
					printf("SDL_image could not initialize! ERR: %s\n", IMG_GetError());
					success = false;
				}else{
					if(TTF_Init() == -1){
						printf("Err could not init TTF. ERR: %s\n", TTF_GetError());
						success = false;
					}else{
						fontLrg = TTF_OpenFont("fonts/Oxygen-Regular.ttf", 36);
						if(!fontLrg){
							printf("Err could not load large font. ERR %s\n", TTF_GetError());
							success = false;
						}
						
						fontMed = TTF_OpenFont("fonts/Oxygen-Regular.ttf", 24);
						if(!fontMed){
							printf("Err could not load medium font. ERR %s\n", TTF_GetError());
							success = false;
						}
						fontSml = TTF_OpenFont("fonts/Oxygen-Regular.ttf", 12);
						if(!fontSml){
							printf("Err could not load small font. ERR %s\n", TTF_GetError());
							success = false;
						}
					}
				}
			}
		}
	}
	return success;
}

