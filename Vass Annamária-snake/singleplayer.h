#ifndef SINGLEPLAYER_H_INCLUDED
#define SINGLEPLAYER_H_INCLUDED
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
    Az egyjátékos módon belül választható két játékmódot tartalmazó felsorolt típus:
        free-szabad játékmód, nincsen fal
        wall-a pályát egy fal veszi körül
*/
typedef enum Game_mode
{
    freemode,wallmode
} Game_mode;

/*
    Egy játékos pontjait illetve nevét tartalmazó struktúra.
*/
typedef struct Player
{
    char name[50];
    int score;
} Player;

void singleplayer_show(SDL_Renderer *renderer,Button* singleplayer_buttons, Snake snake, Apple apple);

void color_picker_show(SDL_Renderer *renderer,Button* colors,Button* gamemodes,Button* startbutton);

void color_picking_show(SDL_Renderer *renderer,Button *colors, Button* clicked);

void wall_show(SDL_Renderer *renderer);

void gamemode_picking(SDL_Renderer *renderer,Button *modes, Button* clicked);

void TTF_Font_init(TTF_Font **pfont, char const *tipus);

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);

void end_of_game_singleplayer(SDL_Renderer *renderer,int score);

void singleplayer_mode(SDL_Renderer *renderer);




#endif // SINGLEPLAYER_H_INCLUDED
