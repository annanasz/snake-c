#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "apple.h"

/*
    A színek könnyebb kezeléséért létrejött struktúra, amleyben az r,g,b,a komponensek találhatóak
*/
typedef struct Color
{
    Uint8 r,g,b,a;
} Color;

/*
    Az irányokat tartalmazó felsorolt típus, mivel az iránynak más értékei nem lehetnek.
*/
typedef enum Direction
{
    up,down,right,left
} Direction;

/*
    Egy téglalap szélességét és magasságát tartalmazó struktúra.
*/
typedef struct Size
{
    int w, h;
} Size;

/*
    A kígyó farkának adatait tartalmazó struktúra, amelyben eltárolódik hogy a kígyó farkát alkotó négyzetek éppen melyik pozícióban vannak,
    valamint a színüket. A struktúra utolsó adattagja tartalmazza az adott farokelem mellett,a láncolt listában kétoldalt található farokelemekre mutató pointereket.

*/
typedef struct Tail
{
    Color color;
    Point position;
    struct Tail *next,*pervious;
} Tail;

/*
    A kígyó tulajdonságait tartalmazó struktúra: irány, helyének koordinátái, mérete, színe.
    A struktúra utolsó tagja a kígyó farkát tartalmazó duplán láncolt lista elejére és végére mutató pointerek, tehát ez a típus tulajdonképpen a
    "kígyó feje", amit a játék során követ a farka is.
*/
typedef struct Snake
{
    Direction dir;
    Point position;
    Size ssize;
    Color color;
    Tail *tail_first, *tail_last;
} Snake;

//A kígyó sebessége
enum {V=10};
void left_screen(Snake *snake);
void beginning(Snake *snake,Apple *apple);
void snake_move(SDL_Renderer *renderer,Snake *snake);
bool snake_eats(Snake *snake, Apple *apple);
bool death1(Snake *snake);
bool death2(Snake *snake1, Snake *snake2);
void free_snake(Snake *snake);
void beginning_multiplayer(Snake *snake1, Snake *snake2, Apple *apple);
void add_tail(Snake *snake);
bool snake_collides_with_wall(Snake *snake);

#endif // SNAKE_H_INCLUDED
