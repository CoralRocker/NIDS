#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <vector>
#include <algorithm>

#include "gamelib.hpp"
#include "menu.hpp"
#include "naomi.hpp"
#include "object.hpp"
#include "save.hpp"

const char *obj_names[OBJ_MAX] = {
	"Table", "Modular Table", "Stool",
	"Cabinet", "Tall Cabinet",
	"Desk", "Modular Desk", "Desk Chair",
	"Mirror", "Standing Mirror", 
	"Bonsai", "Small Plants", "Medium Plants", "Large Plants", "Extra-Large Plants",
	"Toilet", "Misc. Bathroom", "Bathtub", "Sink Vanity", "Sink Cabinet",
	"Bed", "TV",
	
	"Wall", "Side Wall",
	"Naomi",
	"Maggie"
};

const char *opt_names[MENU_MAX] = {
	"DEBUG MODE: ",
	"FPS: ",
	"SHOW FPS: ",
	"GRID: ",
	"Toggle Fullscreen"
};

Menu::Menu(){
	pause = &game_pause;
	visible = false;

	SDL_Surface* txtBoxSrfc = IMG_Load("images/spr_intro_message_0.png");
	textBox = SDL_CreateTextureFromSurface(winRenderer, txtBoxSrfc);
	SDL_FreeSurface(txtBoxSrfc);

	txtRect = {0, 0, 256, 128};

	sel.push_back(0);
}
	
Menu::~Menu(){
	SDL_DestroyTexture(textBox);
	if(DEBUG) puts("destroyed Menu");	
}

// Stretch the menu text box;
SDL_Rect Menu::stretch(float x, float y){
	SDL_Rect tBox = txtRect;
	tBox.w = (uint16_t) nearbyint(tBox.w * x);
	tBox.h = (uint16_t) nearbyint(tBox.h * y);
	return tBox;
}

SDL_Rect Menu::stretch(float factor){
	return stretch(factor, factor);
}

