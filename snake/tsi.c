/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Tutorial (C).            *
 ******************************************************************************/

/**
 * @file tsi.c
 * @author Koryciak
 * @date Oct 2019
 * @brief File containing definitions.
 * @ver 0.1
 * BORROWED FILE TO HANDLE THE SNAKE
 */

#include "tsi.h"

/******************************************************************************\
* Private definitions
\******************************************************************************/
#define SLIDER_LENGTH     40  /* in mm */
#define TOTAL_ELECTRODE   2
#define ELECTRODE0   			9
#define ELECTRODE1   			8
#define TOUCH0						13	/* PTA13 */
#define TOUCH1						12	/* PTA12 */
#define THRESHOLD0   			100
#define THRESHOLD1   			100
/******************************************************************************\
* Private prototypes
\******************************************************************************/
void change_electrode(void);
void self_calibration(void);
void TSI0_IRQHandler(void);
/******************************************************************************\
* Private memory declarations
\******************************************************************************/
static uint8_t elec_array[TOTAL_ELECTRODE]={ELECTRODE0,ELECTRODE1};
static uint16_t gu16TSICount[TOTAL_ELECTRODE];
static uint16_t gu16Baseline[TOTAL_ELECTRODE];
static uint16_t gu16Threshold[TOTAL_ELECTRODE]={THRESHOLD0,THRESHOLD1};
static uint16_t gu16Delta[TOTAL_ELECTRODE];
static uint8_t ongoing_elec;
static uint8_t end_flag = TRUE;

void TSI_Init (void) {
	
  SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;  /* Enable clock gating for TSI */
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
  PORTA->PCR[TOUCH0] = PORT_PCR_MUX(0);   /* PTA13 as TSI channel 9 */
  PORTB->PCR[TOUCH1] = PORT_PCR_MUX(0);   /* PTB12 as TSI channel 8 */

  TSI0->GENCS |= (TSI_GENCS_ESOR_MASK
                | TSI_GENCS_MODE(0)
                | TSI_GENCS_REFCHRG(4)
                | TSI_GENCS_DVOLT(0)
                | TSI_GENCS_EXTCHRG(7)  
                | TSI_GENCS_PS(4)
                | TSI_GENCS_NSCN(11)
                | TSI_GENCS_TSIIEN_MASK 
                | TSI_GENCS_STPE_MASK
								| TSI_GENCS_TSIEN_MASK);
	
	NVIC_SetPriority(TSI0_IRQn, 3);
  NVIC_ClearPendingIRQ(TSI0_IRQn);
  NVIC_EnableIRQ(TSI0_IRQn);
  self_calibration();
}

uint8_t TSI_ReadSlider (void) {

  int a = 0;
  int b = 0;

	if(end_flag){
		end_flag = FALSE;
		if((gu16Delta[0]>gu16Threshold[0])||(gu16Delta[1]>gu16Threshold[1])){
		  a = (gu16Delta[0]*100)/(gu16Delta[0]+gu16Delta[1]);
		  b = (gu16Delta[1]*100)/(gu16Delta[0]+gu16Delta[1]);
		  return (uint8_t)((100 - a + b)/2);
		}else{
		 return 0;
		}
	}else{
	  return 0;
	}
}
/**
 * @brief Calibration for initialization purpose.
 */
void self_calibration(void) {
	
  unsigned char cnt;
  unsigned char trigger_backup;
  
  TSI0->GENCS |= TSI_GENCS_EOSF_MASK;      /* Clear End of Scan Flag */ 
  TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK;    /* Disable TSI module */
  
  if(TSI0->GENCS & TSI_GENCS_STM_MASK) trigger_backup = TRUE;
  else trigger_backup = FALSE;
  
  TSI0->GENCS &= ~TSI_GENCS_STM_MASK;      /* Use SW trigger */
  TSI0->GENCS &= ~TSI_GENCS_TSIIEN_MASK;   /* Enable TSI interrupts */
  TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;     /* Enable TSI module */
  
	/* Get Counts when Electrode not pressed */
  for(cnt=0; cnt < TOTAL_ELECTRODE; cnt++){ 
    TSI0->DATA = (uint32_t)((elec_array[cnt]<<TSI_DATA_TSICH_SHIFT));
    TSI0->DATA |= TSI_DATA_SWTS_MASK;
    while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
    gu16Baseline[cnt] = (TSI0->DATA & TSI_DATA_TSICNT_MASK); 
  }
  
  TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK;    /* Disable TSI module */
  TSI0->GENCS |= TSI_GENCS_TSIIEN_MASK;    /* Enable TSI interrupt */
  if(trigger_backup) TSI0->GENCS |= TSI_GENCS_STM_MASK; /*Restore trigger mode*/
  else TSI0->GENCS &= ~TSI_GENCS_STM_MASK;
  
  TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;     /* Enable TSI module */
  TSI0->DATA = (uint32_t)((elec_array[0]<<TSI_DATA_TSICH_SHIFT));
  TSI0->DATA |= TSI_DATA_SWTS_MASK;
}
/**
 * @brief Change electrode routine.
 */
void change_electrode(void) {

  int16_t s16temp_delta;
    
  gu16TSICount[ongoing_elec] = /* Save Counts for current electrode */
		(TSI0->DATA & TSI_DATA_TSICNT_MASK);
  s16temp_delta =  /* Obtains Counts Delta from callibration reference */
	  (int16_t)(gu16TSICount[ongoing_elec] - gu16Baseline[ongoing_elec]);
  if( s16temp_delta < 0) gu16Delta[ongoing_elec] = 0;
  else gu16Delta[ongoing_elec] = (uint16_t)s16temp_delta;
    
	if(TOTAL_ELECTRODE > 1){  //Change Electrode to Scan
		if((TOTAL_ELECTRODE-1) > ongoing_elec) ongoing_elec++;
		else ongoing_elec = 0;
		TSI0->DATA = (uint32_t)((elec_array[ongoing_elec]<<TSI_DATA_TSICH_SHIFT));
		TSI0->DATA |= TSI_DATA_SWTS_MASK; 
	}
}
/**
 * @brief Interrupt handler for Touch Slider.
 */
void TSI0_IRQHandler(void) {
	
  end_flag = TRUE;
  TSI0->GENCS |= TSI_GENCS_EOSF_MASK; /* Clear End of Scan Flag */
  change_electrode();
}