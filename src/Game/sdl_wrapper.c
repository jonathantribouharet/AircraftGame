#include "log.h"

#include "sdl_wrapper.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

static SDL_Surface *screen = NULL;
static TTF_Font *font = NULL;

void SDLWrapperInit(){
	LOG_DEBUG("Initialize SDL");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_DOUBLEBUF);	
	SDL_EnableUNICODE(1);
	TTF_Init();
	
	screen = SDL_SetVideoMode(getScreenWidth(), getScreenHeight(), 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Aircraft Game", "Aircraft Game");
	
	font = TTF_OpenFont("data/menu_font.ttf", 40);
}

void SDLWrapperQuit(){
	LOG_DEBUG("Quit SDL");

	TTF_CloseFont(font);
	SDL_Quit();
	TTF_Quit();
}

void clearScreen(){
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));	
}

SDL_Surface *getSurfaceScreen(){
	return screen;
}

SDL_Surface *getSurfaceFromText(const char *text){
    SDL_Color fontcolor = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, fontcolor);	
	return surface;
}

SDL_Surface *getSurfaceFromImage(const char *file){
	SDL_Surface *surface = IMG_Load(file);
	SDL_Surface *optimized_surface = SDL_DisplayFormat(surface);
	SDL_FreeSurface(surface);
	return optimized_surface;
}

SDL_Surface *getSurfaceTransparentFromImage(const char *file){
	SDL_Surface *surface = 	getSurfaceFromImage(file);
	SDL_SetColorKey(surface, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 255, 255, 255));
	return surface;
}

int getScreenWidth(){
	return 800;
}

int getScreenHeight(){
	return 600;
}
