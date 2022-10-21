/*
Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.
Animated GIF Tuner Effect
Using the following Arduino libraries:
* AnimatedGIF library by Larry Bank:
  https://github.com/bitbank2/AnimatedGIF
* ESP_8_BIT Color Composite Video by Roger Cheng:
  https://github.com/Roger-random/ESP_8_BIT_composite
An animated GIF is decoded into an intermediate buffer, which is then copied
into the display buffer. The copy is done with a variable image stride width
intentionally not always the correct width value. When the width used is not
the correct width, the bitmap is drawn with an error that superficially
resembles analog TV tuning. This error is the intended effect of this sketch.
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

#include <AnimatedGIF.h>
#include <movingAvg.h>
#include <ESP_8_BIT_GFX.h>
#include "rear_window.h"
#include "spiral.h"
#include "advertisement.h"
#include "solar.h"
#include "interview.h"
#include "mondo.h"
#include "ape.h"
#include "oswald.h"
#include "wireframe.h"
#include "offair.h"



int horizontalOffset;
int secondaryOffset;
movingAvg avgPotValue(20);     

ESP_8_BIT_composite videoOut(true /* = NTSC */);
AnimatedGIF gif;

// The most recently used vertical roll offset. Zero when "in tune".
int verticalRoll;

// Vertical offset to use when picture is "in tune"
int verticalOffset;

// Intermediate buffer filled with data from AnimatedGIF
// to copy into display buffer.
uint8_t* intermediateBuffer;

// Width and height of decoded GIF
int gif_height;
int gif_width;

// When to display the next frame
long millisNextFrame;

// Convert RGB565 to RGB332
uint8_t convertRGB565toRGB332(uint16_t color)
{
  // Extract most significant 3 red, 3 green and 2 blue bits.
  return (uint8_t)(
        (color & 0xE000) >> 8 |
        (color & 0x0700) >> 6 |
        (color & 0x0018) >> 3
      );
}

// Draw a line of image to ESP_8_BIT_GFX frame buffer
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<pDraw->iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + pDraw->iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < pDraw->iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          for(int xOffset = 0; xOffset < iCount; xOffset++ ){
            intermediateBuffer[y*gif_width + pDraw->iX + x + xOffset] = convertRGB565toRGB332(usTemp[xOffset]);
          }
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--;
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
      {
        intermediateBuffer[y*gif_width + x] = convertRGB565toRGB332(usPalette[*s++]);
      }
    }
} /* GIFDraw() */

void setup() {
  Serial.begin(115200);

  videoOut.begin();
  avgPotValue.begin();

  gif.begin(LITTLE_ENDIAN_PIXELS);

  millisNextFrame = millis();

  intermediateBuffer = NULL;
  if (gif.open((uint8_t *)oswald_gif, oswald_gif_len, GIFDraw))
  {
    // Successfully parsed GIF data, allocate intermediate buffer based on GIF image size
    uint8_t* allocated = NULL;
    bool allocateSuccess = true;

    gif_width = gif.getCanvasWidth();
    gif_height = gif.getCanvasHeight();


    allocated = new uint8_t[gif_height*gif_width];
    if (NULL==allocated)
    {

      allocateSuccess = false;
    }
    if (allocateSuccess)
    {
      intermediateBuffer = allocated;
      allocated = NULL;

    }
  }
  else
  {
    gif_width = 0;
    gif_height = 0;

  }

  verticalRoll = 0;
  verticalOffset = (240-gif_height)/2;
}

// Clear output frame buffer to black
void clearFrame()
{
  uint8_t** pFrameBuffer = videoOut.getFrameBufferLines();
  for(int i = 0; i < 240; i++)
  {
    memset(pFrameBuffer[i],0, 256);
  }
}

// Copy intermediate GIF data to output frame buffer using given horizontal and vertical offsets
void copyIntermediateToFrame(int offset_h, int offset_v)
{
  int bufferEndIndex = gif_width * gif_height;
  int lastFullLineIndex = bufferEndIndex - gif_width;
  int currentIndex;

  uint8_t** pFrameBuffer = videoOut.getFrameBufferLines();
  for(int i = 0; i < gif_height; i++)
  {
    currentIndex = i*offset_h;

    // Make sure we don't copy garbage memory outside of intermediateBuffer range
    if (currentIndex < lastFullLineIndex)
    {
      // We can safely copy a full line
      memcpy(pFrameBuffer[(i+offset_v)%240],&(intermediateBuffer[currentIndex]),gif_width);
    }
    else if (currentIndex < bufferEndIndex)
    {
      // We can copy a partial line
      memcpy(pFrameBuffer[(i+offset_v)%240],&(intermediateBuffer[currentIndex]),bufferEndIndex-currentIndex-1);
    }
    else
    {
      // No more valid image lines to copy, stop the loop.
      break;
    }
  }
}

