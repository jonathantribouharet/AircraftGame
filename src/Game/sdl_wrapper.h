#ifndef _SDL_WRAPPER_H_
#define _SDL_WRAPPER_H_

#include "SDL.h"

void SDLWrapperInit();
void SDLWrapperQuit();

void clearScreen();

SDL_Surface *getSurfaceScreen();
SDL_Surface *getSurfaceFromText(const char *);
SDL_Surface *getSurfaceFromImage(const char *);
SDL_Surface *getSurfaceTransparentFromImage(const char *);

int getScreenWidth();
int getScreenHeight();

#endif
