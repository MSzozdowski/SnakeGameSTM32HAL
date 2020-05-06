/*
 * lcd.h
 *
 *  Created on: 16.04.2020
 *      Author: mszoz
 */

#ifndef LCD_H_
#define LCD_H_
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_DC GPIO_PIN_1
#define LCD_CE GPIO_PIN_2
#define LCD_RST GPIO_PIN_3

extern SPI_HandleTypeDef spi;

void lcd_setup(void);
void draw_border_line(void);
void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);
void lcd_copy(void);
void lcd_draw_line(int x1, int y1, int x2, int y2);
void lcd_draw_pixel(int x, int y);
void lcd_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void lcd_draw_circle(uint8_t x1, uint8_t y1, uint8_t r);
void lcd_rectangle_black(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void draw_start_screen(void);
bool exceed(uint8_t x, uint8_t y);
#endif /* LCD_H_ */
