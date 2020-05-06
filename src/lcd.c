/*
 * lcd.c
 *
 *  Created on: 16.04.2020
 *      Author: mszoz
 */
#include <string.h>
#include "lcd.h"
#include "font.h"
#include "bitmap.h"
#include <stdlib.h>
#include <stdbool.h>
#define PCD8544_FUNCTION_SET 0x20
#define PCD8544_DISP_CONTROL 0x08
#define PCD8544_DISP_NORMAL 0x0c
#define PCD8544_SET_Y 0x40
#define PCD8544_SET_X 0x80
#define PCD8544_H_TC 0x04
#define PCD8544_H_BIAS 0x10
#define PCD8544_H_VOP 0x80

#define LCD_BUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT / 8)

SPI_HandleTypeDef spi;

static uint8_t lcd_buffer[LCD_BUFFER_SIZE];

void lcd_cmd(uint8_t cmd){
	HAL_GPIO_WritePin(GPIOC, LCD_CE|LCD_DC, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, LCD_CE|LCD_DC, GPIO_PIN_SET);
}

void lcd_setup(void){
	HAL_GPIO_WritePin(GPIOC, LCD_RST, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LCD_RST, GPIO_PIN_SET);

	lcd_cmd(PCD8544_FUNCTION_SET | 1);
	lcd_cmd(PCD8544_H_BIAS | 4);
	lcd_cmd(PCD8544_H_VOP | 0x3a);
	lcd_cmd(PCD8544_FUNCTION_SET);
	lcd_cmd(PCD8544_DISP_NORMAL);
}

void lcd_clear(void){
	memset(lcd_buffer, 0, LCD_BUFFER_SIZE);
}

void lcd_draw_bitmap(const uint8_t* data){
	memcpy(lcd_buffer, data, LCD_BUFFER_SIZE);
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

inline void lcd_draw_pixel(int x, int y){
	lcd_buffer[ x + (y >> 3) * LCD_WIDTH] |= 1 << (y & 7);
}

void lcd_draw_line(int x1, int y1, int x2, int y2){
	int dx, dy, sx, sy;
	if (x2>=x1){
		dx=x2-x1;
		sx=1;
	}else{
		dx=x1-x2;
		sx=-1;
	}
	if(y2>=y1){
		dy=y1-y2;
		sy=1;
	}else{
		dy=y2-y1;
		sy=-1;
	}

	int dx2=dx<<1;
	int dy2=dy<<1;
    int err=dx2+dy2;
    while(1){
        lcd_draw_pixel(x1,y1);
        if (err>=dy){
            if (x1==x2) break;
            err+=dy2;
            x1+=sx;
        }
        if (err<=dx){
            if (y1==y2) break;
            err+=dx2;
            y1+=sy;
        }
    }
}

void lcd_copy(void){
	 HAL_GPIO_WritePin(GPIOC, LCD_DC, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOC, LCD_CE, GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&spi, lcd_buffer, LCD_BUFFER_SIZE, HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(GPIOC, LCD_CE, GPIO_PIN_SET);
}

void lcd_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	lcd_draw_line(x1, y1, x2, y1);
	lcd_draw_line(x1, y1, x1, y2);
	lcd_draw_line(x2, y1, x2, y2);
	lcd_draw_line(x1, y2, x2, y2);
}

void draw_border_line(){
	lcd_draw_line(0, 0, LCD_WIDTH - 1, 0);
	lcd_draw_line(LCD_WIDTH - 1, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	lcd_draw_line(LCD_WIDTH - 1, LCD_HEIGHT - 1, 0, LCD_HEIGHT - 1);
	lcd_draw_line(0, LCD_HEIGHT - 1, 0, 0);
}

void lcd_draw_circle(uint8_t x1, uint8_t y1, uint8_t r){
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	lcd_draw_pixel(x1, y1 + r);
	lcd_draw_pixel(x1, y1 - r);
	lcd_draw_pixel(x1 + r, y1);
	lcd_draw_pixel(x1 - r, y1);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
	    }
		x++;
		ddF_x += 2;
		f += ddF_x;

		lcd_draw_pixel(x1 + x, y1 + y);
		lcd_draw_pixel(x1 - x, y1 + y);
		lcd_draw_pixel(x1 + x, y1 - y);
		lcd_draw_pixel(x1 - x, y1 - y);
		lcd_draw_pixel(x1 + y, y1 + x);
		lcd_draw_pixel(x1 - y, y1 + x);
		lcd_draw_pixel(x1 + y, y1 - x);
		lcd_draw_pixel(x1 - y, y1 - x);
	}
}

void lcd_rectangle_black(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	uint8_t length=abs(y2-y1);
	uint8_t min;
	if(y2>y1){
		min=y1;
	}else{
		min=y2;
	}
	for(int i=0;i<=length;i++){
		lcd_draw_line(x1,min+i,x2,min+i);
	}
}

void draw_start_screen(){
	lcd_clear();
	lcd_draw_bitmap(snake);
	lcd_draw_text(0, 35, "Press ->");
	lcd_draw_text(1,40,"to play");
	lcd_copy();
}
bool exceed(uint8_t x, uint8_t y){
	bool exceeded;
	if((x>75 || x<0) || (y>=45 || y<0)){
		exceeded=true;
	}else{
		exceeded=false;
	}
	return exceeded;
}
