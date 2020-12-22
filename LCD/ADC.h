#include "MKL05Z4.h"
//----------------------------------------------
// ADC0_CFG1
//----------------------------------------------
// Podzielnik zegara
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03
// Rozdzielczosc (single ended).
#define MODE_8            0x00
#define MODE_12           0x01
#define MODE_10           0x02
//#define MODE_16           0x03

//Zegar taktujacy
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03
//---------------------------------------------
// ADC0_CFG2
//---------------------------------------------
// Dlugosc czasu probkowania
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03
//-----------------------------------------------
//ADCSC2
//-----------------------------------------------
// Napiecie referencyjne
#define REFSEL_EXT         0x00
#define REFSEL_ALT         0x01
#define REFSEL_RES         0x02 
#define REFSEL_RES_EXT     0x03 
//-----------------------------------------------
//ADC0_SC3
//-----------------------------------------------
// Usrednianie
#define AVGS_4             0x00
#define AVGS_8             0x01
#define AVGS_16            0x02
#define AVGS_32            0x03
//************************************************
uint8_t ADC_Init(void);
