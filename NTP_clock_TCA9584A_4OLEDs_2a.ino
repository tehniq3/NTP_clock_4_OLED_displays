/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/tca9548a-i2c-multiplexer-esp32-esp8266-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  v.0 - original version from Rui's site
  v.1 - original sketch by Nicu FLORICA (niq_ro)
      - NTP clock inspired by https://nicuflorica.blogspot.com/2022/10/ceas-ntp-pe-afisaj-cu-tm1637.html
      - hardware switch for DST (Daylight Saving Time) as at https://nicuflorica.blogspot.com/2022/12/ceas-ntp-pe-afisaj-oled-de-096-128x64.html
  v.2 - added 7-segment numbers, based in ideea from https://ta-laboratories.com/blog/2018/09/07/recreating-a-7-segment-display-with-adafruit-gfx-ssd1306-oled/
  v.2a - small correction in sketch (clean unusefull lines)
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char *ssid     = "niq_ro";
const char *password = "berelaburtica";
const long timezoneOffset = 2*3600; // GMT + seconds  (Romania)

#define DSTpin   14 // GPIO14 = D5, see https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", timezoneOffset);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int ora, minut, secunda;
byte DST = 0;
byte DST0;

// define segment truth table for each digit
static const int digit_array[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

uint8_t n1 = 40;
uint8_t n3 = 8;
uint8_t n2 = 52;
uint8_t r = 2;

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(bus);
}
 
void setup() {
  pinMode (DSTpin, INPUT);
  
  Serial.begin(115200);
  Serial.println (" ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }
    Serial.println (" ");
    
  if (digitalRead(DSTpin) == LOW)
   DST = 0;
  else
   DST = 1;
  timeClient.setTimeOffset(timezoneOffset + DST*3600);
  timeClient.begin();
  DST0 = DST;
  
  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Init OLED display on bus number 2
  TCA9548A(2);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  display.setRotation(1); // 
  display.setTextColor(WHITE);
  display.println("1");
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(3);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  display.setRotation(1); // 
  display.setTextColor(WHITE);
  display.println("2");
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 4
  TCA9548A(4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  display.setRotation(1); // 
  display.setTextColor(WHITE);
  display.println("3");
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(5);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  display.setRotation(1); // 
  display.setTextColor(WHITE);
  display.println("4");
  // Clear the buffer
  display.clearDisplay();

delay(2000);
}
 
void loop() {
 if (digitalRead(DSTpin) == LOW)
   DST = 0;
  else
   DST = 1;
  
timeClient.update();
 ora = timeClient.getHours();
 minut = timeClient.getMinutes();
 
/*
 ora = ora%12;  // 12-hour format
 if (ora == 0) ora = 12;  // 12-hour format
*/

Serial.print(ora);
Serial.print(";");
Serial.println(minut);

 // Write to OLED on bus number 2
  TCA9548A(2);
  display.clearDisplay();
  number(0, 0, ora/10, WHITE);
  display.display(); 

 // Write to OLED on bus number 3
  TCA9548A(3);
  display.clearDisplay();
  number(0, 0, ora%10, WHITE);
  
  if (secunda == 1)
  display.fillCircle (SCREEN_HEIGHT-3-r/2,SCREEN_WIDTH-3-r/2,r+1,WHITE);
  display.display(); 

 // Write to OLED on bus number 4
  TCA9548A(4);
  display.clearDisplay();
  number(0, 0, minut/10, WHITE);
  display.display(); 

  // Write to OLED on bus number 5
  TCA9548A(5);
  display.clearDisplay();
  number(0, 0, minut%10, WHITE);
  display.display(); 

if (DST != DST0)
{
  timeClient.setTimeOffset(timezoneOffset + DST*3600);
  timeClient.begin();
DST0 = DST;
}

secunda = (millis()/500)%2;
delay(500);
} // end main loop


void number(uint8_t pos_x, uint8_t pos_y,
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
