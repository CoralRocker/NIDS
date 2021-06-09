#include "save.hpp"
#include "gamelib.hpp"
#include "object.hpp"
#include "naomi.hpp"

#include <stdio.h>

void loadSave(const char* savefile){
	FILE *sf = fopen(savefile, "rb");

	int numObjs;
	fread(&numObjs, sizeof(int), 1, sf);

	for(int i = 0; i < numObjs; i++){
		SAVE_OBJ obj;
		fread(&obj, sizeof(SAVE_OBJ), 1, sf);
		if(obj.type == NAOMI){
			puts("Found Naomi");
			Naomi* n = new Naomi();
			n->image_side = obj.image_side;
			n->image_index = obj.image_index;
			n->image_speed = obj.image_speed;
			n->depth = obj.depth;
			n->direction = obj.direction;
			n->id = obj.id;
			n->posRect = obj.posRect;
			n->clip = obj.clip;
			n->bBox = obj.bBox;
			n->solid = obj.solid;
			n->visible = obj.visible;
			n->moving = obj.moving;

			objects.push_back(n);

		}else{
			Object* o = new Object(obj.type);
			o->image_side = obj.image_side;
			o->image_index = obj.image_index;
			o->image_speed = obj.image_speed;
			o->depth = obj.depth;
			o->direction = obj.direction;
			o->id = obj.id;
			o->posRect = obj.posRect;
			o->clip = obj.clip;
			o->bBox = obj.bBox;
			o->solid = obj.solid;
			o->visible = obj.visible;
			o->moving = obj.moving;

			objects.push_back(o);
		}
	}
}

void save(const char* savefile){
	FILE *sf = fopen(savefile, "wb");
	
	int numObjs = objects.size();

	fwrite(&numObjs, sizeof(int), 1, sf);

	for(auto& x: objects){
		SAVE_OBJ obj = {
			((Object*)x)->image_side, ((Object*)x)->image_index, ((Object*)x)->image_speed,
			((Object*)x)->depth, ((Object*)x)->direction, ((Object*)x)->id,
			((Object*)x)->posRect, ((Object*)x)->clip, ((Object*)x)->bBox,
			((Object*)x)->type,
			((Object*)x)->solid, ((Object*)x)->visible, ((Object*)x)->moving
		};
		fwrite(&obj, sizeof(SAVE_OBJ), 1, sf);
	}

	fclose(sf);
}
