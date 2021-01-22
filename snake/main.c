/*******************************************************************
* 	@file MAIN.c 
*		@author Kamil Gierlach, Karol Piech
*		@date 15.01.2021
*   @ver 1.6
* 	@brief File containing game mechanics
**********************************************************************/

#include "SPI.h"
#include "lcd.h"
#include "snake.h"
#include "font.h"
#include "bitmap.h"
#include "tsi.h"
#include "stdbool.h"
#include "ADC.h"
#include "buttons.h"
#include "player_name.h"
#include "flash.h"

/*DEFINITIONS*/
#define sliderVal 50
#define LEFT 1
#define RIGHT 2

/*highscore*/
static int high_point = 0;
static char high_name[10];

/*Touch slider*/
uint8_t slider;


/*********************************
* SAVING A NEW RECORD 
**********************************/
void records(int point, char *name)
{
	if(point > high_point)
	{
		lcd_clear();
		high_point = point;
		strcpy(high_name, name);
		
		lcd_draw_text(0, 0, "Bravo");
		lcd_draw_text(0, 48, name);
		lcd_draw_text(2,0, "You bit the");
		lcd_draw_text(3,0, " record!!");
		lcd_copy();
	}
}


/*********************************
* DISPLAY THE HIGHSCORE
**********************************/
void highscore()
{
	char points[5];
	sprintf(points, "%d", high_point);
	
	lcd_draw_text(0,0, "Best player");
	lcd_draw_text(2,8, high_name);
	lcd_draw_text(2,48, points);
	
	lcd_draw_text(4,0, "1. New game");
	lcd_draw_text(5,0, "2. New player");
	lcd_copy();
	
}

/**************************
* GAME OVER
**************************/
void the_end()
{
		char name[10];
		get_name(name);
		
		int point = get_point();
		char points[6];
		sprintf(points, "%d", point);
	
		records(point, name);
		DELAY(1000)
		lcd_clear();
		lcd_draw_text(1,8, "Score: ");

		lcd_draw_text(1,56, points);
		lcd_draw_text(3,8, "1. RESET GAME");
		lcd_draw_text(4,8, "2. HIGHSCORE");
		lcd_copy();
}

/*********************************
* START NEW GAME
*********************************/
void begin_the_game()
{
	if(!correct_button()) 
	{
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(2); // again initialize ADC in channel 2 for food in game 
		set_result_ok(0);
		scoreboard();
		snake_init();
	}
}

/**********************************
* RESET GAME
**********************************/
void reset()
{
	set_reset(0);
	set_high_but(0);
	reset_game();
	snake_init();
	
}

/**********************************
* GAME MECHANICS
**********************************/
void game_snake(uint8_t refresh)
{
	while(!correct_button()){
		
			uint8_t p = get_pause();
		
			if(!get_game_over())
			{
				if(p == 1) 			/*moving along the slider*/
				{
					if(refresh == 1)
					{
						scoreboard();
						refresh = 0;
					}
					slider = TSI_ReadSlider();
					if(slider == 0)
					{
						snake_move();

					}
					if(slider > 0 && slider < sliderVal)
					{
						move(LEFT);
					}
					if(slider > 0 && slider > sliderVal)
					{
						move(RIGHT);
					}
				}
				else if(p == 0) 	/*button pause is pressed*/
				{
					scoreboard(); 
					draw_snake();
					refresh = 1;
				}
			}

			/*button resset is pressed*/
			if(get_reset()) 
			{
				reset();
			}
			
			/*GAME OVER*/
			if(get_game_over())
			{
				the_end();
				
				if(get_high_but())
				{
					lcd_clear();
					lcd_copy();
					while(!get_reset())
					{
						highscore(); 
						if(correct_button())
						{
							break;
						}
					}
				}	
			}
	}
}


int main(){
	/*  initialize */ 
	TSI_Init();
	lcd_setup(); 
	buttonsInitialize();
	player_name_init();
	
	
	/* intro */
	snake_intro(); 
	
	/* set player name */
	set_player_name(); 
	set_result_ok(0); 	//approve name 
	
	
	/* beginning the game */
	begin_the_game(); 
	
	
	uint8_t refresh = 0;
	while(1)
	{
		if(!correct_button()) //game mechanics
		{
			game_snake(refresh); 
		}
		else if(correct_button()) //button new player is active 
		{
			new_player();
			set_player_name();
			set_result_ok(0);
			reset_game();
			begin_the_game();
			refresh = 0;
		}
	}
	
}
