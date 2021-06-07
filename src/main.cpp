#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "gamelib.hpp"
#include "menu.hpp"
#include "object.hpp"
#include "naomi.hpp"

int main(int argc, char** argv){

	// If DEBUG build...
	DEBUG = true;

	// Start SDL
	if(init() != true){
		printf("Initialization Routine failed.\n");
		return INIT_FAIL;
	}
	
	// Clear Window
	SDL_RenderClear(winRenderer);
	
	// Load Media
	loadObjectTextures();
	SDL_Surface* loadedBackground = IMG_Load("images/bkgd.png");
	SDL_Texture* background = NULL;
	if(loadedBackground == NULL){
		printf("Err could not load background image. ERR: %s\n", IMG_GetError());
		cleanObjectTextures();
		SDL_FreeSurface(loadedBackground);
		close();
		return LOAD_FAIL;
	}
	background = SDL_CreateTextureFromSurface(winRenderer, loadedBackground);
	SDL_FreeSurface(loadedBackground);
	if(background == NULL){
		printf("Err could not create texture from surface. ERR: %s\n", SDL_GetError());
		cleanObjectTextures();
		close();
		return LOAD_FAIL;
	}

	// Debug Warning
	if(DEBUG){
		printf("WARNING! RUNNING IN DEBUG MODE!\n");
	}

	// Debug Timing
	uint64_t start64, end64, cps;
	double elapsed64;
	cps = SDL_GetPerformanceFrequency();

	// MAIN LOOP
	bool quit = false;
	uint16_t numObjects = 0;	
	uint32_t frame = 0;
	
	SDL_Event e; // Event handler

	// Menu Object
	Menu* menu = new Menu();

	// Naomi object
	Naomi* naomi = new Naomi(96, 96, 0xffff);
	naomi->step_dist = 8;
	naomi->walk_speed = 4;
	naomi->quit = &quit;
	
	menu->naomi = naomi;

	if(DEBUG) printf("org naomiptr: %p\n", naomi);

	// Walls
	Object* tmpObj;

	objects.push_back(new Object(WALL, 0, 0, objects.size()));
	((Object*)objects.back())->stretch(SCREEN_WIDTH/32.f, 1);
	
	objects.push_back(new Object(SIDE_WALL, 0, 0, objects.size()));
	((Object*)objects.back())->stretch(1, SCREEN_HEIGHT/64.f);
	
	objects.push_back(new Object(SIDE_WALL, SCREEN_WIDTH-32, 0, objects.size()));
	((Object*)objects.back())->stretch(1, SCREEN_HEIGHT/64.f);
	
	objects.push_back(new Object(WALL, 32, SCREEN_HEIGHT/2, objects.size()));
	((Object*)objects.back())->stretch(SCREEN_WIDTH/96.f, 1);

	// MISC Objects
	objects.push_back(new Object(MAGGIE, 180, 180, objects.size()));
	((Object*)objects.back())->getFrameClip(3, 0)->image_speed = 4;

	// FPS Stabilization
	uint32_t start, end, elapsed;

	while(!quit){
		start = SDL_GetTicks();
		if(DEBUG) start64 = SDL_GetPerformanceCounter();
		frame++;
		while(SDL_PollEvent(&e)!=0){
			switch(e.type){
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					// naomi->move(e.key.keysym.sym);
					menu->input(e.key.keysym.sym);
					naomi->input(e.key.keysym.sym);
					switch(e.key.keysym.sym){
						case SDLK_TAB:
							printf("cur naomiptr: %p\n", naomi);
							printf("Frame_no: %u\n", frame);
							printf("Time of last frame (ms): %u\n", elapsed);
							if(DEBUG) printf("High-Perf time of last frame (ms): %f\n\n", elapsed64); else puts("");
							break;
					}
			}
		}
		// Clear Window Then Add Background
		SDL_RenderClear(winRenderer);
		SDL_RenderCopy(winRenderer, background, NULL, NULL);

		// Place naomi object into vector, losing track of it.
		// Sort vector by depth
		// Draw Objects By Depth
		objects.push_back(naomi);
		
		std::sort(objects.begin(), objects.end(), [](void* obj1, void* obj2){return ((Object*)obj1)->depth < ((Object*)obj2)->depth;});

		// Draw All Objects On Screen
		// We have to cast the dereferenced iterator to Object* for this to work.
		for(std::vector<void*>::iterator it = objects.begin(); it != objects.end(); it++){
			if(((Object*)*it)->id == 0xFFFF){
				((Naomi*)*it)->step();
			}else{
				((Object*)*it)->step();
			}
			((Object*)*it)->nextFrame(frame);
			((Object*)*it)->draw();
		}

		
		// uint16_t nID = 0xffff;
		auto it = std::find(objects.begin(), objects.end(), naomi);
		// naomi = (Naomi*)*it;
		objects.erase(it);
			
		// Draw Menu
		menu->draw();

		SDL_RenderPresent(winRenderer);
		

		// FPS Correction, If Needed.
		end = SDL_GetTicks();
		elapsed = end-start;
		

		if(DEBUG){
			end64 = SDL_GetPerformanceCounter();
			elapsed64 = (double)((end64-start64)*1000)/cps;
		}
		if(elapsed < TPF){	
			SDL_Delay(TPF - elapsed);
		}
	}

	delete menu;
	delete naomi;
	for(auto& x: objects){
		delete (Object*)x;
	}
	cleanObjectTextures();
	close(); // Clean Up Proceses

	return SUCCESS;
}
