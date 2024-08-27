#ifndef APPLE_H_INCLUDED
#define APPLE_H_INCLUDED
#include <SDL2/SDL2_gfxPrimitives.h>

/*
    Egy pont koordin�t�i strukt�r�ban.
*/
typedef struct Point
{
    int x,y;
} Point;
/*
    Az alma strukt�r�ja, tartalmazza a hely�nek koordin�t�it �s a sugar�t.
*/
typedef struct Apple
{
    Point position;
    int r;
} Apple;

void reposition_apple(SDL_Renderer *renderer, Apple *apple);

#endif // APPLE_H_INCLUDED
