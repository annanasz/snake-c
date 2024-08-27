#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "menu.h"
#include "singleplayer.h"
#include "commongraphics.h"
#include "multiplayer.h"

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Snake", 1000, 600, &window, &renderer);
    SDL_Texture *pic=IMG_LoadTexture(renderer,"snake.png");
    SDL_Texture *leaderboard_img=IMG_LoadTexture(renderer,"leaderboard.png");
    SDL_Rect pic_rect = {.x=300,.y=50,.w=375,.h=190};
    SDL_RenderPresent(renderer);
    Button buttons[5]=
    {
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 300, .w = 100, .h = 40 }},
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 350, .w = 100, .h = 40 }},
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 400, .w = 100, .h = 40 }},
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 450, .w = 100, .h = 40 }},
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 500, .w = 100, .h = 40 }}
    };
    menu_show(renderer,buttons,&pic_rect,pic);
    bool chose=true;
    bool quit=false;
    while (!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                if(Button_Click(&buttons[singleplayer],&ev) && chose)
                {
                    chose=false;
                    singleplayer_mode(renderer);
                    chose=true;
                    menu_show(renderer,buttons,&pic_rect,pic);
                }
                else if(Button_Click(&buttons[multiplayer],&ev) && chose)
                {
                    chose=false;
                    multiplayer_mode(renderer);
                    chose=true;
                    menu_show(renderer,buttons,&pic_rect,pic);

                }
                else if(Button_Click(&buttons[help],&ev))
                {
                    help_show(renderer);
                    menu_show(renderer,buttons,&pic_rect,pic);
                }
                else if(Button_Click(&buttons[leaderboard],&ev))
                {
                    Leaderboard_show(renderer,-1,leaderboard_img);
                    menu_show(renderer,buttons,&pic_rect,pic);
                }
                else if(Button_Click(&buttons[close],&ev)&& chose)
                {
                    quit=true;
                }
            }
            break;
        }
    }
    SDL_DestroyTexture(pic);
    SDL_DestroyTexture(leaderboard_img);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return 0;
}
