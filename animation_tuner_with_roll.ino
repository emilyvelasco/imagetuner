/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Hello World

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. Draws two
rectangles that cycle around the border of the screen. The amount of corners
cut off from these rectangle show the amount of overscan on a particular
screen. In the middle of two rectangles are a bit of text drawn using
Adafruit GFX print() API.

Copyright (c) Roger Cheng

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*this code creates a glitched display of a bitmap by setting the width 
of the bitmap incorrectly, causing pixels from one line to appear on the next
line. The user is then able to 'tune' in the image by turning a potentiometer
whose analogread value is mapped to a variable representing the width of the 
bitmap*/

#include <ESP_8_BIT_GFX.h>
#include "spiralframes.h"
#include "flowerframes.h"

int x_Dimension1Value = 300; //sets the bitmap width wider than it should be
int x_Dimension2Value = 300; //sets the bitmap width wider than it should be
int y_position1 = 23;
int y_position2 = 23;
int rollSpeed1;
int rollSpeed2;
int startTime;

// A list of 8-bit color values that work well in a cycle.
uint8_t colorCycle[] = {
  0xFF, // White
  0xFE, // Lowering blue
  0xFD,
  0xFC, // No blue
  0xFD, // Raising blue
  0xFE,
  0xFF, // White
  0xF3, // Lowering green
  0xE7,
  0xE3, // No green
  0xE7, // Raising green
  0xF3,
  0xFF, // White
  0x9F, // Lowering red
  0x5F,
  0x1F, // No red
  0x5F, // Raising red
  0x9F,
  0xFF
};

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);

void setup() {
  // Initial setup of graphics library
  videoOut.begin();
  videoOut.setRotation(4);
  Serial.begin(115200);
  startTime = millis();
  
}

void loop() {
  // Wait for the next frame to minimize chance of visible tearing
int potVal = analogRead(13); //reads the value of the pot
videoOut.setRotation(4);
Serial.println(x_Dimension1Value);
Serial.println(potVal);

  
  videoOut.waitForFrame();

  // Clear screen
  videoOut.fillScreen(0);
 
//if the potentiometer is turned to less than halfway, display the first animation
  if (potVal <= 2047){
  int potVal = analogRead(13); //reads the value of the pot
  x_Dimension1Value = map(potVal, 0, 2047, 225, 275); //maps the pot value to x dimensions
  rollSpeed1 = 6;
//the following creates an animation by cycling through frames, each frame being layered bitmaps
if (millis()-startTime < 200){
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_1_green, x_Dimension1Value, 179, 0xF0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_1_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_1_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_1_white, x_Dimension1Value, 179, 0xFF);

}
if ((millis()-startTime >= 200) && (millis()-startTime <400)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_2_green, x_Dimension1Value, 179, 0xF0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_2_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_2_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_2_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 400) && (millis()-startTime <600)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_3_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_3_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_3_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_3_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 600) && (millis()-startTime <800)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_4_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_4_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_4_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_4_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 800) && (millis()-startTime <1000)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_5_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_5_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_5_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_5_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 1000) && (millis()-startTime <1200)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_6_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_6_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_6_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_6_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 1200) && (millis()-startTime <1400)){
  videoOut.fillScreen(0);
  videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_7_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_7_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_7_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_7_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 1400) && (millis()-startTime <1600)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_8_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_8_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_8_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_8_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 1600) && (millis()-startTime <1800)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_9_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_9_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_9_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_9_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 1800) && (millis()-startTime <2000)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_10_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_10_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_10_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_10_white, x_Dimension1Value, 179, 0xFF);

}

if ((millis()-startTime >= 2000) && (millis()-startTime <2200)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_11_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_11_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_11_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_11_white, x_Dimension1Value, 179, 0xFF);

}


if (millis()-startTime >= 2200){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_12_green, x_Dimension1Value, 179, 0x1C);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_12_blue, x_Dimension1Value, 179, 0x3F);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_12_red, x_Dimension1Value, 179, 0xE0);
   videoOut.drawBitmap(-30, y_position1, epd_bitmap_Layer_12_white, x_Dimension1Value, 179, 0xFF);

   
  if (millis()-startTime >= 2400){
   startTime = millis();
   }
}
//below makes the image roll/scroll vertically except within "in-tune" range
if( x_Dimension1Value <=249 or x_Dimension1Value >= 256){
    y_position1 = y_position1+rollSpeed1;
    if (y_position1 >= 240){
    y_position1 = 0;
    }
   }
  if( x_Dimension1Value <256 && x_Dimension1Value > 249){
    y_position1 = 40;
  }

 
   }
//if the potentiometer is turned to more than halfway, display the second animation
   if (potVal>=2048){
    int potVal = analogRead(13); //reads the value of the pot
    x_Dimension2Value = map(potVal, 2048, 4095, 275, 225); //maps the pot value to x dimensions
    rollSpeed2 = 6;

if (millis()-startTime < 200){
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_1_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_1_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_1_white, x_Dimension2Value, 143, 0xF7);
}
if ((millis()-startTime >= 200) && (millis()-startTime <400)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_2_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_2_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_2_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 400) && (millis()-startTime <600)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_3_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_3_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_3_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 600) && (millis()-startTime <800)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_4_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_4_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_4_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 800) && (millis()-startTime <1000)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_5_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_5_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_5_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 1000) && (millis()-startTime <1200)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_6_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_6_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_6_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 1200) && (millis()-startTime <1400)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_7_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_7_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_7_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 1400) && (millis()-startTime <1600)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_8_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_8_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_8_white, x_Dimension2Value, 143, 0xF7);
}

if ((millis()-startTime >= 1600) && (millis()-startTime <1800)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_9_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_9_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_9_white, x_Dimension2Value, 143, 0xF7);
}

/*if ((millis()-startTime >= 1800) && (millis()-startTime <2000)){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_10_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_10_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_10_white, x_Dimension2Value, 143, 0xF7);
}*/


if (millis()-startTime >= 1800){
  videoOut.fillScreen(0);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_11_gray1, x_Dimension2Value, 143, 0xE6);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_11_gray2, x_Dimension2Value, 143, 0xA1);
   videoOut.drawBitmap(-30, y_position2, epd_bitmap_flower_11_white, x_Dimension2Value, 143, 0xF7);
  if (millis()-startTime >= 900){
   startTime = millis();
   }
}
//below makes the image roll/scroll vertically except within "in-tune" range
    if( x_Dimension2Value <=249 or x_Dimension2Value >= 256){
    y_position2 = y_position2+rollSpeed2;
    if (y_position2 >= 240){
     y_position2 = 0;
    }
   }
   if( x_Dimension2Value <256 && x_Dimension2Value > 249){
    y_position2 = 40;
  }
 
}
}
