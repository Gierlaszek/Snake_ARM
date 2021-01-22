/**
* @file SPI.h
* @author Kamil Gierlach
* @data December 2020
* @brief File contains structures and declarations for SPI
*/

# include "MKL05Z4.h"

#ifndef SPI_H
#define SPI_H

/**
* @brief SPI initialization
*/
void SPI_Init(void);


typedef enum
{
	SPI_CS_Enable = 0,
	SPI_CS_Disable,
}spi_cs_t;


/**
* @brief SPI write and read operation
*/
void SPI_write(uint8_t data);
void SPI0_IRQHeader(void);

/**
* @brief slave select CS
*/
void SPI_write_cs(uint32_t pin, spi_cs_t st);
void SPI_init_cs(uint32_t pin);
void reset(void);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t* data, int size);
void delay_ms(int n);
void lcd_reset(void);

#endif
