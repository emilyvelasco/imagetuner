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
#include "grouperlayers.h"
#include "anglerlayers.h"


int x_Dimension1Value = 300; //sets the bitmap width wider than it should be
int x_Dimension2Value = 300; //sets the bitmap width wider than it should be
int y_position1 = 23;
int y_position2 = 23;
int rollSpeed = 1;
int rollSpeed2 = 1;

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

}

void loop() {
  // Wait for the next frame to minimize chance of visible tearing

videoOut.setRotation(4);
Serial.print("X2 dimension =");Serial.println(x_Dimension2Value);

  
  videoOut.waitForFrame();

  // Clear screen
  videoOut.fillScreen(0);
  int potVal = analogRead(13); //reads the value of the pot

  if (potVal <= 2047){

  x_Dimension1Value = map(potVal, 0, 2047, 225, 275); //maps the pot value to x dimensions

   videoOut.drawBitmap(0, y_position1, epd_bitmap_grouper_gray1, x_Dimension1Value, 239, 0x88);
   videoOut.drawBitmap(0, y_position1, epd_bitmap_grouper_gray2, x_Dimension1Value, 239, 0xEC);
   videoOut.drawBitmap(0, y_position1, epd_bitmap_grouper_gray3, x_Dimension1Value, 239, 0xF5);
   rollSpeed = 10;

   if( x_Dimension1Value <=249 or x_Dimension1Value >= 256){
    y_position1 = y_position1+rollSpeed;
    if (y_position1 >= 240){
    y_position1 = 0;
    }
   }
  if( x_Dimension1Value <256 && x_Dimension1Value > 249){
    y_position1 = 40;
  }
   }

   if (potVal>=2048){

  x_Dimension2Value = map(potVal, 4095, 2048, 225, 375); //maps the pot value to x dimensions
   videoOut.drawBitmap(0, y_position2, epd_bitmap_anglerfish_gray1, x_Dimension2Value, 239, 0x10);
   videoOut.drawBitmap(0, y_position2, epd_bitmap_anglerfish_gray2, x_Dimension2Value, 239, 0x18);
   videoOut.drawBitmap(0, y_position2, epd_bitmap_anglerfish_gray3, x_Dimension2Value, 239, 0xBE);
   rollSpeed2 = 10;
   
    if( x_Dimension2Value <=250 or x_Dimension2Value >= 258){
    y_position2 = y_position2+rollSpeed2;
    if (y_position2 >= 240){
     y_position2 = 0;
    }
   }
   if( x_Dimension2Value <258 && x_Dimension2Value > 250){
    y_position2 = 40;
  }
}
}
