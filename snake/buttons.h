#ifndef buttons_h
#define buttons_h

#include <MKL05Z4.h>


void PORTB_IRQHandler(void);
void buttonsInitialize(void);
void set_pause(uint8_t p);

#endif
