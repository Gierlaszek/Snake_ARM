/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    BUTTONS.H
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: Pawel Russek AGH University of Science and Technology
 *			
 * 	BORROWED FILE TO HANDLE THE SNAKE
 *  The changes were made by 
 *	Kamil Gierlach and Karol Piech
 *---------------------------------------------------------------------------*/

#ifndef buttons_h
#define buttons_h

#include <MKL05Z4.h>

void PORTB_IRQHandler(void);
void buttonsInitialize(void);

/* GETTERS */
uint8_t get_reset(void);
uint8_t get_pause(void);
uint8_t get_high_but(void);
uint8_t name_button(void);
uint8_t correct_button(void);
uint8_t incorrect_button(void);

/* SETTERS */
void set_pause(uint8_t p);
void set_reset(uint8_t r);
void set_high_but(uint8_t h);
void set_v_name(uint8_t i);
void set_v_correct(uint8_t i);
void set_v_incorrect(uint8_t i);

#endif
