#include "SPI.h"
#include "lcd.h"
#include "tsi.h"
#include "font.h"
#include "bitmap.h"
#include "snake.h"

int main()
{
	
	lcd_setup();
	TSI_Init();
	
	
	//lcd_draw_text(0,0, "*");
	lcd_draw_text(0,0, "time");
	lcd_draw_line_x_or_y(0, 8, 83, 8, 'x');
	//lcd_draw_pixel(0, 0);
	lcd_copy();
	
	while(1)
	{
	
	}
	
}
