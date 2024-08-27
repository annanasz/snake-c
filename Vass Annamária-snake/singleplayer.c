#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"
#include "apple.h"
#include "commongraphics.h"
#include "menu.h"
#include "singleplayer.h"

/*
    Az egyjátékosmód játékpályáját és a játékot irányító gombokat kirajzoló függvény.
    Emellett ez a függvény  jeleníti meg kezdetben kígyót és az almát is a képernyőn,
*/
void singleplayer_show(SDL_Renderer *renderer,Button* singleplayer_buttons, Snake snake, Apple apple)
{
    gamezone_show(renderer,singleplayer_buttons);
    stringRGBA(renderer,ZONE_X,ZONE_Y-20,"score: 0",255,255,255,255);
    SDL_Rect del = {.x=snake.position.x, .y=snake.position.y, .w=snake.ssize.w, .h=snake.ssize.h};
    SDL_SetRenderDrawColor(renderer,232, 196, 143, 190);
    SDL_RenderFillRect(renderer, &del);
    SDL_Rect head = {.x=snake.position.x, .y=snake.position.y, .w=snake.ssize.w, .h=snake.ssize.h};
    SDL_SetRenderDrawColor(renderer, snake.color.r, snake.color.g, snake.color.b, snake.color.a);
    SDL_RenderFillRect(renderer, &head);
    filledCircleRGBA(renderer,apple.position.x,apple.position.y,apple.r,240,52,52,255);
    SDL_RenderPresent(renderer);
}

/*
    Az egyjátékosmód elején található szín- illetve játékmód('free' vagy 'wall') választót kirajzoló függvény
*/
void color_picker_show(SDL_Renderer *renderer,Button* colors,Button* gamemodes,Button* startbutton)
{
    boxRGBA(renderer,0,1000,1000,0,0,0,0,255);
    for(int i=0; i<3; i++)
    {
        Button_Draw(renderer,&colors[i],none);
        filledCircleRGBA(renderer,colors[i].rect.h/2 + colors[i].rect.x,colors[i].rect.w/2+colors[i].rect.y,25,255,255,255,255);
        if(i!=2)
        {
            Button_Draw(renderer,&gamemodes[i],i+9);
            filledCircleRGBA(renderer,gamemodes[i].rect.h/2 + gamemodes[i].rect.x,gamemodes[i].rect.w/2+gamemodes[i].rect.y,10,0,0,0,255);
        }
    }
    filledCircleRGBA(renderer,colors[0].rect.h/2 + colors[0].rect.x,colors[0].rect.w/2+colors[0].rect.y,15,0,0,0,255);
    filledCircleRGBA(renderer,gamemodes[0].rect.h/2 + gamemodes[0].rect.x,gamemodes[0].rect.w/2+gamemodes[0].rect.y,5,255,255,255,255);
    Button_Draw(renderer,startbutton,start);
    stringRGBA(renderer,430,50,"PICK THE GAMEMODE:",255,255,255,255);
    stringRGBA(renderer,390,200,"PICK THE COLOR OF YOUR SNAKE:",255,255,255,255);
    SDL_RenderPresent(renderer);
}

/*
    Ez a függvény jeleníti meg az éppen kijelölt színválasztást, a kiválasztott szín radiobutton-ját feketére színezi.
*/
void color_picking_show(SDL_Renderer *renderer,Button *colors, Button* clicked)
{
    for(int i=0; i<3; i++)
    {
        filledCircleRGBA(renderer,colors[i].rect.h/2 + colors[i].rect.x,colors[i].rect.w/2+colors[i].rect.y,25,255,255,255,255);
    }
    filledCircleRGBA(renderer,clicked->rect.h/2 + clicked->rect.x,clicked->rect.w/2+clicked->rect.y,15,0,0,0,255);
    SDL_RenderPresent(renderer);
}

/*
    Ez a függvény jeleníti meg a pálya körül a falat, a 'wall' játékmód esetén.
*/
void wall_show(SDL_Renderer *renderer)
{
    boxRGBA(renderer,ZONE_X+ZONE_W+7,ZONE_Y-7,ZONE_X-7,ZONE_Y,191,191,191,255);
    boxRGBA(renderer,ZONE_X,ZONE_Y,ZONE_X-7,ZONE_Y+ZONE_H,191,191,191,255);
    boxRGBA(renderer,ZONE_X+ZONE_W+7,ZONE_Y,ZONE_X+ZONE_W,ZONE_Y+ZONE_H,191,191,191,255);
    boxRGBA(renderer,ZONE_X+ZONE_W+7,ZONE_Y+ZONE_H,ZONE_X-7,ZONE_Y+ZONE_H+7,191,191,191,255);
    SDL_RenderPresent(renderer);
}

/*
    Ez a függvény jeleníti meg az éppen kijelölt játékmódválasztást, a kiválasztott mód radiobutton-ját feketére színezi.
*/
void gamemode_picking(SDL_Renderer *renderer,Button *modes, Button* clicked)
{
    for(int i=0; i<2; i++)
    {
        filledCircleRGBA(renderer,modes[i].rect.h/2 + modes[i].rect.x,modes[i].rect.w/2+modes[i].rect.y,10,0,0,0,255);
    }
    filledCircleRGBA(renderer,clicked->rect.h/2 + clicked->rect.x,clicked->rect.w/2+clicked->rect.y,5,255,255,255,255);
    SDL_RenderPresent(renderer);
}

