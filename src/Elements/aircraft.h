#ifndef _AIRCRAFT_H_
#define _AIRCRAFT_H_

#include "SDL.h"

#include "position.h"

struct GameContext;

typedef struct{
	Position position;

	float speed_x;
	float speed_y;	
	
	SDL_Surface *surface;
	
	int hp;
	
	int last_missile;
	
	int missile_type;
	int missile_count;
	float missile_delay;
}Aircraft;

int initAircraftContext();
void clearAircraftContext();

void moveAircraft(struct GameContext *context);

void moveEnemies(Aircraft **, int *);
void initAircraft(Aircraft *);
void createEnemy(Aircraft **, int *);
void destroyAircraft(Aircraft **, int *, int);
void destroyAllAircrafts(Aircraft **, int *);

#endif
