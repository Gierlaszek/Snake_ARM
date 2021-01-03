/**
* 	SNAKE 
*		@author Kamil Gierlach
*		
*/

#include "snake.h"
#include "stdbool.h"
#include "lcd.h"

/* snake defaults*/
#define SNAKE_lenght 10
#define SNAKE_speed 20 

/* snake counts*/
static int snake_lenght = SNAKE_lenght;
static int point = 0;
static int speed = SNAKE_speed;

/* snake coords*/
static int xSnake, ySnake; 
static int xFood, yFood; 
//static int snake_coords [LCD_WIDTH*LCD_HEIGHT / 8];

/* directions*/
static bool LEFT = false; 
static bool RIGHT = false;


//TODO
//sterowanie snakiem
//rysowanie snake
//rysowanie jedzenia
//zjadanie jeddzenia
//snake 
//tablica dwuwymiarowa 
//poruszanie sie prawo lewo - zmiana x i y snak'a i wtedy zmiana w tablicy o wymiarze lcd buffer 
//rysowanie snak'e
//przeslanie tablicy dwuwymiarowej jako lcd_bbuffer i wtedy wyswietlanie 
//rysowanie jedzenia 
//randomowe x i y, (bo dwa jedzenia na raz bedzie rysowalo, lub jedno zobaczymy)
//w miejscu x dodajemy kilka bitow dookola zeby wyrywsoalo ladnie -> sprawdzamy czy nie wyrysuje w snak'u
//pozniej przekazujemy do lcd_buffor i wyswietlamy 



