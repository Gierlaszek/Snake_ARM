#ifndef SNAKE
#define SNAKE

#define offset 12
#define WIDTH 84
#define HEIGHT 48 - offset

#include "stdbool.h"	
#include "MKL05Z4.h"

void draw_snake(void);
void draw_square(uint8_t x, uint8_t y);
void del_square(uint8_t x, uint8_t y);

void colision(void);

void reset_game(void);
void rand_xFood(void);
void rand_yFood(void);
void random_food(void);

void snake_init(void);
void scoreboard(void);
void snake_move(void);

void snake_time(void);


void direction(int dir);
void eat_food(void);

uint8_t get_pause(void);

void move(int dir);

#endif
