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


#define sliderVal 50

#define LEFT 1
#define RIGHT 2


void SysTick_Handler(void);

/*snake timer*/
static volatile unsigned int TimeMS = 0;
uint8_t TimeS = 0;
char time[6];


uint8_t slider;

/*GAME OVER*/
void the_end()
{
		lcd_clear();
		lcd_draw_text(0,8, "GAME OVER");
		lcd_draw_text(2,8, "Score: ");
	
		uint8_t point = get_point();
		//point = point * 10;
		char points[6];
		
		//TimeS = (TimeS + point) ; 
		sprintf(points, "%d", (TimeS + point) * 10);
		lcd_draw_text(2,56, points);
		lcd_draw_text(3,8, "1. RESET GAME");
		lcd_draw_text(4,8, "2. HIGHSCORE");
		lcd_copy();
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
	if(!correct_button()) 
	{
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(2); // again initialize ADC for food in game 
		set_result_ok(0);
		scoreboard();
		snake_init();
		SysTick_Config(SystemCoreClock/1000);	//generate interrupt every millisecond
	}
		
	uint8_t refresh = 0;
	while(1){
		
			uint8_t p = get_pause();
		
			/*moving along the slider*/
			if(p == 1)
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
				else if(slider > 0 && slider > sliderVal)
				{
					move(RIGHT);
				}
				
				//sprintf(time, "%d", TimeS*10);  //points
				//lcd_draw_text(0,0, time);
			}
			
			
			/*button pause is pressed*/
			else if(p == 0 && !get_game_over())
			{
				scoreboard(); 
				refresh = 1;
			}
			
			
			/*button resset is pressed*/
			if(get_reset()) 
			{
				reset_game();
				TimeS = 0;
				set_pause(1);
				snake_init();
				set_reset(0);
			}
			
			/*GAME OVER*/
			if(get_game_over())
			{
				the_end();
			}

	}
}

/*support system clock*/
void SysTick_Handler(){
	if(!get_game_over())
	{
		TimeMS++;
		if(TimeMS == 1000)
		{
			TimeS++;
			TimeMS = 0;
		}	
	}
}


