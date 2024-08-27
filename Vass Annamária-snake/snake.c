#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <time.h>
#include "snake.h"
#include "apple.h"
#include "debugmalloc.h"
#include "commongraphics.h"

/*
    A kígyó farkát tartalmazó duplán láncolt lista végéhez hozzáfűz mégegy farokelemet.
    A játék végén a hívónak fel kell szabadítania a dinamikusan lefoglalt memóriát!
*/
void add_tail(Snake *snake)
{
    Tail *newtail = (Tail*) malloc(sizeof(Tail));
    newtail->color.r=0;
    newtail->color.g=0;
    newtail->color.b=0;
    newtail->color.a=255;
    newtail->next = NULL;
    snake->tail_last->next = newtail;
    newtail->pervious = snake->tail_last;
    snake->tail_last=newtail;
}
/*
    Amikor a játékos meghal a kígyóval, ez a függvény felszabadítja a kígyó farkának dinamikusan lefoglalt láncolt listát
*/

void free_snake(Snake *snake)
{
    Tail *temp=snake->tail_first;
    while(temp!=NULL)
    {
        snake->tail_first=snake->tail_first->next;
        free(temp);
        temp=snake->tail_first;
    }
}

/*
    Ez a függvény beállítja a kezdőállapotot: A kígyó alaphelyzetét, színét, az almát elhelyezi egy random helyre, kiírja a kezdeti pontszámot.
    Előállítja a kígyó kezdeti hosszúságú farkát, dinamikusan foglal memóriát, létrejön a láncolt lista.
*/

void beginning(Snake *snake, Apple *apple)
{
    srand(time(0));
    apple->r=7;
    apple->position.x=rand()%(ZONE_W-ZONE_X-apple->r) + ZONE_X+2*apple->r;
    apple->position.y=rand()%(ZONE_H-ZONE_Y-apple->r)+ZONE_Y+2*apple->r;
    snake->position.x=200;
    snake->position.y=200;
    snake->ssize.h=20;
    snake->ssize.w=20;
    snake->dir=up;
    snake->tail_first = (Tail*) malloc(sizeof(Tail));
    snake->tail_first->position.x=snake->position.x;
    snake->tail_first->position.y=snake->position.y + snake->ssize.h;
    snake->tail_first->next=NULL;
    snake->tail_last= snake->tail_first;
    for(int i=0; i<2; i++)
    {
        add_tail(snake);
    }
}

/*
   Ez a függvény a multiplayer játékmódhoz állítja elő a kezdőállapotot, a két kígyó színét, helyzetét, az alma random kezdeti helyét,
   valamint mindkét kígyóhoz létrehoz egy-egy láncolt listát ami a kígyók farkait tartalmazza.
*/

void beginning_multiplayer(Snake *snake1, Snake *snake2, Apple *apple)
{
    srand(time(0));
    beginning(snake1,apple);
    snake2->position.x=800;
    snake2->position.y=200;
    snake2->ssize.h=20;
    snake2->ssize.w=20;
    snake2->color.r=135;
    snake2->color.g=211;
    snake2->color.b=124;
    snake2->color.a=255;
    snake2->dir=up;
    snake2->tail_first = (Tail*) malloc(sizeof(Tail));
    snake2->tail_first->position.x=snake2->position.x;
    snake2->tail_first->position.y=snake2->position.y + snake2->ssize.h;
    snake2->tail_first->next=NULL;
    snake2->tail_last= snake2->tail_first;
    for(int i=0; i<2; i++)
    {
        add_tail(snake2);
    }
}

/*
    Ez a függvény ellenőrzi, hogy a kígyó kiment-e a játékterületről, és ha igen akkor a pálya ellentétes oldalára helyezi a kígyót (a 'free' modeban használjuk,
    amikor nincsen fal a pálya körül.
*/
void left_screen(Snake *snake)
{
    if(snake->position.y+snake->ssize.h > ZONE_H+ZONE_Y)
        snake->position.y=ZONE_Y;
    else if(snake->position.y <ZONE_Y)
        snake->position.y=ZONE_H+ZONE_Y-snake->ssize.h;
    else if(snake->position.x <ZONE_X)
        snake->position.x=ZONE_W+ZONE_X-snake->ssize.h;
    else if(snake->position.x + snake->ssize.w> ZONE_W+ZONE_X)
        snake->position.x=ZONE_X;
}

