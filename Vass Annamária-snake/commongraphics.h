#ifndef COMMONGRAPHICS_H_INCLUDED
#define COMMONGRAPHICS_H_INCLUDED
#include "snake.h"

//A játékterület méreteit és pozícióját tartalmazza
typedef enum zone {ZONE_X=50,ZONE_Y=50,ZONE_H=500,ZONE_W=900} zone;

/*
    A gombok fajtáját tartalmazó felsorolt típus.
*/
typedef enum Button_Type
{
    singleplayer, multiplayer, help, leaderboard,close, newgame, pause, exitgame, none, free_b,wall_b,start
} Button_Type;

/*
    Egy gomb adatait tartalmazza: az alapjául szolgáló téglalapot, a színét.
*/
typedef struct Button
{
    SDL_Rect rect;
    Color color;
} Button;

Uint32 idozit(Uint32 ms, void *param);

bool Button_Click(Button *button, SDL_Event *ev);

void Button_Draw(SDL_Renderer *renderer, Button *button, Button_Type type);

void score_show(SDL_Renderer *renderer,int score,int x);

void gamezone_show(SDL_Renderer *renderer,Button* singleplayer_buttons);

void snake_redraw(SDL_Renderer *renderer,Snake *snake);



#endif // COMMONGRAPHICS_H_INCLUDED
