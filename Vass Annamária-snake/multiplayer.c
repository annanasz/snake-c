#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "snake.h"
#include "apple.h"
#include "commongraphics.h"
#include "menu.h"
#include "singleplayer.h"

/*
    Ez a függvény jeleníti meg kétjátékos módban a játékpályát, valamint a játékhoz tartozó gombokat.
    Kirajzolja a pályára az almát és a kígyót is, a 'beginning_multiplayer'kezdőállapotuk alapján.
*/
void multiplayer_show(SDL_Renderer *renderer,Button* multiplayer_buttons, Snake snake1, Snake snake2, Apple apple)
{
    gamezone_show(renderer,multiplayer_buttons);
    stringRGBA(renderer,ZONE_X,ZONE_Y-20,"PLAYER1: 0",255,255,255,255);
    stringRGBA(renderer,ZONE_X+ZONE_W-100,ZONE_Y-20,"PLAYER2: 0",255,255,255,255);
    SDL_Rect del1 = {.x=snake1.position.x, .y=snake1.position.y, .w=snake1.ssize.w, .h=snake1.ssize.h},
             del2 = {.x=snake2.position.x, .y=snake2.position.y, .w=snake2.ssize.w, .h=snake2.ssize.h};
    SDL_SetRenderDrawColor(renderer,232, 196, 143, 190);
    SDL_RenderFillRect(renderer, &del1);
    SDL_RenderFillRect(renderer, &del2);
    SDL_Rect head1 = {.x=snake1.position.x, .y=snake1.position.y, .w=snake1.ssize.w, .h=snake1.ssize.h},
             head2 = {.x=snake2.position.x, .y=snake2.position.y, .w=snake2.ssize.w, .h=snake2.ssize.h};
    SDL_SetRenderDrawColor(renderer, snake1.color.r, snake1.color.g, snake1.color.b, snake1.color.a);
    SDL_RenderFillRect(renderer, &head1);
    SDL_SetRenderDrawColor(renderer, snake2.color.r, snake2.color.g, snake2.color.b, snake2.color.a);
    SDL_RenderFillRect(renderer, &head2);
    filledCircleRGBA(renderer,apple.position.x,apple.position.y,apple.r,240,52,52,255);
    SDL_RenderPresent(renderer);
}
/*
    Ez a függvény hívódik meg amikor az egyik játékos meghal, kiírja, hogy vége a játéknak, majd megjeleníti a nyertes játékos nevét,
    és vár a kilépésre, ami a close gomb megnyomásával történik meg.
*/
void end_of_game_multiplayer(SDL_Renderer *renderer,int score1, int score2)
{
    SDL_SetRenderDrawColor(renderer, 250, 159, 143, 255);
    SDL_RenderClear(renderer);
    Button exitbutton = {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 500, .w = 100, .h = 40 }};
    Button_Draw(renderer,&exitbutton,close);
    SDL_Rect gameover_rect = {250,100,500,300};
    SDL_Texture *gameover=IMG_LoadTexture(renderer,"gameover.jpg");
    if(score1>score2)
        stringRGBA(renderer,445,450,"WINNER: PLAYER1",0,0,0,255);
    else
        stringRGBA(renderer,445,450,"WINNER: PLAYER2",0,0,0,255);
    SDL_RenderCopy(renderer,gameover,NULL,&gameover_rect);
    SDL_RenderPresent(renderer);
    bool quit=false;
    while(!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && Button_Click(&exitbutton,&ev))
            quit=true;
    }
    SDL_DestroyTexture(gameover);
}

/*
    Ez a függvény irányítja tulajdonképpen a multiplayer módot, meghívja a pályát és a gombokat kirakójzoló függvényeket,
    érzékeli ha a játékosok a billentyűzet segítségével megváltoztatják a kígyóik haladási irányát, egy időzítő segítségével folyamatosan
    mozgatja a kígyókat a nekik megfelelő irányba. Érzékeli ha egyik játékos kígyója saját magának ütközik, vagy ha egymásnak ütköznek a kígyók,
    ilyenkor pedig meghívja a játék végét megjelenítő függvényt.
*/
void multiplayer_mode(SDL_Renderer *renderer)
{
    int score1=0,score2=0;
    Snake snake1,snake2;
    Apple apple;
    snake1.color.r=0;
    snake1.color.g=181;
    snake1.color.b=204;
    snake1.color.a=255;
    Button multiplayer_buttons[3]=
    {
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }},
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X+ZONE_W/2-50, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }},
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X+ZONE_W-100, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }}
    };
    bool multiplayer_running=true;
    bool stop=false;
    beginning_multiplayer(&snake1,&snake2,&apple);
    multiplayer_show(renderer,multiplayer_buttons,snake1,snake2,apple);
    SDL_TimerID timer = SDL_AddTimer(40,idozit,NULL);
    while(multiplayer_running)
    {
        SDL_Event mp_ev;
        SDL_WaitEvent(&mp_ev);
        switch(mp_ev.type)
        {
        case SDL_KEYDOWN:
            if(!stop)
                switch(mp_ev.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    if(snake2.dir != left)
                        snake2.dir = right;
                    break;
                case SDLK_LEFT:
                    if(snake2.dir != right)
                        snake2.dir = left;
                    break;
                case SDLK_UP:
                    if(snake2.dir != down)
                        snake2.dir = up;
                    break;
                case SDLK_DOWN:
                    if(snake2.dir != up)
                        snake2.dir = down;
                    break;
                case SDLK_d:
                    if(snake1.dir != left)
                        snake1.dir = right;
                    break;
                case SDLK_a:
                    if(snake1.dir != right)
                        snake1.dir = left;
                    break;
                case SDLK_w:
                    if(snake1.dir != down)
                        snake1.dir = up;
                    break;
                case SDLK_s:
                    if(snake1.dir != up)
                        snake1.dir = down;
                    break;
                }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(mp_ev.button.button == SDL_BUTTON_LEFT)
            {
                if(Button_Click(&multiplayer_buttons[1],&mp_ev))
                {
                    if(!stop)
                        stop=true;
                    else
                        stop=false;
                }
                else if(Button_Click(&multiplayer_buttons[2],&mp_ev))
                {
                    SDL_RemoveTimer(timer);
                    free_snake(&snake1);
                    free_snake(&snake2);
                    multiplayer_running=false;
                }
                else if(Button_Click(&multiplayer_buttons[0],&mp_ev))
                {
                    score1=0;
                    score2=0;
                    beginning_multiplayer(&snake1,&snake2,&apple);
                    multiplayer_show(renderer,multiplayer_buttons,snake1,snake2,apple);
                }
            }
            break;
        case SDL_USEREVENT:
            if(!stop)
            {
                snake_redraw(renderer,&snake2);
                snake_redraw(renderer,&snake1);
                if(snake_eats(&snake1,&apple))
                {
                    add_tail(&snake1);
                    reposition_apple(renderer,&apple);
                    score1++;
                    score_show(renderer,score1,115);
                }
                else if(snake_eats(&snake2,&apple))
                {
                    add_tail(&snake2);
                    reposition_apple(renderer,&apple);
                    score2++;
                    score_show(renderer,score2,920);
                }
                if(death1(&snake1)|| death1(&snake2) || death2(&snake1,&snake2))
                {
                    if(death1(&snake1)) score1=-1;
                    if(death1(&snake2)) score2=-1;
                    SDL_RemoveTimer(timer);
                    free_snake(&snake1);
                    free_snake(&snake2);
                    end_of_game_multiplayer(renderer,score1,score2);
                    multiplayer_running=false;
                }

                break;
            }
        }
    }
}