/*
    Ez a logikai típusú függvény ellenőrzi, hogy a kígyó nekiütközött-e a falnak a 'wall' játékmódban.
    Ha a kígyó nekimegy a falnak a függvény true-val tér vissza
*/
bool snake_collides_with_wall(Snake *snake)
{
    if((snake->position.y+snake->ssize.h >= ZONE_H+ZONE_Y) || (snake->position.y <= ZONE_Y) || (snake->position.x <=ZONE_X) || (snake->position.x + snake->ssize.w>= ZONE_W+ZONE_X))
        return true;
    return false;
}

/*
    Ez a függvény mozgatja a kígyót a megadott irányba, és használja a left_screen függvényt, amelyik vissszarakja a kígyót a játékterületre, ha elhagyja azt.
    A farkának mozgását úgy oldja meg, hogy az egyes farokelemeket mindig az előző farokelem helyére rakja.
*/
void snake_move(SDL_Renderer *renderer,Snake *snake)
{
    for(Tail* mozgo = snake->tail_last; mozgo!=snake->tail_first; mozgo=mozgo->pervious)
    {
        mozgo->position=mozgo->pervious->position;
    }
    snake->tail_first->position=snake->position;
    switch(snake->dir)
    {
    case up:
        snake->position.y-=V;
        break;
    case down:
        snake->position.y+=V;
        break;
    case right:
        snake->position.x+=V;
        break;
    case left:
        snake->position.x-=V;
        break;
    }
    left_screen(snake);
}

/*
    Ez a függvény kiszámolja két pont között a távolságot
*/
double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

/*
    Ez a logikai típúsú függvény visszatéríti, hogy a kígyó megette-e az almát, azaz ütközik-e vele a képernyőn.
    Ha a kígyó megette az almát, true értékkel tér vissza, ellenben false-al.
*/
bool snake_eats(Snake *snake, Apple *apple)
{
    for(int i=0; i<snake->ssize.w; i++)
    {
        if(distance(snake->position.x + i, snake->position.y, apple->position.x, apple->position.y)<=apple->r)
            return true;
        if(distance(snake->position.x + i, snake->position.y+snake->ssize.h, apple->position.x, apple->position.y)<=apple->r)
            return true;
    }
    for(int i=0; i<snake->ssize.h; i++)
    {
        if(distance(snake->position.x, snake->position.y + i, apple->position.x, apple->position.y)<=apple->r)
            return true;
        if(distance(snake->position.x + snake->ssize.w, snake->position.y+i, apple->position.x, apple->position.y)<=apple->r)
            return true;
    }
    return false;
}

/*
    Ez a logikai típusú függvény igazat térít vissza, ha a kígyó a saját farkának ütközik neki, másképp hamisat.
*/
bool death1(Snake *snake)
{
    for(Tail *mozgo=snake->tail_first; mozgo!=NULL; mozgo=mozgo->next)
    {
        if(distance(snake->position.x + snake->ssize.w, snake->position.y + snake->ssize.w, mozgo->position.x+snake->ssize.w, mozgo->position.y+ snake->ssize.w)<5)
            return true;
    }
    return false;
}

/*
    Ez a függvény igazat térít vissza, ha a kétjátékosmódban a két kígyó egymásnak ütközik, ellenben hamissal tér vissza a hívóhoz.
*/
bool death2(Snake *snake1, Snake *snake2)
{
    for(Tail *mozgo=snake1->tail_first; mozgo!=NULL; mozgo=mozgo->next)
    {
        if(distance(snake2->position.x + snake2->ssize.w, snake2->position.y + snake2->ssize.w, mozgo->position.x+snake2->ssize.w, mozgo->position.y+ snake2->ssize.w)<20)
            return true;
    }
    for(Tail *mozgo=snake2->tail_first; mozgo!=NULL; mozgo=mozgo->next)
    {
        if(distance(snake1->position.x + snake1->ssize.w, snake1->position.y + snake1->ssize.w, mozgo->position.x+snake1->ssize.w, mozgo->position.y+ snake1->ssize.w)<20)
            return true;
    }
    return false;
}
