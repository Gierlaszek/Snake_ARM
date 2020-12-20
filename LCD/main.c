/**
* 
*/

#include "lcd.h"
#include "bitmap.h"
#include "buttons.h"
#include "tsi.h"

#define sliderVal 50


int main(){
		
		lcd_setup(); //initialize lcd and spi
		buttonsInitialize(); //initialize buttons
		TSI_Init();
	
		uint8_t slider;

	
		lcd_draw_bitmap(logo); //draw to variable logo
		lcd_copy(); 					 //send to LCD data
		delay_ms(1000);	
		
		lcd_clear();					 //clear variable
		for (int i = 0; i < 6; i++)	
			lcd_draw_text(i, i*8, "Fobot !");		//draw to variable text
		lcd_copy();							//send variable to LCD 
	
				//buttonsInitialize();

		while(1){
			
	
			slider = TSI_ReadSlider();
			if(slider < sliderVal){
				lcd_clear();
				lcd_draw_text(2, 2*8, "Prawo");
				lcd_copy();
			}
			else if(slider > sliderVal){
				lcd_clear();
				lcd_draw_text(2, 2*8, "Lewo");
				lcd_copy();
			}

			
		}
}
