/*
 * OLED.c
 *
 *  Created on: Aug 19, 2024
 *      Author: musta
 */

#include "OLED.h"
#include "string.h"
#include "Fonts.h"
#include "I2C.h"

static uint8_t screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

void SSD1306_WRITECOMMAND(uint8_t command) {

	I2C_Start();
	I2C_Address(SSD1306_ADDRESS << 1);
	I2C_Write(SSD1306_COMMAND);
	I2C_Write(command);
	I2C_Stop();
}

void SSD1306_WRITEDATA(uint8_t dataByte) {

	I2C_Start();
	I2C_Address(SSD1306_ADDRESS << 1);
	I2C_Write(SSD1306_DATA);
	I2C_Write(dataByte);
	I2C_Stop();
}

void OLED_Init() {

	SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); //
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
}

void SSD1306_Test(void) {

	uint8_t x, y;

	// Clear display buffer
	for (y = 0; y < 8; y++) {
		SSD1306_WRITECOMMAND(0xB0 + y); // Set page address
		SSD1306_WRITECOMMAND(0x00); // Set lower column address
		SSD1306_WRITECOMMAND(0x10); // Set higher column address

		for (x = 0; x < 128; x++) {
			SSD1306_WRITEDATA(0xFF); // Write data to fill the entire display
		}
	}
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {

	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		// Koordinatlar ekran sınırlarını aşıyorsa, çık
		return;
	}

	// Hafızada hangi byte'ın güncelleneceğini ve hangi bitin değişeceğini hesapla
	uint16_t byteIndex = (x + (y / 8) * SCREEN_WIDTH);
	uint8_t bitIndex = y % 8;

	if (color) {
		// Pikseli aç
		screenBuffer[byteIndex] |= (1 << bitIndex);
	} else {
		// Pikseli kapat
		screenBuffer[byteIndex] &= ~(1 << bitIndex);
	}
}

void SSD1306_DrawChar(uint8_t x, uint8_t y, char c) {
	uint32_t i, b, j;

	for (i = 0; i < 10; i++) {
		b = font7x10[(c - 32) * 10 + i];

		for (j = 0; j < 7; j++) {
			if ((b << j) & 0x8000) {  // Bit maskesi kullanarak kontrol
				SSD1306_DrawPixel(x + j, y + i, 1);  // Pixel açık
			} else {
				SSD1306_DrawPixel(x + j, y + i, 0);  // Pixel kapalı
			}
		}
	}

	// Karakterler arasında boşluk bırakmak (isteğe bağlı)
	for (uint8_t i = 0; i < 10; i++) {
		SSD1306_DrawPixel(x + 7, y + i, 0); // Karakter sonrası boşluk
	}

}

void SSD1306_DrawString(uint8_t x, uint8_t y, const char *str) {

	while (*str) {
		SSD1306_DrawChar(x, y, *str++);
		x += 8;  // Her karakterin genişliği 7 piksel ve 1 piksel boşluk
	}
}

void SSD1306_UpdateScreen(void) {

	for (uint8_t page = 0; page < 8; page++) {
		SSD1306_WRITECOMMAND(0xB0 + page);
		SSD1306_WRITECOMMAND(0x00);
		SSD1306_WRITECOMMAND(0x10);

		for (uint8_t col = 0; col < SCREEN_WIDTH; col++) {
			SSD1306_WRITEDATA(screenBuffer[page * SCREEN_WIDTH + col]);
		}
	}
}

void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color){

    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(int16_t j=0; j<h; j++, y++)
    {
        for(int16_t i=0; i<w; i++)
        {
            if(i & 7)
            {
               byte <<= 1;
            }
            else
            {
               byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }
            if(byte & 0x80) SSD1306_DrawPixel(x+i, y, color);
        }
    }
}

void SSD1306_Fill(uint8_t color) {
	/* Set memory */
	memset(screenBuffer, (color == 0) ? 0x00 : 0xFF, sizeof(screenBuffer));
}

void SSD1306_Clear(void) {
	SSD1306_Fill(0);
//    SSD1306_UpdateScreen();
}
