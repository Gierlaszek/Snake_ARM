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
int naz = 0;


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
	
		const char* znak[] = {" ", " ", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " ", " ", " ", " ", "5", "6", "7"};
		//const char znak[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
	uint8_t	kal_error;
		char display[]={0x20,0x20,0x20,0x20,0x20};
		
		lcd_setup(); //initialize lcd and spi
		buttonsInitialize(); //initialize buttons
		TSI_Init();
	
		//uint8_t slider;

	
	//	lcd_draw_bitmap(logo); //draw to variable logo
	//	lcd_copy(); 					 //send to LCD data
	//	delay_ms(1000);	
		
		lcd_clear();					 //clear variable
		lcd_draw_text(0, 8, "Nickname:");		//draw to variable text 
		char nazwa[] = "__________";
		lcd_draw_text(2,8, nazwa);
		lcd_copy();							//send variable to LCD 
		DELAY(5)

		
		
		kal_error=ADC_Init();				// Inicjalizacja i kalibracja przetwornika A/C
		if(kal_error)
		{	
			while(1);									// Klaibracja sie nie powiodla
		}
		ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12);		// Pierwsze wyzwolenie przetwornika ADC0 w kanale 12 i odblokowanie przerwania
		int a = 0;



		while(1){

			while(correct_button())
			{
					if(wynik_ok){
					int i = (int)wynik /150;
					if(i >= 2){
						char output[10];
						sprintf(output, "%s %s %s %s %s", znak[i-2], znak[i-1], znak[i], znak[i+1], znak[i+2], znak[i+3]); //znak[i+2] zostaje przypisany do nazwy niewiadomo czemu 
						lcd_draw_text(4,16,output);
						lcd_draw_text(5,40, "^");
						
						if(name_button())
						{
							//nazwa[3] = znak[3];
							nazwa[a] = *znak[i];
							lcd_draw_text(2,8,nazwa);
							set_v_name(0);
							a++;
						}
						
						if(incorrect_button())
						{
							nazwa[--a] = '_';
							lcd_draw_text(2,8,nazwa);
							set_v_incorrect(0);
						}
							
					}		
					wynik_ok=0;
				}
				lcd_copy();
			}
			
			lcd_clear();
			lcd_draw_text(0,0,"snake teraz");
			lcd_copy();
				
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