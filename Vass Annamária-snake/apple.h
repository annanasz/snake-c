#ifndef APPLE_H_INCLUDED
#define APPLE_H_INCLUDED
#include <SDL2/SDL2_gfxPrimitives.h>

/*
    Egy pont koordinátái struktúrában.
*/
typedef struct Point
{
    int x,y;
} Point;
/*
    Az alma struktúrája, tartalmazza a helyének koordinátáit és a sugarát.
*/
typedef struct Apple
{
    Point position;
    int r;
} Apple;

void reposition_apple(SDL_Renderer *renderer, Apple *apple);

#endif // APPLE_H_INCLUDED