void loop() {
  // Horizontal offset is directly mapped from potentiometer position on pin 13.


  int potVal = analogRead(13);
  int AveragepotVal = avgPotValue.reading(potVal);


  if (AveragepotVal<=409){
    secondaryOffset = abs(AveragepotVal-409);
    horizontalOffset = map(secondaryOffset, 409, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 1");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

  if (AveragepotVal>409 && AveragepotVal<=819){
    secondaryOffset = abs(AveragepotVal-819);
    horizontalOffset = map(secondaryOffset, 410, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 2");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

  if (AveragepotVal>818 && AveragepotVal<=1228){
    secondaryOffset = abs(AveragepotVal-1228);
    horizontalOffset = map(secondaryOffset, 409, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 3");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

  if (AveragepotVal>1228 && AveragepotVal<=1638){
    secondaryOffset = abs(AveragepotVal-1639);
    horizontalOffset = map(secondaryOffset, 410, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

    if (AveragepotVal>1638 && AveragepotVal<=2047){
    secondaryOffset = abs(AveragepotVal-2047);
    horizontalOffset = map(secondaryOffset, 409, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }
    
    if (AveragepotVal>2047 && AveragepotVal<=2457){
    secondaryOffset = abs(AveragepotVal-2457);
    horizontalOffset = map(secondaryOffset, 410, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

    if (AveragepotVal>2457 && AveragepotVal<=2866){
    secondaryOffset = abs(AveragepotVal-2866);
    horizontalOffset = map(secondaryOffset, 409, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

    if (AveragepotVal>2866 && AveragepotVal<=3276){
    secondaryOffset = abs(AveragepotVal-3276);
    horizontalOffset = map(secondaryOffset, 410, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

    if (AveragepotVal>3276 && AveragepotVal<=3685){
    secondaryOffset = abs(AveragepotVal-3685);
    horizontalOffset = map(secondaryOffset, 409, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 4");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

  if (AveragepotVal>3685){
    secondaryOffset =abs(AveragepotVal-4095);
    horizontalOffset = map(secondaryOffset, 410, 0, gif_width+25, gif_width-25);
    Serial.println ("GIF zone 5");
    Serial.print("potentiometer value = "); 
    Serial.println (potVal);
    Serial.print("mapped value = "); 
    Serial.println (horizontalOffset);
    Serial.println();
  }

  // Vertical roll effect is calculated based on horizontal offset
  if (horizontalOffset < gif_width-1 || horizontalOffset > gif_width+1)
  {
    // If horizontal offset is far from actual gif width, add a vertical roll
    verticalRoll += horizontalOffset-gif_width;

    if (verticalOffset + verticalRoll < 0)
    {
      // Keep verticalOffset + verticalRoll within range of screen.
      verticalRoll += 240;
    }
    else if (verticalOffset + verticalRoll > 240)
    {
      // Keep verticalOffset + verticalRoll within range of screen.
      verticalRoll -= 240;
    }
  }
  else
  {
    // No vertical roll if horizontal offset is close to actual width.
    verticalRoll = 0;
  }

  // If it is time for the next animated GIF frame to be shown, advance a
  // frame or reset back to beginning.
  if (millis() > millisNextFrame)
  {
    int millisFrame;


    if(!gif.playFrame(false, &millisFrame))
    {

    if (AveragepotVal<= 409){
      gif.open((uint8_t *)solar_gif, solar_gif_len, GIFDraw);
    }

    if (AveragepotVal>409 && AveragepotVal<=819) {
      gif.open((uint8_t *)oswald_gif, oswald_gif_len, GIFDraw);
    }

    if (AveragepotVal>819 && AveragepotVal<=1228) {
      gif.open((uint8_t *)ape_gif, ape_gif_len, GIFDraw);
    }

    if (AveragepotVal>1228 && AveragepotVal<=1638) {
      gif.open((uint8_t *)wireframe_gif, wireframe_gif_len, GIFDraw);
    }

    if (AveragepotVal>1638 && AveragepotVal<=2047) {
      gif.open((uint8_t *)rear_window_gif, rear_window_gif_len, GIFDraw);
    }

    if (AveragepotVal>2047 && AveragepotVal<=2457) {
      gif.open((uint8_t *)offair_gif, offair_gif_len, GIFDraw);
    }

    if (AveragepotVal>2457 && AveragepotVal<=2866) {
      gif.open((uint8_t *)mondo_gif, mondo_gif_len, GIFDraw);
    }

    if (AveragepotVal>2866 && AveragepotVal<=3276) {
      gif.open((uint8_t *)advertisement_gif, advertisement_gif_len, GIFDraw);
    }

    if (AveragepotVal>3276 && AveragepotVal<=3685) {
      gif.open((uint8_t *)spiral_gif, spiral_gif_len, GIFDraw);
    }


    if (AveragepotVal>3685) {
      gif.open((uint8_t *)interview_gif, interview_gif_len, GIFDraw);
    }
      // No more frames, reset the loop to start again.
      gif.reset();
    }

    // Track the time for us to show the next frame.

    millisNextFrame = millis() + millisFrame;
  }

  // Output the current animation frame with veritical/horizontal offsets.
  clearFrame();
  copyIntermediateToFrame(horizontalOffset, verticalOffset + verticalRoll);
  videoOut.waitForFrame();
}
