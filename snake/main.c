#include "SPI.h"
#include "lcd.h"
#include "snake.h"
#include "font.h"
#include "bitmap.h"
#include "tsi.h"
#include "stdbool.h"
#include "ADC.h"
#include "buttons.h"

#define sliderVal 50

#define LEFT 1
#define RIGHT 2

void SysTick_Handler(void);


/*snake timer*/
static volatile unsigned int TimeMS = 0;
static volatile unsigned int TimeS = 0;
static volatile unsigned int TimeM = 0;
char time[] = {0x20, 0x20, 0x20};


uint8_t slider;

int main(){
	
	ADC_Init();
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(4);	
	
	SysTick_Config(SystemCoreClock/1000);	//generate interrupt every millisecond

	
	TSI_Init();
	lcd_setup();
	lcd_clear();
	
	
	buttonsInitialize();
	scoreboard();
	snake_init();
	
	int i =0;
	while(1){
		
			//lcd_draw_text(0,0, time);
		
			
			bool p = get_pause();
			if(p == 1)
			{
				slider = TSI_ReadSlider();
				if(slider == 0)
				{
					snake_move();
				}
				else if(slider > 0 && slider < sliderVal)
				{
					move(LEFT);
				}
				else if(slider > 0 && slider > sliderVal)
				{
					move(RIGHT);
				}
				i++;
			}
			
			
			
			
			//sprawdzenie czy dziala stopowanie gry  --- dziala 
			/*
			if(i > 5)
			{
				set_pause(0);
				scoreboard();
				draw_snake();
				DELAY(100)
				set_pause(1);
				scoreboard();
				i = 0;
			}
			*/
			//sprawdzenie czy dziala reset  --- dziala 
			/*
			if(i > 8)
			{
				reset_game();
				snake_init();
				i = 0;
			}
			*/
			//snake_time();//uzycie tego powoduje ze snake sie nie porusza 
			// zapytac o generacje liczb pseudolosowych - ADC 

		
		
	}
}

void SysTick_Handler(){
	TimeMS++;
	if(TimeMS == 1000)
	{
		if(TimeS == 59)
		{
			TimeM++;
			TimeS = 0;
		}
		TimeS++;
		TimeMS = 0;
	}	
}

void snake_time(){
	if(TimeM < 10)
		{
			if(TimeS < 10)
			{
				sprintf(time, "0%d:0%d", TimeM, TimeS); 
			}
			else if(TimeS >= 10)
			{
				sprintf(time, "0%d:%d", TimeM, TimeS); 
			}
		}
		else if(TimeM > 10)
		{
			if(TimeS < 10)
			{
				sprintf(time, "%d:0%d", TimeM, TimeS); 
			}
			else if(TimeS >= 10)
			{
				sprintf(time, "%d:%d", TimeM, TimeS); 
			}
		}
		//lcd_draw_text(0,0, time);
		//lcd_copy();
}


