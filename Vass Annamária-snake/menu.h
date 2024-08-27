#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "commongraphics.h"

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void menu_show(SDL_Renderer *renderer, Button *buttons ,SDL_Rect *pic_rect, SDL_Texture* pic);

void Leaderboard_show(SDL_Renderer *renderer,int score,SDL_Texture *leaderboard_img);

void help_show(SDL_Renderer *renderer);


#endif // MENU_H_INCLUDED
