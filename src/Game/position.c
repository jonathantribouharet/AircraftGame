#include "position.h"

#define inrange(point, position_left, position_right) ((point >= position_left) && (point < position_right))

int isOverlap(const Position *position_1, const SDL_Surface *surface_1, const Position *position_2, const SDL_Surface *surface_2){
	int x_overlap = inrange(position_1->x, position_2->x, position_2->x + surface_2->w) ||
		inrange(position_2->x, position_1->x, position_1->x + surface_1->w);
	
	int y_overlap = inrange(position_1->y, position_2->y, position_2->y + surface_2->h) ||
		inrange(position_2->y, position_1->y, position_1->y + surface_1->h);	
	
	return x_overlap && y_overlap;
}

SDL_Rect *getSDLRectFromPosition(const Position * position){
	static SDL_Rect *rect = NULL;
	if(!rect){
		rect = malloc(sizeof(SDL_Rect));
	}
	
	rect->x = position->x;
	rect->y = position->y;
	
	return rect;
}
