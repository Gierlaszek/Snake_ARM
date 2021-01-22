/*******************************************************************
* 	@file LCD.h
*		@author Kamil Gierlach, Karol Piech
*		@date 15.12.2020
*   @ver 1.2
* 	@brief File containg declarations and structure for LCD
**********************************************************************/

#ifndef __LCD__
#define __LCD__
 
#include <stdint.h>
#include "stdbool.h"
 
#define DC			7
#define CS			5
#define RST			11

#define LCD_WIDTH 84
#define LCD_HEIGHT 48
 
void delay_ms(int n);

//Initialize function responsible for display configuration
void lcd_setup(void);
void lcd_reset(void);
void lcd_cmd(uint8_t cmd);

//Initialize funtcion responsible for sending and clear data 
void lcd_copy(void);
void lcd_clear(void);

//Initialize function responsible for drawing on the LCD 
void lcd_del_pixel(int x, int y); 
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);
void lcd_draw_pixel(int x, int y);
void lcd_draw_line_x_or_y(int x1, int y1, int x2, int y2, char dir);
 
#endif // __LCD__