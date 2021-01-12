#ifndef buttons_h
#define buttons_h

#include <MKL05Z4.h>


void PORTB_IRQHandler(void);
void buttonsInitialize(void);
uint8_t name_button(void);
void set_v_name(uint8_t i);
uint8_t correct_button(void);
void set_v_correct(uint8_t i);
uint8_t incorrect_button(void);
void set_v_incorrect(uint8_t i);

#endif