// The input method does the legwork of controlling the menu options and taking user input.
// It's not self-documenting code by any means but its relatively clear to me right now.
// It'll bite me in the ass later, but that's a later me problem. Ha. Future me doesn't know
// what's gonna hit 'im.
void Menu::input(SDL_Keycode sym){
	switch(sym){
		case SDLK_p:
			*pause = 1-(*pause);
			while(sel.size() > 1){sel.pop_back();}
			sel[0] = 0;

			break;
		case SDLK_d:
			if(!game_pause) break;
			DEBUG = 1-DEBUG;
			break;
		case SDLK_RIGHT:
			if(sel.size() == 1){ 
				sel[0]++;
				if(sel[0] >= 4) sel[0] = 0;
			}
			break;
		case SDLK_LEFT:
			if(sel.size() == 1){ 
				sel[0]--;
				if(sel[0] > 4) sel[0] = 3;
			}
			break;
		case SDLK_RETURN:
			switch(sel.size()){
				case 1:
					sel.push_back(0);
					break;
				case 2:
					switch(sel[0]){
						case 0:
							naomi->objtype = (AVAIL_OBJECTS) sel[1]; // Set correct held object type;
							sel.pop_back();
							sel[0] = 0;
							*pause = 1-(*pause);
							break;
						case 1:
							switch(sel[1]){
								case TOGGLE_DEBUG:
									DEBUG = 1-DEBUG;
									break;
								case TOGGLE_FPS:
									if(FPS == 60)
										FPS = 30;
									else
										FPS = 60;
									TPF = 1000.f/FPS;
									break;
								case TOGGLE_SHOW_FPS:
									SHOW_FPS = 1-SHOW_FPS;
									break;
								case TOGGLE_GRID:
									DRAW_GRID = 1-DRAW_GRID;
									break;
								case TOGGLE_FULLSCREEN:
									FULLSCREEN = 1-FULLSCREEN;
									if(FULLSCREEN)
										SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
									else
										SDL_SetWindowFullscreen(win, 0);
									break;
							}
							break;
						case 2:
							naomi->objcolmod = (ColorCodes)sel[1];
							break;
						case 3:
							// Exit and Save Game
							*naomi->quit = true;
							objects.push_back(naomi);
							save("sav.nidsav");
							objects.erase(std::find(objects.begin(), objects.end(), naomi));
							break;
					}
					break;
			}
			break;
		case SDLK_ESCAPE:
			if(sel.size() == 1 && *pause){sel[0]=0;*pause = 1 - (*pause);break;} // Quit the menu if the time is right.
			while(sel.size() > 1){
				sel.pop_back();
			}
			sel[0] = 0;
			break;
		case SDLK_UP:
			if(!(sel.size() == 2)) break;
			switch(sel[0]){
				case 0:{
					if(++sel[1] == OBJ_MAX) sel[1] = 0;
					bool exit = false;
					// Skip over non placeable objects
					while(!exit){
						switch(sel[1]){
							case NAOMI:
							case MAGGIE:
							case WALL:
							case SIDE_WALL:
								if(++sel[1] == OBJ_MAX) sel[1] = 0;
								break;
							default:
								exit = true;
								break;
						}
					}
					break;
					}
				case 1:
					if(++sel[1] == MENU_MAX) sel[1] = MENU_MAX - 1;
					break;
				case 2: 
					if(++sel[1] == COLOR_MAX) sel[1] = 0;
					break;

			}
			break;
		case SDLK_DOWN:
			if(!(sel.size() == 2)) break; // Only used for submenus
			switch(sel[0]){
				case 0:{
					if(sel[1]-- == 0) sel[1] = OBJ_MAX - 1;
					bool exit = false;

					// Skip over non-placeable objects
					while(!exit){
						switch(sel[1]){
							case NAOMI:
							case MAGGIE:
							case WALL:
							case SIDE_WALL:
								if(sel[1]-- == 0) sel[1] = OBJ_MAX - 1;
								break;
							default:
								exit = true;
								break;
						}
					}
					
					break;
				       }
				case 1:
					if(sel[1]-- == 0) sel[1] = 0;
					break;
				case 2:
					if(sel[1]-- == 0) sel[1] = COLOR_MAX - 1;
					break;
			}
			break;
	}
}

