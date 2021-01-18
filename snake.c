/**
* 	SNAKE 
*		@author Kamil Gierlach
*		
*/

#include "buttons.h"
#include "snake.h"
#include "lcd.h"
#include "string.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include "tsi.h"
#include "ADC.h"
#include "tpm_pcm.h"
#include "player_name.h"
#include "MKL05Z4.h"

/* snake defaults*/
#define SNAKE_lenght 10
#define SNAKE_width 2


/* snake counts*/
static uint8_t snake_lenght = SNAKE_lenght;
static uint8_t point;
static uint8_t snake_width = SNAKE_width;

/* snake coords*/
static uint8_t xSnake, ySnake; 
static uint8_t xFood, yFood; 
static uint8_t snakeRow[WIDTH];
static uint8_t snakeCol[HEIGHT];

#define  Left 1
#define  Right 2


/*snake direction*/
static bool right = true;
static bool left = false;
static bool HORIZONTAL = true;
static bool VERTICAL = false;
static int vectorX = 1;
static int vectorY = 0;

/*points*/
char points[] = {0x20, 0x20, 0x20};

uint8_t game_over = 0;
char out_name[];


/*random*/
float ADC;
float correct_ADC = ((float)(((float)2.91) / 4095) );	 // Wspólczynnik korekcji wyniku, w stosunku do napiecia referencyjnego przetwornika


void snake_intro()
{
	lcd_clear();
	lcd_draw_bitmap(intro);
	lcd_copy();
	//DELAY(10000)
	lcd_clear();
	lcd_copy();
}

void scoreboard(){
	
	uint8_t PAUSE = get_pause();
	if(PAUSE == 0){  //button pause is on 
			lcd_draw_bitmap(play);
	}
	else if(PAUSE == 1){  //button pause is off
		lcd_draw_bitmap(pause);
	}
	
	sprintf(points, "%d", point);
	lcd_draw_text(0, 67, points);
	lcd_draw_line_x_or_y(0,8,83,8,'x');
	lcd_copy();
}

void rand_xFood()
{
		
		ADC = get_resultADC();
		ADC = ADC*correct_ADC*10000;		// Dostosowanie wyniku do zakresu napieciowego
		xFood = (int)ADC % 100;
		if(xFood >= 82)
		{
			xFood = xFood / 4; //
		}
		else 
		{
			xFood = xFood / 2;
		}	
	
		if(xFood < 2)
		{
			xFood = xFood + 2;
		}


}
	
void rand_yFood()
{

		ADC = get_resultADC();
		ADC = ADC*correct_ADC*10000;		// Dostosowanie wyniku do zakresu napieciowego
		yFood = (int)ADC % 100;
		
		if(yFood >= 46)
		{
			yFood = yFood / 4;
		}
		else
		{
			yFood = yFood / 2;
		}
		
		if(yFood < 10)
		{
			yFood = yFood + 10;
		}
	
}	

void random_food()
{
	char out[5];
	uint8_t result;
	result = get_result_ok();
	if(result)
	{
		rand_xFood();
		rand_yFood();
		
		set_result_ok(0);
	}
		//sprawdzenie czy losowe jedzenie nie jest w snake'u		
}

void eat_food()
{
	if(xFood == snakeRow[0])
	{
		if(yFood == snakeCol[0])
		{
			++point;
			if(point > 10)
			{
				snake_lenght++;
			}
			
			snake_lenght++;
			sprintf(points, "%d", point);
			lcd_draw_text(0, 67, points);
			del_square(xFood, yFood);
			random_food();
			TPM0_PCM_Play();
		}
	}
}

void draw_snake(){
	for(int i = 0; i < snake_lenght; i++)
	{
		draw_square(snakeRow[i], snakeCol[i]);
	}
	draw_square(xFood, yFood);
	lcd_copy();
}

void draw_square(uint8_t x, uint8_t y){
	x = x * snake_width;
	y = y * snake_width;
	
	lcd_draw_pixel(x, y);
	lcd_draw_pixel(x + 1, y);
	lcd_draw_pixel(x, y + 1);
	lcd_draw_pixel(x + 1, y + 1);
	
}

