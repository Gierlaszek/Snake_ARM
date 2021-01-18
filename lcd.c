#include "SPI.h"
#include "string.h"
#include "lcd.h"
#include "font.h"
#include "stdbool.h"

#define PCD8544_FUNCTION_SET		0x20
#define PCD8544_DISP_CONTROL		0x08
#define PCD8544_DISP_NORMAL			0x0c
#define PCD8544_SET_Y				0x40
#define PCD8544_SET_X				0x80
#define PCD8544_H_TC				0x04
#define PCD8544_H_BIAS				0x10
#define PCD8544_H_VOP				0x80
 
#define LCD_BUFFER_SIZE			(LCD_HEIGHT * LCD_WIDTH / 8)

uint8_t lcd_buffer[LCD_BUFFER_SIZE];

void delay_ms( int n) {
	volatile int i;
	volatile int j;
	for( i = 0 ; i < n; i++)
	for(j = 0; j < 3500; j++) {}
}

//function to setup LCD and SPI 
void lcd_setup(){
		
		SPI_Init();
		lcd_reset();
	
		lcd_cmd(0x21);
		lcd_cmd(0x13); //0x13 - kontrast 
		lcd_cmd(0x04);
		lcd_cmd(0x14);
		lcd_cmd(0x80 | 0xb1); 
		lcd_cmd(0x20); //
		lcd_cmd(0x0c);
		
}

//function to reset LCD 
void lcd_reset(){
		PTB->PCOR |= (1<<RST); //reset bit
		delay_ms(500);
		PTB->PSOR |= (1<<RST); //set bit
}

void lcd_draw_bitmap(const uint8_t* data){
		memcpy(lcd_buffer, data, LCD_BUFFER_SIZE);
}

void lcd_clear(){
		memset(lcd_buffer, 0, LCD_BUFFER_SIZE);	
}


void lcd_draw_text(int row, int col, const char* text)
{
	int i;
	uint8_t* pbuf = &lcd_buffer[row * 84 + col];
	while ((*text) && (pbuf < &lcd_buffer[LCD_BUFFER_SIZE - 6])) {
		int ch = *text++;
		const uint8_t* font = &font_ASCII[ch - ' '][0];
		for (i = 0; i < 5; i++) {
			*pbuf++ = *font++;
		}
		*pbuf++ = 0;
	}
}

void lcd_draw_pixel(int x, int y){
	lcd_buffer[ x + (y >> 3) * LCD_WIDTH] |= 1 << (y & 7);
}


void lcd_del_pixel(int x, int y){
	lcd_buffer[ x + (y >> 3) * LCD_WIDTH] &=~ 1 << (y & 7);
}


// draw line in direction x or y
void lcd_draw_line_x_or_y(int x1, int y1, int x2, int y2, char dir){
	int dx = x2 - x1;
	int dy = y2 - y1;
	if(dir == 'x'){
		for(int i = 0; i < dx; i++)
		{
			lcd_draw_pixel(x1+i, y1);
		}
	}
	else if(dir == 'y'){
		for(int i = 0; i < dy; i++)
		{
			lcd_draw_pixel(x1, y1+i);
		}
	}
}

void lcd_cmd(uint8_t cmd)
{
		PTB->PCOR |= (1<<DC);	//reset bit
		SPI_write(cmd);
}

void lcd_copy(void)
{
		int i;
		PTB->PSOR |= (1<<DC); //set bit 
		for (i = 0; i < LCD_BUFFER_SIZE; i++)
			SPI_write(lcd_buffer[i]);
}
