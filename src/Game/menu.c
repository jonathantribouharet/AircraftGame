#include "sdl_wrapper.h"
#include "label.h"
#include "game.h"

enum { MENU_PLAY, MENU_QUIT };
static const int MENUS_SIZE = 2;

static const int ARROW_OFFSET_X = 20;
static const int MENU_OFFSET_Y = 80;

typedef struct{
    int active;
    int position;
    SDL_Event event;
        
    Label menus[MENUS_SIZE];
    Label arrow;
}MenuContext;

static void initContext(MenuContext *context);
static void closeContext(MenuContext *context);
static void displaySurfaces(MenuContext *context);
static void processEvent(MenuContext *context);
static void refreshMenuArrowPosition(MenuContext *context);
static void setMenusPosition(MenuContext *context);

void menu(){
    MenuContext context;

    initContext(&context);

    while(context.active){      
        processEvent(&context);
        displaySurfaces(&context);
    }
    
    closeContext(&context);
}

static void initContext(MenuContext *context){
    context->active = 1;
    context->position = 0;

    context->menus[0].surface = getSurfaceFromText("Play");
    context->menus[1].surface = getSurfaceFromText("Quit");
    context->arrow.surface = getSurfaceFromText("-->");

    setMenusPosition(context);
}

static void closeContext(MenuContext *context){
    SDL_FreeSurface(context->arrow.surface);

    for(int i = 0; i < MENUS_SIZE; ++i){
       SDL_FreeSurface(context->menus[i].surface);
    }
}

static void displaySurfaces(MenuContext *context){
    clearScreen();
    SDL_BlitSurface(context->arrow.surface, NULL, getSurfaceScreen(), &context->arrow.position);

    for(int i = 0; i < MENUS_SIZE; ++i){
        SDL_BlitSurface(context->menus[i].surface, NULL, getSurfaceScreen(), &context->menus[i].position);    
    }
    
    SDL_Flip(getSurfaceScreen());
}

static void processEvent(MenuContext *context){
    SDL_WaitEvent(&context->event);

    if(context->event.type == SDL_QUIT){
        context->active = 0;
    }
    else if(context->event.type == SDL_KEYDOWN){
        switch(context->event.key.keysym.sym){
            case SDLK_ESCAPE:
                context->active = 0;
                break;
            case SDLK_UP:
                if(context->position > 0){
                    context->position--;
                }                   

                refreshMenuArrowPosition(context);
                break;
            case SDLK_DOWN:
                if(context->position < MENUS_SIZE - 1){
                    context->position++;
                }

                refreshMenuArrowPosition(context);
                break;
            case SDLK_RETURN:
                switch(context->position){
                    case MENU_PLAY:
                        context->active = play();
                        break;           
                    case MENU_QUIT:
                        context->active = 0;
                        break;
                }
                break;
            default:
                break;
        }
    }
}

static void refreshMenuArrowPosition(MenuContext *context){
    context->arrow.position.y = context->menus[context->position].position.y;
}

static void setMenusPosition(MenuContext *context){
    int MENUS_MAX_WIDTH = 0;
    int MENUS_HEIGHT = 0;

    for(int i = 0; i < MENUS_SIZE; ++i){
        if(context->menus[i].surface->h > MENUS_MAX_WIDTH){
            MENUS_MAX_WIDTH = context->menus[i].surface->w;
        }
    }

    for(int i = 0; i < MENUS_SIZE; ++i){
        MENUS_HEIGHT += context->menus[i].surface->h;
    }
    MENUS_HEIGHT += MENU_OFFSET_Y * (MENUS_SIZE - 1);

    for(int i = 0; i < MENUS_SIZE; ++i){
        context->menus[i].position.x = (getScreenWidth() - MENUS_MAX_WIDTH) / 2;
        context->menus[i].position.y = (getScreenHeight() - MENUS_HEIGHT) / 2 + (i * MENU_OFFSET_Y);
    }

    context->arrow.position.x = context->menus[0].position.x - context->arrow.surface->w - ARROW_OFFSET_X;
    refreshMenuArrowPosition(context);
}