void Menu::draw(){
	if(!game_pause) return;

	// Initial Menu Box
	SDL_Rect tBox;
	tBox = stretch(SCREEN_WIDTH/256.f, 0.75);
	tBox.y = SCREEN_HEIGHT - 96;
	SDL_RenderCopy(winRenderer, textBox, NULL, &tBox);

	TTF_Font* fnt = fontSml; // Font variable used to draw text

	// Draw basal menu options
	for(int i = 0; i < 4; i++){
		switch(i){
			case 0:
				if(i == sel[0]) fnt = fontMed; else fnt = fontSml;
				renderText({SCREEN_WIDTH/5, SCREEN_HEIGHT - 48, 0,0}, "OBJECTS", PRED, fnt, TXT_MIDDLE);
				break;
			case 1:
				if(i == sel[0]) fnt = fontMed; else fnt = fontSml;
				renderText({SCREEN_WIDTH/5 * 2, SCREEN_HEIGHT - 48, 0,0}, "OPTIONS", ORANGE, fnt, TXT_MIDDLE);
				break;
			case 2:
				if(i == sel[0]) fnt = fontMed; else fnt = fontSml;
				renderText({SCREEN_WIDTH/5 * 3, SCREEN_HEIGHT - 48, 0,0}, "COLORS", LBLUE, fnt, TXT_MIDDLE);
				break;
			case 3:
				if(i == sel[0]) fnt = fontMed; else fnt = fontSml;
				renderText({SCREEN_WIDTH/5 * 4, SCREEN_HEIGHT - 48, 0,0}, "SAVE & EXIT", YELLOW, fnt, TXT_MIDDLE);
				break;
		}
	}

	// Draw submenus if necessary
	if(sel.size() == 2){
		switch(sel[0]){
			// Object Selection Wheel Drawing
			case 0: {
				tBox = stretch(0.75, 2);
				tBox.x = SCREEN_WIDTH/5 - tBox.w/2;
				tBox.y = SCREEN_HEIGHT - 96 - tBox.h;
				SDL_RenderCopy(winRenderer, textBox, NULL, &tBox);
				
				int indx = sel[1] - 2;
				if(indx < 0) indx = OBJ_MAX + sel[1] - 6; // Correct for indexes less than 2;
				for(int i = 0; i < 5; i++){
					fnt = (i == 2) ? fontMed : fontSml;
					renderText({SCREEN_WIDTH/5, SCREEN_HEIGHT - 96 - (tBox.h/6 * (i+1)),0,0}, obj_names[indx], BLACK, fnt, TXT_MIDDLE);
					indx++;
					if(indx == OBJ_MAX-4) indx = 0;
				}
				break;
				}
			
			// Game Option Screen
			case 1:
				tBox = stretch(1, (MENU_MAX*0.5));
				tBox.x = SCREEN_WIDTH/5 * 2 - tBox.w/2;
				tBox.y = SCREEN_HEIGHT - 96 - tBox.h;
				SDL_RenderCopy(winRenderer, textBox, NULL, &tBox);
				for(int i = 0; i < MENU_MAX; i++){
					fnt = (i == sel[1]) ? fontMed : fontSml;

					char* tmpstr = new char[64];
					switch(i){
						case TOGGLE_DEBUG:
							sprintf(tmpstr, "%s%s", opt_names[i], DEBUG ? "On" : "Off");
							break;
						case TOGGLE_FPS:
							sprintf(tmpstr, "%s%d", opt_names[i], FPS);
							break;
						case TOGGLE_SHOW_FPS:
							sprintf(tmpstr, "%s%s", opt_names[i], SHOW_FPS ? "On" : "Off");
							break;
						case TOGGLE_GRID:
							sprintf(tmpstr, "%s%s", opt_names[i], DRAW_GRID ? "On" : "Off");
							break;
						case TOGGLE_FULLSCREEN:
							sprintf(tmpstr, "%s", opt_names[i]);
							break;
					}
					renderText({SCREEN_WIDTH/5 * 2, SCREEN_HEIGHT - 96 - (tBox.h/(MENU_MAX+1) * (i+1)), 0,0}, tmpstr, BLACK, fnt, TXT_MIDDLE);
					delete[] tmpstr;
				}
				break;

			// Color Wheel Drawing
			case 2: {
				tBox = stretch(0.75, 2);
				tBox.x = SCREEN_WIDTH/5 * 3 - tBox.w/2;
				tBox.y = SCREEN_HEIGHT - 96 - tBox.h;
				SDL_RenderCopy(winRenderer, textBox, NULL, &tBox);
				
				int indx = sel[1] - 2;
				if(indx < 0) indx = COLOR_MAX + sel[1] - 2; // Correct for indexes less than 2;
				for(int i = 0; i < 5; i++){
					fnt = (i == 2) ? fontMed : fontSml;
					renderText({SCREEN_WIDTH/5 * 3, SCREEN_HEIGHT - 96 - (tBox.h/6 * (i+1)),0,0}, colorNames[indx], (ColorCodes)indx, fnt, TXT_MIDDLE);
					indx++;
					if(indx == COLOR_MAX) indx = 0;
				}
				break;
				}

			// Save and Exit Confirmation
			case 3:
				tBox = stretch(0.75, 0.5);
				tBox.x = SCREEN_WIDTH/5 * 4 - tBox.w/2;
				tBox.y = SCREEN_HEIGHT - 96 - tBox.h;
				SDL_RenderCopy(winRenderer, textBox, NULL, &tBox);
				
				renderText({SCREEN_WIDTH/5 * 4, SCREEN_HEIGHT - 96 - tBox.h/2, 0,0}, "EXIT?", BLACK, fontMed, TXT_MIDDLE);
				break;
		}
	}
}
