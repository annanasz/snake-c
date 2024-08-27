#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "commongraphics.h"
#include "snake.h"

/*
    A singleplayer és multiplayer módokban használt időzítő
*/
Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

/*
    Ez a logikai függvény megmondja, hogy egy gombra történt az egérkattintás (true), vagy nem (false)
*/
bool Button_Click(Button *button, SDL_Event *ev)
{
    if(ev->button.x <= (button->rect.x + button->rect.w)
            && ev->button.x >= button->rect.x
            && ev->button.y <= (button->rect.y + button->rect.h)
            && ev->button.y >= button->rect.y)
        return true;
    return false;
}

/*
    Ez a függvény megrajzol egy gombot, és kiírja a típusának megfelelő szöveget rá.
*/
void Button_Draw(SDL_Renderer *renderer, Button *button, Button_Type type)
{
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_RenderFillRect(renderer, &button->rect);
    switch(type)
    {
    case singleplayer:
        stringRGBA(renderer, button->rect.x+2, button->rect.y+20, "SINGLEPLAYER", 0, 0, 0, 255);
        break;
    case multiplayer:
        stringRGBA(renderer, button->rect.x+5, button->rect.y+20, "MULTIPLAYER", 0, 0, 0, 255);
        break;
    case help:
        stringRGBA(renderer, button->rect.x+35, button->rect.y+20, "HELP", 0, 0, 0, 255);
        break;
    case leaderboard:
        stringRGBA(renderer, button->rect.x+2, button->rect.y+20, "LEADERBOARD",0,0,0,255);
        break;
    case close:
        stringRGBA(renderer, button->rect.x+30, button->rect.y+20, "CLOSE", 0, 0, 0, 255);
        break;
    case newgame:
        stringRGBA(renderer, button->rect.x+17, button->rect.y+5, "new game", 0, 0, 0, 255);
        break;
    case pause:
        stringRGBA(renderer, button->rect.x+30, button->rect.y+5, "pause", 0, 0, 0, 255);
        break;
    case exitgame:
        stringRGBA(renderer, button->rect.x+32, button->rect.y+5, "exit", 0, 0, 0, 255);
        break;
    case wall_b:
        stringRGBA(renderer, button->rect.x+8, button->rect.y+5, "wall", 0, 0, 0, 255);
        break;
    case free_b:
        stringRGBA(renderer, button->rect.x+6, button->rect.y+5, "free", 0, 0, 0, 255);
        break;
    case start:
        stringRGBA(renderer, button->rect.x+30, button->rect.y+5, "START", 0, 0, 0, 255);
        break;
    case none:
        break;
    }
}

/*
    Ez a függvény kiírja a sztringgé alakított ponszámot a képernyőre, a megadott x koordinátájú helyre.
*/
void score_show(SDL_Renderer *renderer,int score,int x)
{
    char s[10];
    itoa(score,s,10);
    boxRGBA(renderer,x,30,x+45,38,0,0,0,255);
    stringRGBA(renderer,ZONE_X + x-50,ZONE_Y-20,s,255,255,255,255);
    SDL_RenderPresent(renderer);
}
/*
    Ez a függvény kirajzolja a játékpályát és a hozzá tartozó gombokat.
*/
void gamezone_show(SDL_Renderer *renderer,Button* singleplayer_buttons)
{
    boxRGBA(renderer,0,1000,1000,0,0,0,0,255);
    SDL_Rect zone = {.x=ZONE_X,.y=ZONE_Y,.h=ZONE_H,.w=ZONE_W};
    SDL_SetRenderDrawColor(renderer,232, 196, 143, 190);
    SDL_RenderFillRect(renderer, &zone);
    for(int i=0; i<3; i++)
    {
        Button_Draw(renderer,&singleplayer_buttons[i],i+5);
    }
}

/*
    Ez a függvény rajzolja újra a kígyót a pályán, azáltal, hogy kitörli a régi helyéről, majd az új pozíciójában kirajzolja azht a farkával együtt.
*/
void snake_redraw(SDL_Renderer *renderer,Snake *snake)
{
    filledCircleRGBA(renderer,snake->position.x+ snake->ssize.w/2,snake->position.y + snake->ssize.h/2,5,232,196,143,255);
    SDL_Rect del = {.x=snake->position.x, .y=snake->position.y, .w=snake->ssize.w, .h=snake->ssize.h};
    SDL_SetRenderDrawColor(renderer,232, 196, 143, 190);
    SDL_RenderFillRect(renderer, &del);
    for(Tail*mozgo=snake->tail_first; mozgo!=NULL; mozgo=mozgo->next)
    {
        SDL_Rect deletee = {.x=mozgo->position.x, .y=mozgo->position.y, .w=snake->ssize.w, .h=snake->ssize.h};
        SDL_RenderFillRect(renderer, &deletee);
    }
    snake_move(renderer, snake);
    SDL_Rect head = {.x=snake->position.x, .y=snake->position.y, .w=snake->ssize.w, .h=snake->ssize.h};
    SDL_SetRenderDrawColor(renderer, snake->color.r, snake->color.g, snake->color.b, snake->color.a);
    SDL_RenderFillRect(renderer, &head);
    for(Tail*mozgo=snake->tail_first; mozgo!=NULL; mozgo=mozgo->next)
    {
        SDL_Rect tail = {.x=mozgo->position.x, .y=mozgo->position.y, .w=snake->ssize.w, .h=snake->ssize.h};
        SDL_RenderFillRect(renderer, &tail);
    }
    filledCircleRGBA(renderer,snake->position.x+ snake->ssize.w/2,snake->position.y + snake->ssize.h/2,5,255,255,255,255);
    filledCircleRGBA(renderer,snake->position.x+ snake->ssize.w/2,snake->position.y + snake->ssize.h/2,2,0,0,0,255);
    SDL_RenderPresent(renderer);
}
