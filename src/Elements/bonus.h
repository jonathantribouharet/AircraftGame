#ifndef _BONUS_H_
#define _BONUS_H_

#include "SDL.h"

#include "position.h"

enum BONUS_TYPE {BONUS_HEALTH, BONUS_WEAPON_UPGRADE, BONUS_BOMB};

typedef struct{
	Position position;

	float speed_x;
	float speed_y;	
	
	SDL_Surface *surface;
	
	int type;
}Bonus;

int initBonusContext();
void clearBonusContext();

void moveBonus(Bonus **, int *);
void createRandomBonus(Bonus **, int *);
void destroyBonus(Bonus **, int *, int);

#endif
