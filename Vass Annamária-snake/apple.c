#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "apple.h"
#include "commongraphics.h"

/*
    Ez a függvény kitörli az almát a régi helyéről és új, random helyre rakja az almát, miután a kígyó megette azt.
*/
void reposition_apple(SDL_Renderer *renderer, Apple *apple)
{
    filledCircleRGBA(renderer,apple->position.x,apple->position.y,apple->r,232, 196, 143, 255);
    apple->position.x=rand()%(ZONE_W-ZONE_X-apple->r) + ZONE_X+2*apple->r;
    apple->position.y=rand()%(ZONE_H-ZONE_Y-apple->r)+ZONE_Y+2*apple->r;
    filledCircleRGBA(renderer,apple->position.x,apple->position.y,apple->r,240,52,52,255);
    SDL_RenderPresent(renderer);
}

