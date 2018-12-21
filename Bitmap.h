#ifndef Bitmap_h
#define Bitmap_h

#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <SPI.h>
#include <SD.h>

void bmpDraw(const char *filename, uint8_t x, uint16_t y, Adafruit_ILI9341 tft);
uint16_t read16(File &f);
uint32_t read32(File &f);

#endif //Bitmap_h

