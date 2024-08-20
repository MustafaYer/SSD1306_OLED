/*
 * OLED.h
 *
 *  Created on: Aug 19, 2024
 *      Author: mustafa
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "stm32f4xx.h"

#define SSD1306_ADDRESS 0x3C // OLED I2C adresi
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7

#define I2C_TIMEOUT 10000 // Zaman aşımı süresi

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64


void SSD1306_WRITECOMMAND(uint8_t command);
void SSD1306_WRITEDATA(uint8_t dataByte);
void OLED_Init();
void SSD1306_Test(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c);
void SSD1306_DrawString(uint8_t x, uint8_t y, const char *str);
void SSD1306_UpdateScreen(void);
void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);
void SSD1306_Fill(uint8_t color);
void SSD1306_Clear(void);

#endif /* INC_OLED_H_ */
