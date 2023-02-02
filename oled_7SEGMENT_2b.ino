// EXAMPLE 7 SEGMENT DISPLAY RENDERING FOR ADAFRUIT_GFX & SSD1306
// Copyright 2018 T&A Laboratories LLC
// writen by: Aaron Williams on 9/7/2018
// https://ta-laboratories.com/blog/2018/09/07/recreating-a-7-segment-display-with-adafruit-gfx-ssd1306-oled/
// changes by Nicu FLORICA (niq_ro) for more feature (big characters, decimal point)
// added more characters (degree, o, C, -);

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// define segment truth table for each digit
/*
 *      --a-- 
 *     |     | 
 *     f     b
 *     |     |
 *      --g--
 *     |     |
 *     e     c
 *     |     |
 *      --d--  dp
 *   
 */

static const int digit_array[14][7] = {
 //a, b, c, d, e, f, g 
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1},  // 9
  {1, 1, 0, 0, 0, 1, 1},  // degree
  {0, 0, 1, 1, 1, 0, 1},  // o
  {1, 0, 0, 1, 1, 1, 0},  // C
  {0, 0, 0, 0, 0, 0, 1}   // - 
};


uint8_t n1 = 38;
uint8_t n3 = 8;
uint8_t n2 = 51;
uint8_t r = 4;

void setup()   {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.setRotation(1); // now canvas is 11x21
  display.clearDisplay();   // Clear the buffer.
  // func params: render_digits(pox_x, pos_y, spacing, color)
  //render_digits(0, 0, 0, 999, WHITE);
  display.drawRect(0,0,SCREEN_HEIGHT-1,SCREEN_WIDTH-1, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();   // Clear the buffer.
}

void loop() {
 //display.fillCircle (SCREEN_HEIGHT-1-r/2,SCREEN_WIDTH-1-r/2,r,WHITE);
 //display.display();

  // count up to 9
  for (int cnt = 0; cnt < 14; cnt++) {
    display.clearDisplay();   // Clear the buffer.
    render_digit(0, 0, cnt, WHITE);
    if (cnt%2 == 0) 
      display.fillCircle (SCREEN_HEIGHT-3-r/2,SCREEN_WIDTH-3-r/2,r+1,WHITE);
    display.display();
    delay(1000);
  }
  
}  // end main loop


// RENDER DIGIT
// don't use this unless you only need a single digit
// use render_digits() func above instead
void render_digit(uint8_t pos_x, uint8_t pos_y,
                  uint8_t digit, uint8_t color) {
  // loop through 7 segments
  for (uint8_t i = 0; i < 7; i++) {
    bool seg_on = digit_array[digit][i];
    // if seg_on is true draw segment
    if (seg_on) {
      switch (i) {
        case 0:
          display.fillRoundRect(n3 + pos_x, 0 + pos_y, n1, n3, r, color); // SEG a   // https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
          break;
        case 1:
          display.fillRoundRect(n1+n3 + pos_x, n3 + pos_y, n3, n2, r, color); // SEG b
          break;
        case 2:
          display.fillRoundRect(n1+n3 + pos_x, n2+2*n3 + pos_y, n3, n2, r, color); // SEG c
          break;
        case 3:
          display.fillRoundRect(n3 + pos_x, 2*n2+2*n3+ pos_y, n1, n3, r, color); // SEG d
          break;
        case 4:
          display.fillRoundRect(0 + pos_x, n2+2*n3 + pos_y, n3, n2, r, color); // SEG e
          break;
        case 5:
          display.fillRoundRect(0 + pos_x, n3 + pos_y, n3, n2, r, color); // SEG f
          break;
        case 6:
          display.fillRoundRect(n3 + pos_x, n2+n3 + pos_y, n1, n3, r, color); // SEG g
          break;
      }
      seg_on = false;
    }
  }
}
