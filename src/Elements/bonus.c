#include "bonus.h"
#include "sdl_wrapper.h"
#include <stdlib.h>

static SDL_Surface *bonus_health_surface = NULL;
static SDL_Surface *bonus_weapon_upgrade_surface = NULL;
static SDL_Surface *bonus_bomb_surface = NULL;

void initBonusContext(){
	bonus_health_surface = getSurfaceTransparentFromImage("data/bonus_health_surface.jpg");
	bonus_weapon_upgrade_surface = getSurfaceTransparentFromImage("data/bonus_weapon_upgrade_surface.jpg");
	bonus_bomb_surface = getSurfaceTransparentFromImage("data/bonus_bomb.png");
	
	if(!bonus_health_surface){
		printf("Could not load data/bonus_health_surface.jpg\n");
	}
	if(!bonus_weapon_upgrade_surface){
		printf("Could not load data/bonus_weapon_upgrade_surface.jpg\n");	
	}
}

void clearBonusContext(){
	SDL_FreeSurface(bonus_health_surface);	
	SDL_FreeSurface(bonus_weapon_upgrade_surface);	
	SDL_FreeSurface(bonus_bomb_surface);	
}

void moveBonus(Bonus **bonuses, int *size){
	if(*size == 0){
		return;
	}
	
	int i, j = 0, bonuses_count = 0;
	Bonus *new_bonuses = NULL;
	
	for(i = 0; i < *size; ++i){
		(*bonuses)[i].position.x += (*bonuses)[i].speed_x;
		(*bonuses)[i].position.y += (*bonuses)[i].speed_y;
		if((*bonuses)[i].position.y >= 0 && (*bonuses)[i].position.y <= getScreenHeight() && (*bonuses)[i].position.x >= 0 && (*bonuses)[i].position.x <= getScreenWidth()){
			bonuses_count++;
		}
	}
	
	if(bonuses_count <= 0){
		new_bonuses = calloc(1, sizeof(Bonus));
	}
	else{
		new_bonuses = calloc(bonuses_count, sizeof(Bonus));
	}
	
	for(i = 0; i < *size; ++i){
		if((*bonuses)[i].position.y >= 0 && (*bonuses)[i].position.y <= getScreenHeight() && (*bonuses)[i].position.x >= 0 && (*bonuses)[i].position.x <= getScreenWidth()){
			new_bonuses[j] = (*bonuses)[i];
			j++;
		}
	}
	
	free(*bonuses);
	*bonuses = new_bonuses;	
	*size = bonuses_count;
}

void createRandomBonus(Bonus **bonuses, int *size){	
	*bonuses = realloc(*bonuses, (*size + 1) * sizeof(Bonus));
	
	if(random() % 2){
		(*bonuses)[*size].position.x = 0;
		(*bonuses)[*size].speed_x = 1;
	}
	else{
		(*bonuses)[*size].position.x = getScreenWidth();
		(*bonuses)[*size].speed_x = -1;
	}
	
	if(random() % 2){
		(*bonuses)[*size].position.y = 0;
		(*bonuses)[*size].speed_y = 1.2;
	}
	else{
		(*bonuses)[*size].position.y = getScreenHeight();
		(*bonuses)[*size].speed_y = -1.2;		
	}	
	
	(*bonuses)[*size].type = random() % 3; //3 car il n'existe que 2 bonus
	
	switch((*bonuses)[*size].type){
		case BONUS_HEALTH:
			(*bonuses)[*size].surface = bonus_health_surface;
			break;
		case BONUS_WEAPON_UPGRADE:
			(*bonuses)[*size].surface = bonus_weapon_upgrade_surface;
			break;
		case BONUS_BOMB:
			(*bonuses)[*size].surface = bonus_bomb_surface;
			break;		
	}
	
	(*size)++;		
}

void destroyBonus(Bonus **bonuses, int *size, int index){
	int i, j = 0;
	
	Bonus *new_bonuses = NULL;
	if(*size <= 1){
		new_bonuses = calloc(1, sizeof(Bonus));
	}
	else{
		new_bonuses = calloc(*size - 1, sizeof(Bonus));
	}
	
	for(i = 0; i < *size; ++i){
		if(i != index){
			new_bonuses[j] = (*bonuses)[i];
			j++;
		}
	}
	
	(*size)--;
	free(*bonuses);
	*bonuses = new_bonuses;	
}
