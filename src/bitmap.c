/*
 * bitmap.c

 *
 *  Created on: 06.05.2020
 *      Author: mszoz
 */
#include "bitmap.h"
const uint8_t snake[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x30, 0x18, 0x0C, 0x0E, 0x07, 0x07, 0x65, 0x94,
	0xF4, 0xF4, 0xF4, 0xF8, 0xE4, 0xE4, 0xE5, 0x67, 0x06, 0x06, 0x06, 0x04, 0x08, 0x88, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7E, 0xFE, 0x9E, 0x07, 0x03, 0x02, 0x02, 0xCE, 0xCE,
	0xCE, 0xCE, 0xC7, 0xC1, 0x03, 0x80, 0x80, 0xC3, 0xC3, 0xC7, 0xC7, 0x0E, 0x0E, 0x0F, 0x03, 0x03,
	0x07, 0x0F, 0xFF, 0xFF, 0x06, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x31, 0xF7, 0xFE, 0x3C,
	0x1C, 0x1C, 0x19, 0x19, 0x19, 0x19, 0x11, 0xF1, 0xF0, 0x71, 0x31, 0xB1, 0xB1, 0xF1, 0xF1, 0xF8,
	0xFC, 0xFC, 0x1E, 0x7E, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0E,
	0x07, 0x07, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0x3E, 0x3F, 0x03, 0xE3, 0xE0, 0xE7,
	0xF7, 0xFF, 0xFF, 0xFF, 0x3F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00, 0xC0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7E, 0xFF, 0xFF, 0xE3, 0xE1, 0x80, 0x80,
	0x00, 0x03, 0x03, 0x67, 0x67, 0xFF, 0xFF, 0xF9, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x20, 0x30,
	0x70, 0x70, 0x78, 0x78, 0xF8, 0xFC, 0x7C, 0x7C, 0x7E, 0x9E, 0xBE, 0x9E, 0xC7, 0xCF, 0xDF, 0xDF,
	0xDF, 0xDF, 0xDF, 0xDF, 0x8E, 0x8E, 0xBE, 0xFE, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0xC0,
	0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0xFA, 0x7F, 0x1F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x03, 0x07, 0x07, 0x07, 0x3E, 0x3C, 0x3C, 0x3C, 0x3C, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39,
	0x39, 0x39, 0x30, 0x38, 0x38, 0x38, 0x2C, 0x2E, 0x0E, 0x06, 0x06, 0x03, 0x03, 0x03, 0x01, 0x01,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x06, 0x06, 0x2C,
	0x38, 0x38, 0x38, 0x38, 0x39, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x2F, 0x2F, 0x27, 0x27, 0x03,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
