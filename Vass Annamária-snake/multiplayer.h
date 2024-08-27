#ifndef MULTIPLAYER_H_INCLUDED
#define MULTIPLAYER_H_INCLUDED

void multiplayer_show(SDL_Renderer *renderer,Button* multiplayer_buttons, Snake snake1, Snake snake2, Apple apple);

void end_of_game_multiplayer(SDL_Renderer *renderer,int score1, int score2);

void multiplayer_mode(SDL_Renderer *renderer);


#endif // MULTIPLAYER_H_INCLUDED
