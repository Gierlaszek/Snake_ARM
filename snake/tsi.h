/******************************************************************************
 * This file is a part of the Sysytem Microprocessor Tutorial (C).            *
 ******************************************************************************/

/**
 * @file tsi.h
 * @author Koryciak
 * @date Oct 2019
 * @brief File containing enums, structures and declarations.
 * @ver 0.1
 * BORROWED FILE TO HANDLE THE SNAKE
 */

#ifndef TSI_H
#define TSI_H

#include "MKL05Z4.h"

#ifndef TRUE
# define TRUE        (1)
#endif

#ifndef	FALSE
# define FALSE       (0)
#endif

#define MASK(x)		 (uint32_t)(1UL << (x))    /* turn bit number into 32b mask */
#define DELAY(x)   for(uint32_t i=0;i<(x*10000);i++)__nop(); 					/* wait */

/**
 * @brief Touch slider initialization.
 */
void TSI_Init(void);
/**
 * @brief Return value read from the slider (0,100).
 */
uint8_t TSI_ReadSlider (void);

#endif /* TSI_H */
