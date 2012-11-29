#ifndef _POSITION_H_
#define _POSITION_H_

#include "SDL.h"

typedef struct{
	float x;
	float y;
}Position;

int isOverlap(const Position *, const SDL_Surface *, const Position *, const SDL_Surface *);
SDL_Rect *getSDLRectFromPosition(const Position *); //Utiliser uniquement pour l'affichage (SDL_BlitSurface)

#endif