/*
    Ez a függvény inicializálja a játékos nevének beírásához a fontot.
*/
void TTF_Font_init(TTF_Font **pfont, char const *tipus)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(tipus, 32);
    if (font == NULL)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    *pfont = font;
}

/*
    Ez a függvény teszi lehetővé, hogy a játékos beírhassa a nevét, amiután vége a játéknak.
    Forrás: infoc
*/
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer)
{
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter)
    {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0')
        {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        }
        else
        {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw)
        {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        /* Kulonleges karakter */
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                int textlen = strlen(dest);
                do
                {
                    if (textlen == 0)
                    {
                        break;
                    }
                    if ((dest[textlen-1] & 0x80) == 0x00)
                    {
                        /* Egy bajt */
                        dest[textlen-1] = 0x00;
                        break;
                    }
                    if ((dest[textlen-1] & 0xC0) == 0x80)
                    {
                        /* Bajt, egy tobb-bajtos szekvenciabol */
                        dest[textlen-1] = 0x00;
                        textlen--;
                    }
                    if ((dest[textlen-1] & 0xC0) == 0xC0)
                    {
                        /* Egy tobb-bajtos szekvencia elso bajtja */
                        dest[textlen-1] = 0x00;
                        break;
                    }
                }
                while(true);
            }
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                enter = true;
            }
            break;

        /* A feldolgozott szoveg bemenete */
        case SDL_TEXTINPUT:
            if (strlen(dest) + strlen(event.text.text) < hossz)
            {
                strcat(dest, event.text.text);
            }

            /* Az eddigi szerkesztes torolheto */
            composition[0] = '\0';
            break;

        /* Szoveg szerkesztese */
        case SDL_TEXTEDITING:
            strcpy(composition, event.edit.text);
            break;

        case SDL_QUIT:
            /* visszatesszuk a sorba ezt az eventet, mert
             * sok mindent nem tudunk vele kezdeni */
            SDL_PushEvent(&event);
            kilep = true;
            break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

