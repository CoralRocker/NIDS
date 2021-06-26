#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <unistd.h>

#include "gamelib.hpp"
#include "menu.hpp"
#include "object.hpp"
#include "naomi.hpp"
#include "save.hpp"

int main(int argc, char** argv){

	// If DEBUG build...
	// DEBUG = true;

	// Start SDL
	if(init() != true){
		printf("Initialization Routine failed.\n");
		return INIT_FAIL;
	}
	
	// Clear Window
	SDL_RenderClear(winRenderer);
	
	SDL_RenderSetLogicalSize(winRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	//SDL_RenderSetScale(winRenderer, 2, 2);

	// Load Media
	loadObjectTextures();

	// Load background of game
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
	Naomi* naomi;

	if(!(access("sav.nidsav", F_OK) == 0)){
		// Naomi object
		naomi = new Naomi(96, 96, 0xffff);
		naomi->step_dist = 8;
		naomi->walk_speed = 4;
		naomi->quit = &quit;
		
		menu->naomi = naomi;

		// Walls
		Object* tmpObj;
		
		//T
		objects.push_back(new Object(WALL, 0, 0, objects.size()));
		((Object*)objects.back())->stretch(SCREEN_WIDTH/32.f, 1);
		
		//L
		objects.push_back(new Object(SIDE_WALL, 0, 0, objects.size()));
		((Object*)objects.back())->stretch(1, SCREEN_HEIGHT/64.f);
		
		//R
		objects.push_back(new Object(SIDE_WALL, SCREEN_WIDTH-32, 0, objects.size()));
		((Object*)objects.back())->stretch(1, SCREEN_HEIGHT/64.f);
			
		// B 
		objects.push_back(new Object(SIDE_WALL, 0, SCREEN_HEIGHT, objects.size()));
		((Object*)objects.back())->stretchFitX(SCREEN_WIDTH);
		
		//Right Mid wall
		objects.push_back(new Object(WALL, 32, SCREEN_HEIGHT/2, objects.size()));
		((Object*)objects.back())->stretchFitX(roundTo8(SCREEN_WIDTH/7.f * 2));//stretch(SCREEN_WIDTH/96.f, 1);
		
		//Left Mid wall
		objects.push_back(new Object(WALL, roundTo8(SCREEN_WIDTH/7*4),SCREEN_HEIGHT/3 * 2, objects.size()));
		((Object*)objects.back())->stretchFitX(roundTo8(SCREEN_WIDTH/7*3 - 24));

		//Top Laundry wall
		objects.push_back(new Object(SIDE_WALL, roundTo8(SCREEN_WIDTH/7 * 4), 0, objects.size()));
		((Object*)objects.back())->stretchFitY(128);
		objects.push_back(new Object(WALL, roundTo8(SCREEN_WIDTH/7 * 4), 64, objects.size()));
		
		//Bottom Laundry wall
		objects.push_back(new Object(SIDE_WALL, roundTo8(SCREEN_WIDTH/7*4), 152, objects.size()));
		((Object*)objects.back())->stretchFitY(SCREEN_HEIGHT/3 - 32);

		// Top Bathroom wall
		objects.push_back(new Object(WALL, roundTo8(SCREEN_WIDTH/7*4), SCREEN_HEIGHT/3 + 56, objects.size()));

		// Bottom Bathroom wall
		objects.push_back(new Object(SIDE_WALL, roundTo8(SCREEN_WIDTH/7*4), SCREEN_HEIGHT/3 + 48+32+64, objects.size()));

		// Laundry/Bathroom Wall
		objects.push_back(new Object(WALL, roundTo8(SCREEN_WIDTH/7*4 + 32), SCREEN_HEIGHT/3, objects.size()));
		((Object*)objects.back())->stretchFitX(roundTo8(SCREEN_WIDTH/3-16));

		// MISC Objects
		objects.push_back(new Object(MAGGIE, 180, 180, objects.size()));
		((Object*)objects.back())->image_speed = 4;

	}else{
		// Load from save
		loadSave("sav.nidsav");
		
		// Find Naomi and remove from objects list
		auto it = std::find_if(objects.begin(), objects.end(), [](auto &x){return ((Object*)x)->id == 0xffff;}); 
		naomi = (Naomi*)*it;
		objects.erase(it);

		// Give naomi correct attributes.
		naomi->step_dist = 8;
		naomi->walk_speed = 4;
		naomi->quit = &quit;
		menu->naomi = naomi;

	}


	// FPS Stabilization
	uint32_t start, end, elapsed;

	while(!quit){
		// Get frame starttime for FPS correction.
		start = SDL_GetTicks();
		if(DEBUG) start64 = SDL_GetPerformanceCounter();

		frame++; // Frame counter for animation
		while(SDL_PollEvent(&e)!=0){
			switch(e.type){
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					// Handle input through menu and naomi
					menu->input(e.key.keysym.sym);
					naomi->input(e.key.keysym.sym);
					
					// Catch keys for system use.
					switch(e.key.keysym.sym){
						case SDLK_TAB:
							printf("Frame_no: %u\n", frame);
							printf("Time of last frame (ms): %u\n", elapsed);
							if(DEBUG){
								printf("High-Perf time of last frame (ms): %f\n\n", elapsed64); 
								naomi->objDump();
								if(naomi->heldObject) naomi->heldObject->objDump();
							}else 
								puts("");
							
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

		// Remove Naomi Object from vector	
		auto it = std::find(objects.begin(), objects.end(), naomi);
		objects.erase(it);
			
		// Draw Menu
		menu->draw();

		// Draw Grid
		if(DRAW_GRID) drawGrid(naomi->step_dist);

		// FPS Correction, If Needed.
		end = SDL_GetTicks();
		elapsed = end-start;
	
		// Get more accurate time if debugging
		if(DEBUG){
			end64 = SDL_GetPerformanceCounter();
			elapsed64 = (double)((end64-start64)*1000)/cps;
		}

		// Draw FPS if user requests it
		if(SHOW_FPS){
			char* txt = new char[16];
			if(elapsed or DEBUG)
				if(DEBUG)
					sprintf(txt, "FPS: %.1f", 1000/elapsed64);
				else
					sprintf(txt, "FPS: %d", 1000/elapsed);
			else
				sprintf(txt, "FPS: 1000+");
			renderText({0,0,0,0}, txt, BLACK, fontSml, TXT_LEFT);
			delete[] txt;
		}
	
		// Draw to window
		SDL_RenderPresent(winRenderer);

		
		// Correct for FPS timing

		if(elapsed < TPF){	
			SDL_Delay(TPF - elapsed);
		}
	}
	
	// Clean Up Objects and Resources 	
	delete menu;
	delete naomi;
	if(DEBUG) puts("Cleaned Naomi and Menu");
	for(auto& x: objects){
		delete (Object*)x;
	}
	if(DEBUG) puts("Cleaned Objects");
	SDL_DestroyTexture(background);
	cleanObjectTextures();
	close(); // Clean Up Proceses

	return SUCCESS;
}
