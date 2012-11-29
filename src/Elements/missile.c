#include "missile.h"
#include "sdl_wrapper.h"

static SDL_Surface *missile_surface = NULL;

int initMissileContext(){
	missile_surface = getSurfaceTransparentFromImage("data/missile.bmp");
	
	if(!missile_surface){
		printf("Could not load data/missile.bmp\n");
		return 0;
	}

	return 1;
}

void clearMissileContext(){
	SDL_FreeSurface(missile_surface);	
}

void moveMissiles(Missile **missiles, int *missiles_size){
	if(*missiles_size == 0){
		return;
	}
	
	int i, j = 0, missiles_count = 0;
	Missile *new_missiles = NULL;
	
	for(i = 0; i < *missiles_size; ++i){
		(*missiles)[i].position.x += (*missiles)[i].speed_x;
		(*missiles)[i].position.y += (*missiles)[i].speed_y;
		if((*missiles)[i].position.y >= 0 && (*missiles)[i].position.y <= getScreenHeight() && (*missiles)[i].position.x >= 0 && (*missiles)[i].position.x <= getScreenWidth()){
			missiles_count++;
		}
	}
	
	if(missiles_count <= 0){
		new_missiles = calloc(1, sizeof(Missile));
	}
	else{
		new_missiles = calloc(missiles_count, sizeof(Missile));
	}
	
	for(i = 0; i < *missiles_size; ++i){
		if((*missiles)[i].position.y >= 0 && (*missiles)[i].position.y <= getScreenHeight() && (*missiles)[i].position.x >= 0 && (*missiles)[i].position.x <= getScreenWidth()){
			new_missiles[j] = (*missiles)[i];
			j++;
		}
	}
	
	free(*missiles);
	*missiles = new_missiles;
	*missiles_size = missiles_count;
}

static void createMissileWithType(Missile *missile, const Aircraft *aircraft){
	missile->surface = missile_surface;
	missile->speed_x = 0;
	
	switch(aircraft->missile_type){
		case MISSILE_TYPE_1:
			missile->damage = 3;
			missile->speed_y = 3;
			break;
		case MISSILE_TYPE_2:
			missile->damage = 10;
			missile->speed_y = 10;
			break;
	}
	
	if(aircraft->speed_y < 0){
		missile->speed_y *= -1;
		missile->position.y = aircraft->position.y;
	}
	else{
		missile->position.y = aircraft->position.y + aircraft->surface->h;
	}
}

void createMissile(Missile **missiles, int *size, Aircraft *aircraft){
	int i;
	
	if(SDL_GetTicks() - aircraft->last_missile < aircraft->missile_delay){
		return;
	}
	
	aircraft->last_missile = SDL_GetTicks();

	*missiles = realloc(*missiles, ((*size) + aircraft->missile_count) * sizeof(Missile));
	
	if(aircraft->missile_count % 2 == 0){
			for(i = 0; i < aircraft->missile_count; i += 2){
				createMissileWithType(&(*missiles)[*size + i], aircraft);
				(*missiles)[*size + i].position.x = aircraft->position.x + (aircraft->surface->w / 2) + ((i / 2)* (*missiles)[*size].surface->w);
				
				createMissileWithType(&(*missiles)[*size + i + 1], aircraft);
				(*missiles)[*size + i + 1].position.x = aircraft->position.x + (aircraft->surface->w / 2) - (((i / 2)+ 1) * (*missiles)[*size].surface->w);	
			}	
	}
	else{
			createMissileWithType(&(*missiles)[*size], aircraft);
			(*missiles)[*size].position.x = aircraft->position.x + (aircraft->surface->w / 2) - ((*missiles)[*size].surface->w / 2);
		
			for(i = 1; i < aircraft->missile_count; i += 2){
				createMissileWithType(&(*missiles)[*size + i], aircraft);
				(*missiles)[*size + i].position.x = aircraft->position.x + (aircraft->surface->w / 2) + ((*missiles)[*size].surface->w / 2)  + ((i / 2)* (*missiles)[*size].surface->w);
				
				createMissileWithType(&(*missiles)[*size + i + 1], aircraft);
				(*missiles)[*size + i + 1].position.x = aircraft->position.x + (aircraft->surface->w / 2) - ((*missiles)[*size].surface->w / 2) - (((i / 2)+ 1) * (*missiles)[*size].surface->w);	
			}		
	}
	
	(*size) += aircraft->missile_count;
}

void destroyMissile(Missile **missiles, int *missiles_size, int index){
	int i, j = 0;

	Missile *new_missiles = NULL;
	if(*missiles_size <= 1){
		new_missiles = calloc(1, sizeof(Missile));
	}
	else{
		new_missiles = calloc(*missiles_size - 1, sizeof(Missile));	
	}
	
	for(i = 0; i < *missiles_size; ++i){
		if(i != index){
			new_missiles[j] = (*missiles)[i];
			j++;
		}
	}
	
	free(*missiles);
	*missiles = new_missiles;		
	(*missiles_size)--;
}
