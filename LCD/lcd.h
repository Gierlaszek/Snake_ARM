#ifndef __LCD__
#define __LCD__
 
#include <stdint.h>
 
#define DC			7
#define CS			5
#define RST			11
 
void lcd_setup(void);
void delay_ms(int n);
void lcd_reset(void);
void lcd_cmd(uint8_t cmd);
 
void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);
 
void lcd_copy(void);
 
#endif // __LCD__