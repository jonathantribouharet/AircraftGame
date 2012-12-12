#include "context.h"
#include "sdl_wrapper.h"
#include <time.h>

static const float ENEMY_DELAY = 750;
static const float BONUS_DELAY = 8000;

static void refreshAircraftScore(GameContext *context);
static void processCollisions(GameContext *context);

static int processCollisionsEnemies(GameContext *context);
static int processCollisionsEnemieMissiles(GameContext *context);
static int processCollisionsBonuses(GameContext *context);

int initContext(GameContext *context){
	if(!(initAircraftContext() && initBonusContext() && initMissileContext())){
		return 0;
	}

	context->active = 1;
	context->keysstate = SDL_GetKeyState(NULL);
		
	context->score = 0;

	context->enemies = calloc(1, sizeof(Aircraft));
	context->enemies_size = 0;

	context->aircraft_missiles = calloc(1, sizeof(Missile));
	context->aircraft_missiles_size = 0;

	context->enemy_missiles = calloc(1, sizeof(Missile));
	context->enemy_missiles_size = 0;

	context->bonuses = calloc(1, sizeof(Bonus));
	context->bonuses_size = 0;
	
	context->last_enemy = 0;
	context->last_bonus = 0;
	
	context->label_hp.surface = getSurfaceFromText("");
	context->label_score.surface = getSurfaceFromText("");
	refreshAircraftScore(context);
	
	initAircraft(&context->aircraft);

	return 1;
}

void closeContext(GameContext *context){
	SDL_FreeSurface(context->label_hp.surface);
	SDL_FreeSurface(context->label_score.surface);

	free(context->enemies);
	free(context->aircraft_missiles);
	free(context->enemy_missiles);
	free(context->bonuses);
	
	clearAircraftContext();
	clearBonusContext();
	clearMissileContext();
}

void processEvent(GameContext *context){	
	int i;
	
	//Update keysstate
	// SDL_PumpEvents(); //call by SDL_PollEvent
	
	if(context->keysstate[SDLK_ESCAPE]){
		context->active = 0;
	}

	createMissile(&context->aircraft_missiles, &context->aircraft_missiles_size, &context->aircraft);
	
	if(SDL_GetTicks() - context->last_enemy >= ENEMY_DELAY){
		context->last_enemy = SDL_GetTicks();
		createEnemy(&context->enemies, &context->enemies_size);	
	}
	
	if(SDL_GetTicks() - context->last_bonus >= BONUS_DELAY){
		context->last_bonus = SDL_GetTicks();
		createRandomBonus(&context->bonuses, &context->bonuses_size);
	}
	
	moveAircraft(context);
	moveBonus(&context->bonuses, &context->bonuses_size);
	
	moveMissiles(&context->aircraft_missiles, &context->aircraft_missiles_size);
	moveMissiles(&context->enemy_missiles, &context->enemy_missiles_size);
	
	moveEnemies(&context->enemies, &context->enemies_size);
	
	for(i = 0; i < context->enemies_size; i++){
		createMissile(&context->enemy_missiles, &context->enemy_missiles_size, &context->enemies[i]);
	}
		
	processCollisions(context);
	refreshAircraftScore(context);
}

static void refreshAircraftScore(GameContext *context){
	char aircraft_hp[20];
	char aircraft_score[20];
	
	static int last_aircraft_hp = -1;
	static int last_aircraft_score = -1;
	
	if(last_aircraft_hp != context->aircraft.hp){
		last_aircraft_hp = context->aircraft.hp;
		sprintf(aircraft_hp, "%d HP", context->aircraft.hp);

		SDL_FreeSurface(context->label_hp.surface);
		context->label_hp.surface = getSurfaceFromText(aircraft_hp);

		context->label_hp.position.x = getScreenWidth() - context->label_hp.surface->w - 15;
		context->label_hp.position.y = getScreenHeight() - context->label_hp.surface->h - 5;
	}	

	if(last_aircraft_score != context->score){
		last_aircraft_score = context->score;
		sprintf(aircraft_score, "%d", context->score);

		SDL_FreeSurface(context->label_score.surface);
		context->label_score.surface = getSurfaceFromText(aircraft_score);

		context->label_score.position.x = 15;
		context->label_score.position.y = getScreenHeight() - context->label_score.surface->h - 5;
	}		
}

static void processCollisions(GameContext *context){
	processCollisionsEnemies(context) && processCollisionsEnemieMissiles(context) && processCollisionsBonuses(context);
}

static int processCollisionsEnemies(GameContext *context){
	for(int i = 0; i < context->enemies_size; ++i){
		if(isOverlap(&context->aircraft.position, context->aircraft.surface, &context->enemies[i].position, context->enemies[i].surface)){
			context->score++;
			context->aircraft.hp -= context->enemies[i].hp / 5;
			destroyAircraft(&context->enemies, &context->enemies_size, i);
			i--;
			
			if(context->aircraft.hp <= 0){
				context->aircraft.hp = 0;
				context->active = 0;
				return 0;
			}

			continue;
		}
		
		for(int j = 0; j < context->aircraft_missiles_size; ++j){				
			if(isOverlap(&context->aircraft_missiles[j].position, context->aircraft_missiles[j].surface, &context->enemies[i].position, context->enemies[i].surface)){
				context->enemies[i].hp -= context->aircraft_missiles[j].damage;
				destroyMissile(&context->aircraft_missiles, &context->aircraft_missiles_size, j);

				if(context->enemies[i].hp <= 0){
					context->score++;
					destroyAircraft(&context->enemies, &context->enemies_size, i);
					i--;
				}
				break;
			}
		}
	}	

	return 1;
}

static int processCollisionsEnemieMissiles(GameContext *context){
	for(int i = 0; i < context->enemy_missiles_size; ++i){
		if(isOverlap(&context->enemy_missiles[i].position, context->enemy_missiles[i].surface, &context->aircraft.position, context->aircraft.surface)){
			context->aircraft.hp -= context->enemy_missiles[i].damage;
			destroyMissile(&context->enemy_missiles, &context->enemy_missiles_size, i);
			
			if(context->aircraft.hp <= 0){
				context->aircraft.hp = 0;
				context->active = 0;
				return 0;
			}
			i--;
			break;
		}
	}

	return 1;
}

static int processCollisionsBonuses(GameContext *context){
	for(int i = 0; i < context->bonuses_size; ++i){
		if(isOverlap(&context->bonuses[i].position, context->bonuses[i].surface, &context->aircraft.position, context->aircraft.surface)){
			switch(context->bonuses[i].type){
				case BONUS_HEALTH:
					context->aircraft.hp += 100;
					if(context->aircraft.hp > 200){
						context->aircraft.hp = 200;
					}
					break;
				case BONUS_WEAPON_UPGRADE:
					context->aircraft.missile_count++;
					if(context->aircraft.missile_count > 3){
						context->aircraft.missile_count = 3;
						context->aircraft.missile_delay -= 30;
						if(context->aircraft.missile_delay < 75){
							context->aircraft.missile_delay = 75;
						}
					}
					break;
				case BONUS_BOMB:
					context->score += context->enemies_size;
					destroyAllAircrafts(&context->enemies, &context->enemies_size);
					break;
			}
			destroyBonus(&context->bonuses, &context->bonuses_size, i);
			i--;
		}
	}

	return 1;
}
