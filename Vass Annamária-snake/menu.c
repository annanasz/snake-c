#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "commongraphics.h"
#include "singleplayer.h"


/* ablak+megjelenítő létrehozása */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;
}
/*
    Ez a függvény jeleníti meg a menüt a benne található gombokkal, valamint a játéklogoval együtt.
*/
void menu_show(SDL_Renderer *renderer, Button *buttons, SDL_Rect *pic_rect, SDL_Texture* pic)
{
    SDL_SetRenderDrawColor(renderer, 250, 159, 143, 255);
    SDL_RenderClear(renderer);
    for(int i=0; i<5; i++)
    {
        Button_Draw(renderer, &buttons[i], i);
    }
    SDL_RenderCopy(renderer,pic,NULL,pic_rect);
    SDL_RenderPresent(renderer);
}

/*
    Ez a függvény jeleníti meg a dicsőséglistát, azáltal, hogy fájlból kiolvassa azt, és kiírja a képernyőre.
    Ha a score változó -1, akkor tudjuk, hogy a menüből volt meghívva a függvény, ellenkező esetben pedig a singleplayer
    mód végén, ilyenkor a függvény ki is írja az aktuális játékos pontszámát is.
*/
void Leaderboard_show(SDL_Renderer *renderer,int score ,SDL_Texture *leaderboard_img)
{
    SDL_SetRenderDrawColor(renderer, 250, 159, 143, 255);
    SDL_RenderClear(renderer);
    SDL_Rect leaderboard1= {300,10,90,90},leaderboard2= {600,10,90,90};
    SDL_RenderCopy(renderer,leaderboard_img,NULL,&leaderboard1);
    SDL_RenderCopy(renderer,leaderboard_img,NULL,&leaderboard2);
    Button exitbutton = {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 500, .w = 100, .h = 40 }};
    bool quit=false;
    Button_Draw(renderer,&exitbutton,close);
    SDL_Rect zone = {.x=200,.y=100,.h=300,.w=600};
    SDL_SetRenderDrawColor(renderer,243, 241, 239, 255);
    SDL_RenderFillRect(renderer, &zone);
    stringRGBA(renderer,445,80,"TOP 5 PLAYERS:",0,0,0,255);
    stringRGBA(renderer,250,110,"NAME",0,0,0,255);
    stringRGBA(renderer,700,110,"POINTS",0,0,0,255);
    if(score!=-1)
    {
        stringRGBA(renderer,445,450,"YOUR SCORE:",0,0,0,255);
        char s[10];
        itoa(score,s,10);
        stringRGBA(renderer,535,450,s,0,0,0,255);
    }
    Player top_5[5];
    FILE *fp = fopen("leaderboard.txt","rt");
    if(fp==NULL)
    {
        printf("nem sikerult megnyitni a fajlt");
        return;
    }
    for(int i=0; i<5; i++)
    {
        char temp[10];
        fscanf(fp, " %[^\n]", top_5[i].name);
        fscanf(fp, " %[^\n]", temp);
        stringRGBA(renderer,220,(150+i*50),top_5[i].name,0,0,0,255);
        stringRGBA(renderer,730,(150+i*50),temp,0,0,0,255);
    }
    fclose(fp);
    SDL_RenderPresent(renderer);
    while(!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && Button_Click(&exitbutton,&ev))
            quit=true;
    }
}
/*
    Ez a függvény jeleníti meg a help menüpontot, amelyben egy rövid leírás található a játék menetéről.
*/
void help_show(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 250, 159, 143, 255);
    SDL_RenderClear(renderer);
    Button exitbutton = {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 500, .w = 100, .h = 40 }};
    bool quit=false;
    Button_Draw(renderer,&exitbutton,close);
    SDL_Rect zone = {.x=100,.y=100,.h=300,.w=800};
    SDL_SetRenderDrawColor(renderer,232, 196, 143, 255);
    SDL_RenderFillRect(renderer, &zone);
    SDL_RenderPresent(renderer);
    FILE *fp=fopen("help.txt","rt");
    if(fp==NULL)
    {
        printf("Nem sikerult megnyitni a fajlt");
        return;
    }
    char temp[200];
    int y=125;
    fscanf(fp, " %[^\n]", temp);
    while(strcmp(temp,"end")!=0)
    {
        stringRGBA(renderer,120,y,temp,0,0,0,255);
        y+=15;
        fscanf(fp, " %[^\n]", temp);
    }
    fclose(fp);
    SDL_RenderPresent(renderer);
    while(!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT && Button_Click(&exitbutton,&ev))
            quit=true;
    }
}
