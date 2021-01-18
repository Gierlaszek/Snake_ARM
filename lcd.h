#ifndef __LCD__
#define __LCD__
 
#include <stdint.h>
#include "stdbool.h"
 
#define DC			7
#define CS			5
#define RST			11

#define LCD_WIDTH 84
#define LCD_HEIGHT 48
 
void lcd_setup(void);
void delay_ms(int n);
void lcd_reset(void);
void lcd_cmd(uint8_t cmd);

void lcd_del_pixel(int x, int y);
 
void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);
void lcd_draw_pixel(int x, int y);

void lcd_draw_line_x_or_y(int x1, int y1, int x2, int y2, char dir);
 
 
void lcd_copy(void);
 
#endif // __LCD__