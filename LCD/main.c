/**
* 
*/

#include "lcd.h"
#include "bitmap.h"
#include "buttons.h"
#include "tsi.h"
#include "ADC.h"
#include "string.h"

#define sliderVal 50

float volt_coeff = ((float)(((float)2.91) / 4095) );		
uint8_t wynik_ok=0;
uint16_t temp;
float	wynik;

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];	// Odczyt danej i skasowanie flagi COCO
	if(!wynik_ok)				// Sprawdz, czy wynik skonsumowany przez petle glówna
	{
		wynik = temp;			// Wyslij nowa dana do petli glównej
		wynik_ok=1;
	}
	NVIC_EnableIRQ(ADC0_IRQn);
}

int main(){
	
		char* znak[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7"};
		uint8_t	kal_error;
		char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
		
		lcd_setup(); //initialize lcd and spi
		buttonsInitialize(); //initialize buttons
		TSI_Init();
	
		//uint8_t slider;

	
	//	lcd_draw_bitmap(logo); //draw to variable logo
	//	lcd_copy(); 					 //send to LCD data
	//	delay_ms(1000);	
		
		lcd_clear();					 //clear variable
		lcd_draw_text(0, 8, "Nickname:");		//draw to variable text
		lcd_draw_text(2,8, "__________");
		lcd_copy();							//send variable to LCD 
		DELAY(5)
		
		kal_error=ADC_Init();				// Inicjalizacja i kalibracja przetwornika A/C
		if(kal_error)
		{	
			while(1);									// Klaibracja sie nie powiodla
		}
		
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12);		// Pierwsze wyzwolenie przetwornika ADC0 w kanale 12 i odblokowanie przerwania
	

		while(1){
			
			if(wynik_ok){
				int i = (int)wynik /150;
				char output[5];
				char nazwa[5];
				if(i >= 2){
					sprintf(output, "%s %s %s %s %s", znak[i-2], znak[i-1], znak[i], znak[i+1], znak[i+2]);
					//jesli przycisniety button to zapisuje sie znak[i] do zmiennej nazwa, wybierajac zamieniamy _ na litere 
					lcd_draw_text(4,16,output);
					lcd_draw_text(5,40, "^");
				}				
				lcd_copy();
				wynik_ok=0;
			}
			
							
		}
}

		/**
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
		*/