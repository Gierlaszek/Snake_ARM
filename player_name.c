
#include "player_name.h"
#include "ADC.h"
//float correct_ADC = ((float)(((float)2.91) / 4095) );		
//uint8_t wynik_ok=0;
//uint16_t temp;
float	adc;
int naz = 0;
int a = 0;
const char* znak[] = {" ", " ", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " ", " ", " ", " ", "5", "6", "7"};
char name[] = "__________";

	
uint8_t result_ok = 0;
uint16_t temp;
float	result_ADC;

	
float get_resultADC()
{
	return result_ADC;
}

uint8_t get_result_ok()
{
	return result_ok;
}

void set_result_ok(uint8_t r)
{
	result_ok = r;
}
	
void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];										// Odczyt danej i skasowanie flagi COCO
	if(!result_ok)													// Sprawdz, czy wynik skonsumowany przez petle glówna
	{
		result_ADC = temp;												// Wyslij nowa dana do petli glównej
		result_ok=1;
	}
	
	if(correct_button())
	{
		ADC0->SC1[0] |= ADC_SC1_ADCH(12);	
	}
	else if(!correct_button())
	{
		ADC0->SC1[0] |= ADC_SC1_ADCH(2);	
	}
	NVIC_EnableIRQ(ADC0_IRQn);
}
	
void player_name_init()
{
	uint8_t	kal_error;
		
	kal_error=ADC_Init();				// Inicjalizacja i kalibracja przetwornika A/C
	if(kal_error)
	{	
		while(1);									// Klaibracja sie nie powiodla
	}
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12);		// Pierwsze wyzwolenie przetwornika ADC0 w kanale 12 i odblokowanie przerwania
}

void get_name(char *out_name)
{
	for(uint8_t i = 0; i < sizeof(name); i ++ )
	{
		out_name[i] = name[i];
	}
}

void set_player_name()
{
	lcd_clear();					 //clear variable
	lcd_draw_text(0, 8, "Nickname:");		//draw to variable text 
	lcd_draw_text(2,8, name);
	lcd_copy();	
	
	while(correct_button())
			{
				
					uint8_t result = get_result_ok();
					
					if(result_ok){
					adc = get_resultADC();
					int i = (int)adc /150;
					if(i >= 2){
						char output[10];
						sprintf(output, "%s %s %s %s %s", znak[i-2], znak[i-1], znak[i], znak[i+1], znak[i+2], znak[i+3]); //znak[i+2] zostaje przypisany do nazwy niewiadomo czemu 
						lcd_draw_text(4,16,output);
						lcd_draw_text(5,40, "^");
						
						if(name_button())
						{
							name[a] = *znak[i];
							lcd_draw_text(2,8,name);
							set_v_name(0);
							a++;
						}
						
						if(incorrect_button())
						{
							name[--a] = '_';
							lcd_draw_text(2,8,name);
							set_v_incorrect(0);
						}
							
					}		
					set_result_ok(0);
				}
				lcd_copy();
			}
}
