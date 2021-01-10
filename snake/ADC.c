#include "ADC.h"

uint8_t ADC_Init(void)
{
	uint16_t kalib_temp;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;          // Dołączenie sygnału zegara do ADC0
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;					// Dołączenie sygnału zegara do portu B
	PORTB->PCR[2] &= ~(1<<2);										//PTB2 - wejście analogowe, kanał 4
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_4) | ADC_CFG1_ADLSMP_MASK;	// Zegar wejściowy BUS/2=10.49MHz, zegar ADCK równy 2.62MHz (2621440Hz), długi czas prókowania
	ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;										// Włącz wspomaganie zegara o dużej częstotliwości
	ADC0->SC3  = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);		// Włącz uśrednianie na 32 próbki, domyślnie wyzwalanie na żądanie
	ADC0->SC3 |= ADC_SC3_CAL_MASK;											// Rozpoczęcie kalibracji
	while(ADC0->SC3 & ADC_SC3_CAL_MASK);								// Czekaj na koniec kalibracji
	
	if(ADC0->SC3 & ADC_SC3_CALF_MASK)
{
	  ADC0->SC3 |= ADC_SC3_CALF_MASK;
	  return(1);																				// Wróć, jeśli błąd kalibracji
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
	kalib_temp |= 0x8000;                       // Ustaw najbardziej znaczący bit na 1
	ADC0->PG = ADC_PG_PG(kalib_temp);           // Zapisz w  "plus-side gain calibration register"
	ADC0->OFS = 1;													// Klaibracja przesunięcia zera (z pomiaru swojego punktu odniesienia - masy)
	ADC0->SC1[0] = ADC_SC1_ADCH(31);						// Zablokuj przetwornik ADC0
	ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK;					// Włącz tryb szybkiej konwersji
	ADC0->CFG1 = ADC_CFG1_ADICLK(ADICLK_BUS_2) | ADC_CFG1_ADIV(ADIV_1) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(MODE_12);	// Zegar ADCK równy 10.49MHz, rozdzielczość 12 bitów, długi czas prókowania
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
	return(0);																	// Wróć, jeśli wszystko w porządku
}
