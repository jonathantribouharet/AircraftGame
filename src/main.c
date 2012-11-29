#include "sdl_wrapper.h"
#include "menu.h"

int main(int argc, char *argv[]){

	if(!SDLWrapperInit()){
		return EXIT_FAILURE;
	}
	
	menu();
	SDLWrapperQuit();
	
	return EXIT_SUCCESS;
}
