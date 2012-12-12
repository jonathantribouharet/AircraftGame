#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "SDL.h"

#include "position.h"
#include "label.h"
#include "aircraft.h"
#include "missile.h"
#include "bonus.h"

typedef struct{
	int active;
	Uint8 *keysstate;
		
	Label label_hp;
	Label label_score;

	Aircraft aircraft;
	int score;

	Aircraft *enemies;
	int enemies_size;

	Missile *aircraft_missiles;
	int aircraft_missiles_size;

	Missile *enemy_missiles;
	int enemy_missiles_size;

	Bonus *bonuses;
	int bonuses_size;
	
	int last_enemy;
	int last_bonus;
}GameContext;

int initContext(GameContext *);
void closeContext(GameContext *);
void processEvent(GameContext *);

#endif
