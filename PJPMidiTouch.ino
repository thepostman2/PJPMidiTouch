//
// PJPMidiTouch
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Peter
// 				PJP
//
// Date			19-04-18 21:02
// Version		<#version#>
//
// Copyright	© Peter, 2018
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//

#include <SPI.h>
//#include <SD.h>
#include <Wire.h>      // this is needed even though we aren't using it

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

#include "PJPTchObject.h"
#include "PJPButton.h"
#include "PJPPage.h"
#include "Bitmap.h"
#include "MackieCtrl.h"

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef ESP32
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#endif
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__) 
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif
#ifdef ARDUINO_NRF52_FEATHER /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#endif

// Core library for code-sense - IDE-based
/* !!! Help: http://bit.ly/2AdU7cu
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE
*/
// Set parameters


// Include application, user and local libraries


// Define structures and classes
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);


// Define variables and constants
// Color definitions
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750


using namespace PJP;
boolean touchd;
TS_Point p;
TS_Point p_released;


PJPPage myPage(tft);

// Prototypes
// !!! Help: http://bit.ly/2l0ZhTa


// Utilities
void clearBufferTS()
{
    while ( ! ts.bufferEmpty() ) {
        p = ts.getPoint();
    }
    ts.writeRegister8(STMPE_INT_STA, 0xFF);    // clear the 'touched' interrupts  

}


// Functions
void touchActions()
{
  // Retrieve a point
  if(ts.touched())
  {
    clearBufferTS();
    p_released=ts.getPoint(); //last release point
    while(ts.touched())
    {
      p=ts.getPoint();
      if(p.x!=p_released.x && p.y!=p_released.y)
      {
        // Scale from ~0->4000 to tft.width using the calibration #'s
        int x=p.x;
        p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.width()); 
        p.y = tft.height()-map(x, TS_MINX, TS_MAXX, 0, tft.height());

        //tft.fillCircle(p.x, p.y, 3, ILI9341_RED);
        
        if(touchd)//still touching, so not a new touch
        {
            dragTouchActions(p);
        }
        else //new touch
        {
            newTouchActions(p);
            touchd=true;
        }
        //PaintScreen();

      }
    }
    p_released=ts.getPoint(); //last release point
    releaseTouchActions(p);
    touchd=false;
  }
}  

 
uint16_t prevXval;
int16_t Value=127;
void PaintScreen( );
void newTouchActions(TS_Point p)
{
  myPage.Touched(p);
  prevXval=p.x;
  Value=myPage.Value();
}

void dragTouchActions(TS_Point p)
{
  int diff=(p.x-prevXval)/2;

  if(diff!=0)
  {
    Value=Value+diff;
    prevXval=p.x;
    if(Value>127) Value=127;
    if(Value<0) Value=0;
    myPage.Value(Value);
  }
}
void releaseTouchActions(TS_Point p)
{
  //tchSquare.releaseButton();
  int diff=(p.x-prevXval)/2;

  if(diff!=0 && diff<20 && diff>-20)
  {
    Value=Value+diff;
    prevXval=p.x;
    if(Value>127) Value=127;
    if(Value<0) Value=0;
  }  touchd=false;
}

void PaintScreen()
{

}
// Add setup code
void setup()
{
    // put your setup code here, to run once:
    touchd=false;
    Serial.begin(115200);
    pinMode(13, OUTPUT); 
  usbMIDI.setHandleControlChange(handleControlChange);
  usbMIDI.setHandleSystemExclusive(handleSysEx);

    delay(10);
    Serial.println("FeatherWing TFT");
    if (!ts.begin())
    {
        Serial.println("Couldn't start touchscreen controller");
        while (1);
    }
    Serial.println("Touchscreen started");

    tft.begin();
    tft.setRotation(1);
        
    tft.fillScreen(ILI9341_BLACK);
    //PaintScreen();
    myPage.Draw();
    tft.fillCircle(2,2,2,ILI9341_RED);
}

// Add loop code
void loop()
{
    touchActions();
}