void del_square(uint8_t x, uint8_t y){
	x = x * snake_width;
	y = y * snake_width;
	
	lcd_del_pixel(x, y);
	lcd_del_pixel(x + 1, y);
	lcd_del_pixel(x, y + 1);
	lcd_del_pixel(x + 1, y + 1);

}


void snake_init()
{		
	TPM0_Init_PCM();
	random_food();
	xSnake = 42 / snake_width;
	ySnake = 21 / snake_width;
	for(uint8_t i = 0; i < snake_lenght; i++)
	{
		snakeRow[i] = xSnake - i;
		snakeCol[i] = ySnake;
	}
	scoreboard();
	draw_snake();
}

void reset_game()
{
	lcd_clear();
	lcd_copy();
	snake_lenght = SNAKE_lenght;
	point = 0;
	xSnake = 42 / snake_width;
	ySnake = 21 / snake_width;
	vectorX = 1;
	vectorY = 0;
	memset(snakeRow, '\0', WIDTH);
	memset(snakeCol, '\0', HEIGHT);
	game_over = 0;
}

void snake_move(){

	for(int i = snake_lenght - 1; i > 0; i--)
	{
		del_square(snakeRow[i], snakeCol[i]); // wylacz pixel 
		lcd_draw_line_x_or_y(0,8,83,8,'x'); //jesli bedzie znikalo jedzenie po przejsciu snake obok to dodac tutaj linijke aby poprawialo rysowanie 
		snakeRow[i] = snakeRow[i - 1];
		snakeCol[i] = snakeCol[i - 1];
	}

	snakeRow[0] += vectorX;
	snakeCol[0] += vectorY;
	
	
	//przechodzenie przez sciany
	if(snakeRow[0] >= 42)
	{
		snakeRow[0] = 1;
	}
	else if(snakeRow[0] <= 0)
	{
		snakeRow[0] = 41;
	}
	
	if(snakeCol[0] >= 24)
	{
		snakeCol[0] = 5;
	}
	else if(snakeCol[0] <= 4)
	{
		snakeCol[0] = 24;
	}	

	draw_snake();
	colision();
	eat_food();
}


void colision()
{
	//koniec gry jesli wejdzie w siebie
	for(int i = 1; i < snake_lenght; i++)
	{
		if((snakeRow[0] == snakeRow[i]) && (snakeCol[0] == snakeCol[i]))
		{
			set_pause(0);
			scoreboard();
			for(int j =0; j < 5; j++)
			{
				TPM0_PCM_Play();
				DELAY(150)
			}
			game_over = 1;
		}
	}
	
}

uint8_t get_game_over()
{
	return game_over;
}

uint8_t get_point()
{
	return point;
}

void move(int dir)
{

	if(HORIZONTAL) //porusza sie horyzontalnie 
	{
		HORIZONTAL = !HORIZONTAL; VERTICAL = true;
		if(right)		//w prawo
		{
			switch(dir){
				case Left: {vectorX = 0; vectorY = -1; right = true; left = false;} break;
				case Right: {vectorX = 0; vectorY = 1; right = false; left = true;} break;
			}
		}
		else if(left) //w lewo 
		{
			switch(dir){
				case Left: {vectorX = 0; vectorY = 1; right = false; left = true;} break;
				case Right: {vectorX = 0; vectorY = -1; right = true; left = false;} break;
			}
		}
	}

	else if(!HORIZONTAL) //porusza sie wertykalnie 
	{
		HORIZONTAL = !HORIZONTAL; VERTICAL = false;
		if(right)		//w gore 
		{
			switch(dir){
				case Left: {vectorX = -1; vectorY = 0; right = false; left = true;} break;
				case Right: {vectorX = 1; vectorY = 0; right = true; left = false;} break;
			}
		}
		else if(left) //w dol 
		{
			switch(dir){
				case Left: {vectorX = 1; vectorY = 0; right = true; left = false;} break;
				case Right: {vectorX = -1; vectorY = 0; right = false; left = true;} break;
			}
		}
	}
	snake_move();
}
	
