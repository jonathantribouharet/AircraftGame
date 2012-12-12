#include "aircraft.h"
#include "missile.h"
#include "sdl_wrapper.h"
#include "context.h"

static SDL_Surface *aircraft_surface = NULL;
static SDL_Surface *enemy_surface = NULL;

static int AIRCRAFT_HP;
static float AIRCRAFT_SPEED_VERTICAL;
static float AIRCRAFT_SPEED_HORIZONTAL;
static float AIRCRAFT_MISSILE_TYPE;
static float AIRCRAFT_MISSILE_COUNT;
static float AIRCRAFT_MISSILE_DELAY;		

static int ENEMY_HP;
static float ENEMY_SPEED;
static float ENEMY_MISSILE_TYPE;
static float ENEMY_MISSILE_COUNT;
static float ENEMY_MISSILE_DELAY;
static float ENEMY_UPGRADE_DELAY;

int initAircraftContext(){
	aircraft_surface = getSurfaceTransparentFromImage("data/avion.bmp");
	enemy_surface = getSurfaceTransparentFromImage("data/ennemi.bmp");	
	
	if(!aircraft_surface){
		printf("Could not load data/avion.bmp\n");
		return 0;
	}	
	if(!enemy_surface){
		printf("Could not load data/ennemi.bmp\n");
		SDL_FreeSurface(aircraft_surface);
		return 0;
	}	

	AIRCRAFT_HP = 100;
	AIRCRAFT_SPEED_VERTICAL = -7;
	AIRCRAFT_SPEED_HORIZONTAL = 7;
	AIRCRAFT_MISSILE_TYPE = MISSILE_TYPE_2;
	AIRCRAFT_MISSILE_COUNT = 1;
	AIRCRAFT_MISSILE_DELAY = 300;		

	ENEMY_HP = 30;
	ENEMY_SPEED = 1.0;
	ENEMY_MISSILE_TYPE = MISSILE_TYPE_1;
	ENEMY_MISSILE_COUNT = 2;
	ENEMY_MISSILE_DELAY = 1800;
	ENEMY_UPGRADE_DELAY = 20000;

	return 1;
}

void clearAircraftContext(){
	SDL_FreeSurface(aircraft_surface);
	SDL_FreeSurface(enemy_surface);
}

void moveAircraft(struct GameContext *context){
	if(context->keysstate[SDLK_UP])
		context->aircraft.position.y += context->aircraft.speed_y;
	if(context->keysstate[SDLK_DOWN])
		context->aircraft.position.y -= context->aircraft.speed_y;		
	if(context->keysstate[SDLK_LEFT])
		context->aircraft.position.x -= context->aircraft.speed_x;		
	if(context->keysstate[SDLK_RIGHT])
		context->aircraft.position.x += context->aircraft.speed_x;
	
	if(context->aircraft.position.x < 0)
		context->aircraft.position.x = 0;
	if(context->aircraft.position.y < 0)
		context->aircraft.position.y = 0;
	if(context->aircraft.position.x > getScreenWidth() - context->aircraft.surface->w)
		context->aircraft.position.x = getScreenWidth() - context->aircraft.surface->w;
	if(context->aircraft.position.y > getScreenHeight() - context->aircraft.surface->h)
		context->aircraft.position.y = getScreenHeight() - context->aircraft.surface->h;
}

void moveEnemies(Aircraft **enemies, int *size){
	if(*size == 0){
		return;
	}
	
	int i, j = 0, enemies_count = 0;
	Aircraft *new_enemies = NULL;
	
	for(i = 0; i < *size; ++i){
		(*enemies)[i].position.x += (*enemies)[i].speed_x;
		(*enemies)[i].position.y += (*enemies)[i].speed_y;

		if((*enemies)[i].position.y >= 0 && (*enemies)[i].position.y <= getScreenHeight() && (*enemies)[i].position.x >= 0 && (*enemies)[i].position.x <= getScreenWidth()){
			enemies_count++;
		}
	}
	
	if(enemies_count <= 0){
		new_enemies = calloc(1, sizeof(Aircraft));
	}
	else{
		new_enemies = calloc(enemies_count, sizeof(Aircraft));
	}
	
	for(i = 0; i < *size; ++i){
		if((*enemies)[i].position.y >= 0 && (*enemies)[i].position.y <= getScreenHeight() && (*enemies)[i].position.x >= 0 && (*enemies)[i].position.x <= getScreenWidth()){
			new_enemies[j] = (*enemies)[i];
			j++;
		}
	}
	
	*size = enemies_count;
	free(*enemies);
	*enemies = new_enemies;
}

void initAircraft(Aircraft *aircraft){
	aircraft->position.x = 350;
	aircraft->position.y = 500;		
	aircraft->surface = aircraft_surface;	
	aircraft->speed_x = AIRCRAFT_SPEED_HORIZONTAL;		
	aircraft->speed_y = AIRCRAFT_SPEED_VERTICAL;		
	aircraft->last_missile = 0;
	aircraft->missile_type = AIRCRAFT_MISSILE_TYPE;
	aircraft->missile_count = AIRCRAFT_MISSILE_COUNT;
	aircraft->missile_delay = AIRCRAFT_MISSILE_DELAY;
	aircraft->hp = AIRCRAFT_HP;	
}

void createEnemy(Aircraft **aircrafts, int *size){
	static int last_upgrade = 0;

	if((SDL_GetTicks() - last_upgrade) > ENEMY_UPGRADE_DELAY){
		last_upgrade = SDL_GetTicks();
		ENEMY_HP += 10;
	}
	
	*aircrafts = realloc(*aircrafts, (*size + 1) * sizeof(Aircraft));
	
	(*aircrafts)[*size].surface = enemy_surface;
	(*aircrafts)[*size].hp = ENEMY_HP;
	(*aircrafts)[*size].last_missile = 0;
	(*aircrafts)[*size].missile_type = ENEMY_MISSILE_TYPE;
	(*aircrafts)[*size].missile_count = ENEMY_MISSILE_COUNT;
	(*aircrafts)[*size].missile_delay = ENEMY_MISSILE_DELAY;
	(*aircrafts)[*size].speed_x = 0;
	(*aircrafts)[*size].speed_y = ENEMY_SPEED;
	(*aircrafts)[*size].position.y = 0;
	(*aircrafts)[*size].position.x = (rand() % getScreenWidth()) - (*aircrafts)[*size].surface->w;
	
	(*size)++;		
}

void destroyAircraft(Aircraft **aircrafts, int *size, int index){
	int i, j = 0;
	
	Aircraft *new_aircrafts = NULL;
	if(*size <= 1){
		new_aircrafts = calloc(1, sizeof(Aircraft));
	}
	else{
		new_aircrafts = calloc(*size - 1, sizeof(Aircraft));
	}
	
	for(i = 0; i < *size; ++i){
		if(i != index){
			new_aircrafts[j] = (*aircrafts)[i];
			j++;
		}
	}
	
	(*size)--;
	free(*aircrafts);
	*aircrafts = new_aircrafts;	
}

void destroyAllAircrafts(Aircraft **aircrafts, int *size){
	free(*aircrafts);
	(*size) = 0;

	*aircrafts = calloc(1, sizeof(Aircraft));
}
