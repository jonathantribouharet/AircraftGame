#include "sdl_wrapper.h"
#include "context.h"

#define TICK_INTERVAL 15 // 60 FPS

static Uint32 next_synchronisation;

static void displayGameOver();
static void synchronize();
static void displaySurfaces(GameContext *context);

int play(){
	SDL_Event event;
    GameContext context;
    
    if(!initContext(&context)){
    	return 0;
    }
    
    while(context.active){		
    	if(SDL_PollEvent(&event) == 1 && event.type == SDL_QUIT){
    		closeContext(&context);
			return 0;	
		}

        processEvent(&context);
        displaySurfaces(&context);
        synchronize();
    }
	
    closeContext(&context);

    displayGameOver();

	do{
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT){
			return 0;
		}
	}while(event.type != SDL_KEYDOWN || event.key.keysym.sym != SDLK_RETURN);

	return 1;
}


static void displayGameOver(){
	Label label;
	label.surface = getSurfaceFromText("Game Over!");
	
	label.position.x = (getScreenWidth() - label.surface->w) / 2;
	label.position.y = (getScreenHeight() - label.surface->h) / 2;
	
	SDL_BlitSurface(label.surface, NULL, getSurfaceScreen(), &label.position);
	SDL_Flip(getSurfaceScreen());
	SDL_FreeSurface(label.surface);
}

static void synchronize(){
	Uint32 now = SDL_GetTicks();
	Uint32 offset = 0;

	if(next_synchronisation > now){
		offset = next_synchronisation - now;
	}

	SDL_Delay(offset);
	next_synchronisation += TICK_INTERVAL;
}

static void displaySurfaces(GameContext *context){
	int i = 0;
		
	clearScreen();
	
	for(i = 0; i < context->aircraft_missiles_size; ++i){
		SDL_BlitSurface(context->aircraft_missiles[i].surface, NULL, getSurfaceScreen(), getSDLRectFromPosition(&context->aircraft_missiles[i].position));
	}

	for(i = 0; i < context->enemy_missiles_size; ++i){
		SDL_BlitSurface(context->enemy_missiles[i].surface, NULL, getSurfaceScreen(), getSDLRectFromPosition(&context->enemy_missiles[i].position));
	}		
	
	for(i = 0; i < context->enemies_size; ++i){
		SDL_BlitSurface(context->enemies[i].surface, NULL, getSurfaceScreen(), getSDLRectFromPosition(&context->enemies[i].position));
	}

	for(i = 0; i < context->bonuses_size; ++i){
		SDL_BlitSurface(context->bonuses[i].surface, NULL, getSurfaceScreen(), getSDLRectFromPosition(&context->bonuses[i].position));
	}	
	
	SDL_BlitSurface(context->aircraft.surface, NULL, getSurfaceScreen(), getSDLRectFromPosition(&context->aircraft.position));
	
	SDL_BlitSurface(context->label_hp.surface, NULL, getSurfaceScreen(), &context->label_hp.position);
	SDL_BlitSurface(context->label_score.surface, NULL, getSurfaceScreen(), &context->label_score.position);
	
	SDL_Flip(getSurfaceScreen());	
}
