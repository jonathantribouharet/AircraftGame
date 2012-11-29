#include "SDL_main.h"
#include "sdl_wrapper.h"
#include "menu.h"

 int SDL_main(int argc, char *argv[]){
	SDLWrapperInit();
	menu();
	SDLWrapperQuit();
	
	return EXIT_SUCCESS;
}