/*
    Ez a függvény hívódik meg, amikor a játékos meghal, itt kérődik be a játékos neve.
    A függvény a dicsőséglista fájlból kiolvassa az eddigi legjobb 5 játékost, megnézi, hogy az éppen most végzett játékosnak fel kell-e
    kerülnie a listára, ha igen beszúrja a megfelelő helyre, majd frissíti a dicsőséglista fájlt.
    A legvégén meghívja a dicsőséglistát megjelenítő függvényt, amelyben írja a játékos pontszámát is a 'Your score' felírat mellett.
*/
void end_of_game_singleplayer(SDL_Renderer *renderer,int score)
{
    TTF_Font *font;
    TTF_Font_init(&font, "LiberationSerif-Regular.ttf");
    SDL_SetRenderDrawColor(renderer, 250, 159, 143, 255);
    SDL_RenderClear(renderer);
    SDL_Color white= {255,255,255,255},black= {0,0,0,255};
    SDL_Rect place= {350,300,300,40};
    stringRGBA(renderer,350,280,"TYPE IN YOUR NAME:",0,0,0,255);
    char name[50];
    input_text(name,50,place,white,black,font,renderer);
    Player top_5[5];
    FILE *fp;
    fp = fopen("leaderboard.txt","rt");
    if(fp==NULL)
    {
        printf("nem sikerult megnyitni a fajlt");
        return;
    }
    for(int i=0; i<5; i++)
    {
        char temp[50];
        fscanf(fp, " %[^\n]", top_5[i].name);
        top_5[i].name[strlen(top_5[i].name)]='\0';
        fscanf(fp, " %[^\n]", temp);
        top_5[i].score=atoi(temp);
    }
    fclose(fp);
    int i;
    for(i=0; i<5; i++)
    {
        if(score>top_5[i].score)
            break;
    }
    if(i!=5)
    {
        char temp[50];
        strcpy(temp,name);
        for(int j=5; j>i; j--)
        {
            top_5[j]=top_5[j-1];
        }
        top_5[i].score=score;
        strcpy(top_5[i].name,temp);
        fp=fopen("leaderboard.txt","wt");
        if(fp==NULL)
        {
            printf("nem sikerult megnyitni a fajlt");
            return;
        }
        for(int j=0; j<5; j++)
        {
            fprintf(fp,"%s\n",top_5[j].name);
            fprintf(fp,"%d\n",top_5[j].score);
        }
        fclose(fp);
    }
    TTF_CloseFont(font);
    font = NULL;
    Leaderboard_show(renderer,score,NULL);
}
/*
    Ez a függvény vezérli az egyjátékos módot, meghívja a szín- és játékmód megjelenítő és választó függvényeket, időzítővel mozgatja a kígyót a pályán,
    és figyeli, hogy meghalt-e a játékos, vagy esetleg megnyomta-e valamelyik gombot, valamint érzékeli a billentyűzeten található nyilak megnyomását,
    ezáltal beállítja, hogy melyik irányba mozogjon a kígyó. Itt számolódik a játékos pontszáma is.

*/
void singleplayer_mode(SDL_Renderer *renderer)
{
    int score=0;
    Snake snake;
    Apple apple;
    bool singleplayer_running=false;
    bool stop=false;
    Game_mode mode=freemode;
    snake.color.r=0;
    snake.color.g=181;
    snake.color.b=204;
    snake.color.a=255;
    Button singleplayer_buttons[3]=
    {
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }},
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X+ZONE_W/2-50, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }},
        {.color = {.r=255,.g=255,.b=255,.a=0}, .rect= { .x = ZONE_X+ZONE_W-100, .y = ZONE_Y+ZONE_H+10, .w = 100, .h = 20 }}
    };
    bool color_pick=true;
    Button colors[3]=
    {
        {.color = {.r=0,.g=181,.b=204,.a=255}, .rect= { .x = 50, .y = 250, .w = 270, .h = 270 }},
        {.color = {.r=135,.g=211,.b=124,.a=255}, .rect= { .x = 355, .y = 250, .w = 270, .h = 270 }},
        {.color = {.r=219,.g=10,.b=91,.a=255}, .rect= { .x = 660, .y = 250, .w = 270, .h = 270 }}
    };
    Button gamemodes[2]=
    {
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 350, .y = 70, .w = 50, .h = 50 }},
        {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 600, .y = 70, .w = 50, .h = 50 }}
    };
    Button startbutton= {.color = {.r=255,.g=255,.b=255,.a=255}, .rect= { .x = 450, .y = 550, .w = 100, .h = 20 }};
    color_picker_show(renderer,colors,gamemodes,&startbutton);
    while(color_pick)
    {
        SDL_Event pick;
        SDL_WaitEvent(&pick);
        if(pick.type == SDL_MOUSEBUTTONDOWN && pick.button.button == SDL_BUTTON_LEFT)
        {
            if(Button_Click(&startbutton,&pick))
            {
                color_pick=false;
            }
            for(int i=0; i<3; i++)
            {
                if(Button_Click(&colors[i],&pick))
                {
                    snake.color = colors[i].color;
                    color_picking_show(renderer,colors,&colors[i]);
                }
            }
            for(int i=0; i<2; i++)
            {
                if(Button_Click(&gamemodes[i],&pick))
                {
                    mode=i;
                    gamemode_picking(renderer,gamemodes,&gamemodes[i]);
                }
            }
        }
    }
    singleplayer_running=true;
    beginning(&snake,&apple);
    singleplayer_show(renderer,singleplayer_buttons,snake,apple);
    if(mode==wallmode)
        wall_show(renderer);
    SDL_TimerID timer = SDL_AddTimer(40,idozit,NULL);
    while(singleplayer_running)
    {
        SDL_Event sp_ev;
        SDL_WaitEvent(&sp_ev);
        switch(sp_ev.type)
        {
        case SDL_KEYDOWN:
            if(!stop)
                switch(sp_ev.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    if(snake.dir != left)
                        snake.dir = right;
                    break;
                case SDLK_LEFT:
                    if(snake.dir != right)
                        snake.dir = left;
                    break;
                case SDLK_UP:
                    if(snake.dir != down)
                        snake.dir = up;
                    break;
                case SDLK_DOWN:
                    if(snake.dir != up)
                        snake.dir = down;
                    break;
                }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(sp_ev.button.button == SDL_BUTTON_LEFT)
            {
                if(Button_Click(&singleplayer_buttons[1],&sp_ev))
                {
                    if(!stop)
                        stop=true;
                    else
                        stop=false;
                }
                else if(Button_Click(&singleplayer_buttons[2],&sp_ev))
                {
                    SDL_RemoveTimer(timer);
                    free_snake(&snake);
                    singleplayer_running=false;
                }
                else if(Button_Click(&singleplayer_buttons[0],&sp_ev))
                {
                    score=0;
                    beginning(&snake,&apple);
                    singleplayer_show(renderer,singleplayer_buttons,snake,apple);
                }
            }
            break;
        case SDL_USEREVENT:
            if(!stop)
            {
                if(snake_collides_with_wall(&snake) && mode==wallmode)
                {
                    SDL_RemoveTimer(timer);
                    free_snake(&snake);
                    end_of_game_singleplayer(renderer,score);
                    singleplayer_running=false;
                    break;
                }
                if(death1(&snake))
                {
                    SDL_RemoveTimer(timer);
                    free_snake(&snake);
                    end_of_game_singleplayer(renderer,score);
                    singleplayer_running=false;
                    break;
                }
                snake_redraw(renderer,&snake);
                if(snake_eats(&snake,&apple))
                {
                    add_tail(&snake);
                    reposition_apple(renderer,&apple);
                    score++;
                    score_show(renderer,score,105);
                }
            }
            break;
        }
    }
}
