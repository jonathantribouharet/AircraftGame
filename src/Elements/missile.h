#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "position.h"
#include "aircraft.h"

enum MISSILE_TYPE {MISSILE_TYPE_1, MISSILE_TYPE_2};

typedef struct{
	Position position;
	
	float speed_x;
	float speed_y;
	int damage;
	
	SDL_Surface *surface;
}Missile;

void initMissileContext();
void clearMissileContext();

void moveMissiles(Missile **, int *);
void createMissile(Missile **, int *, Aircraft *);
void destroyMissile(Missile **, int *, int);

#endif
