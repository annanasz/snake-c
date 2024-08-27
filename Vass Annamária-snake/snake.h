#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "apple.h"

/*
    A sz�nek k�nnyebb kezel�s��rt l�trej�tt strukt�ra, amleyben az r,g,b,a komponensek tal�lhat�ak
*/
typedef struct Color
{
    Uint8 r,g,b,a;
} Color;

/*
    Az ir�nyokat tartalmaz� felsorolt t�pus, mivel az ir�nynak m�s �rt�kei nem lehetnek.
*/
typedef enum Direction
{
    up,down,right,left
} Direction;

/*
    Egy t�glalap sz�less�g�t �s magass�g�t tartalmaz� strukt�ra.
*/
typedef struct Size
{
    int w, h;
} Size;

/*
    A k�gy� fark�nak adatait tartalmaz� strukt�ra, amelyben elt�rol�dik hogy a k�gy� fark�t alkot� n�gyzetek �ppen melyik poz�ci�ban vannak,
    valamint a sz�n�ket. A strukt�ra utols� adattagja tartalmazza az adott farokelem mellett,a l�ncolt list�ban k�toldalt tal�lhat� farokelemekre mutat� pointereket.

*/
typedef struct Tail
{
    Color color;
    Point position;
    struct Tail *next,*pervious;
} Tail;

/*
    A k�gy� tulajdons�gait tartalmaz� strukt�ra: ir�ny, hely�nek koordin�t�i, m�rete, sz�ne.
    A strukt�ra utols� tagja a k�gy� fark�t tartalmaz� dupl�n l�ncolt lista elej�re �s v�g�re mutat� pointerek, teh�t ez a t�pus tulajdonk�ppen a
    "k�gy� feje", amit a j�t�k sor�n k�vet a farka is.
*/
typedef struct Snake
{
    Direction dir;
    Point position;
    Size ssize;
    Color color;
    Tail *tail_first, *tail_last;
} Snake;

//A k�gy� sebess�ge
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
