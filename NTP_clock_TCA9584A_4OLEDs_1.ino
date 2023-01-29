/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/tca9548a-i2c-multiplexer-esp32-esp8266-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  v.0 - original version from Rui's site
  v.1 - original sketch by Nicu FLORICA (niq_ro)
      -  NTP clock inspired by https://nicuflorica.blogspot.com/2022/10/ceas-ntp-pe-afisaj-cu-tm1637.html
      -  hardware switch for DST (Daylight Saving Time) as at https://nicuflorica.blogspot.com/2022/12/ceas-ntp-pe-afisaj-oled-de-096-128x64.html
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
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(3);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 4
  TCA9548A(4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(5);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
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
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Display static text
  display.println(ora/10);
  display.display(); 

 // Write to OLED on bus number 3
  TCA9548A(3);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Display static text
  display.println(ora%10);
  if (secunda ==1)
  {
  display.fillCircle(127, 30, 3, WHITE);  // https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
  display.fillCircle(127, 50, 3, WHITE);  // https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
  }
  display.display(); 

 // Write to OLED on bus number 4
  TCA9548A(4);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Display static text
  display.println(minut/10);
  if (secunda ==1)
  {
  display.fillCircle(0, 30, 3, WHITE);  // https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
  display.fillCircle(0, 50, 3, WHITE);  // https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
  }
  display.display(); 

  // Write to OLED on bus number 5
  TCA9548A(5);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Display static text
  display.println(minut%10);
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

void clear_all()
{
  
}
