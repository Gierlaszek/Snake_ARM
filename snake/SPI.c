/**
* @file SPI.c
* @author Kamil Gierlach
* @date December 2020
* @brief File contain definitions for SPI
*/

#include "SPI.h"

/**
*PRIVATE DEFINITION 
*/

#define SCK 0
#define MOSI 7
#define CS 5
#define DC 7
#define RST 11



void SPI_Init(void){
		//enable clock 
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
		SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;
		
		//configure output
		//SPI0
	
		PORTB->PCR[SCK] |= PORT_PCR_MUX(3); 					/*SCK (PTB0) */
		//PTB->PDDR |= (1<<SCK);
	
		PORTA->PCR[MOSI] |= PORT_PCR_MUX(3);					/*MOSI (PTA7) */
		//PTB->PDDR |= (1<<SCK);

		//GPIO
		PORTA->PCR[CS] |= PORT_PCR_MUX(1);						/*CS (PTA5) */
		PTA->PDDR |= (1<<CS); 												/* Set CS pin as output*/
		PTA->PSOR |= (1<<CS);													/*Set active 1 on pin CS*/
	
		PORTB->PCR[DC] |= PORT_PCR_MUX(1); 					/*DC (PTB7)*/
		PTB->PDDR |= (1<<DC);						/*set DC pin as output*/
		PTB->PCOR |= (1<<DC);
	
		PORTB->PCR[RST] |= PORT_PCR_MUX(1); 					/*RST (PTB11)*/
		PTB->PDDR |= (1<<RST);						/*set RST pin as output*/
		PTB->PSOR |= (1<<RST);						/*Set active 1 on pin RST*/
		
		
		
		
		//Config registers, turn on SPI0 as master

		SPI0->C1 |= SPI_C1_SPE_MASK | SPI_C1_MSTR_MASK;
		SPI0->C1 &= ~SPI_C1_CPHA_MASK; 
		
		SPI0->BR = 35;
		
		
}


void SPI_write(uint8_t data){
			
			PTA->PCOR |= (1<<CS); //reset bit
		
			//CS musi byc w spi write 
			//male opoznienie pozwala na wyrysowanie czegokolwiek
	
			while (!(SPI0->S & SPI_S_SPTEF_MASK));
			SPI0->D = data;
			delay_ms(1); //rysuje cos

			PTA->PSOR |= (1<<CS);	//set bit
}

