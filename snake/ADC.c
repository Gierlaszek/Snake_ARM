#include "ADC.h"
#include "buttons.h"

uint8_t ADC_Init(void)
{
	uint16_t kalib_temp;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;          // Dolaczenie sygnalu zegara do ADC0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;	// Dolaczenie syganlu zegara do portu A
	PORTA->PCR[0] &= ~(PORT_PCR_MUX(7));		//PTA0 - wejscie analogowe, kanal 12
	PORTA->PCR[9] &= ~(PORT_PCR_MUX(0));	//PTA9 - wejscie analogowe, kanal 2
	
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_4) | ADC_CFG1_ADLSMP_MASK;	// Zegar ADCK równy 2.62MHz (2621440Hz)
	ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;		// Wlacz wspomaganie zegara o duzej czestotliwosci
	ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);		// Wlacz usrednianie na 32
	//ADC0->SC2 = ADC_SC2_REFSEL(REFSEL_EXT);		// Zrodlo referencyjne zewnetrzne
	
	ADC0->SC3 |= ADC_SC3_CAL_MASK;				// Rozpoczecie kalibracji
	while(ADC0->SC3 & ADC_SC3_CAL_MASK);		// Czekaj na koniec kalibracji
	
	if(ADC0->SC3 & ADC_SC3_CALF_MASK)
{
	  ADC0->SC3 |= ADC_SC3_CALF_MASK;
	  return(1);								// Wróc, jesli blad kalibracji
	}
	
	kalib_temp = 0x00;
	kalib_temp += ADC0->CLP0;
	kalib_temp += ADC0->CLP1;
	kalib_temp += ADC0->CLP2;
	kalib_temp += ADC0->CLP3;
	kalib_temp += ADC0->CLP4;
	kalib_temp += ADC0->CLPS;
	kalib_temp += ADC0->CLPD;
	kalib_temp /= 2;
	kalib_temp |= 0x8000;                       // Ustaw najbardziej zanaczacy bit na 1
	ADC0->PG = ADC_PG_PG(kalib_temp);           // Zapisz w  "plus-side gain calibration register"
	ADC0->OFS = 0;														// Klaibracja przesuniecia zera (z pomiaru swojego punktu odniesienia - masy)
	ADC0->SC1[0] = ADC_SC1_ADCH(31);
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_1) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(MODE_12);	// Zegar ADCK równy 10.49MHz (10485760Hz), rozdzielczosc 12 bitów, dlugi czas próbkowania
	ADC0->SC3 |= ADC_SC3_ADCO_MASK;							//Przetwarzanie ciagle
	//ADC0->SC2 |= ADC_SC2_ADTRG_MASK;						// Wlaczenie wyzwalania sprzetowego
	//SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK | SIM_SOPT7_ADC0TRGSEL(4);		// Wyzwalanie ADC0 przez PIT0
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
	
	return(0);
}