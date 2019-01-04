/*!
* @file PJPMidiTouch
*
* @mainpage Midi Controller using Teensy and Adafruit Featherwing 2.4
* Developed with [embedXcode](http://embedXcode.weebly.com)
*
* @section intro_sec Introduction
* 
* This is the documentation for PJPMidiTouch. 
* This project is aimed on creating general a midi controller
* for soft synthesizers. The idea is to make a controller that is
* not aimed on controlling DAWs, but rather on controlling plug-ins.
* 
* Author 	PJP
* Date			21-12-2018
* Version		1
*
* Copyright	© PJP, 2018
* Licence		<#licence#>
*
*
*/

#include <SPI.h>
//#include <SD.h>
#include <Wire.h>      // this is needed even though we aren't using it

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

#include "PJPTchObject.h"
#include "PJPTchButton.h"
#include "PJPTchPage.h"
#include "Bitmap.h"
#include "MackieCtrl.h"

#define DEBUG true //!< flag to turn on/off debugging.
#define Serial if(DEBUG)Serial 
#define SHOWTOUCH true //!< flag to turn on/off showing the touch locations.

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
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);//!< Reference to Adafruit TFT screen
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);//!< Reference to Adafruit touch screen


// Define variables and constants
// Color definitions
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800 //!< Touchscreen minimal X value
#define TS_MAXX 100 //!< Touchscreen max X value
#define TS_MINY 100 //!< Touchscreen minimal Y value
#define TS_MAXY 3750 //!< Touchscreen max Y value

#define MIDI_MAXVALUE 127 //!< maximum value for midi
#define MIDI_MINVALUE 0 //!< minimum value for midi

using namespace PJPTch;
boolean touchd; //!< Valid if touch screen is already being touched
TS_Point p; //!< Location where the screen is being touched
TS_Point p_released; //!< Location where the screen is being released


PJPTchPage myPage("first",tft);//!< Page with all controls

// Prototypes
void clearBufferTS();
void touchActions();
void newTouchActions(TS_Point p);
void dragTouchActions(TS_Point p);
void releaseTouchActions(TS_Point p);


// Utilities
uint16_t prevXval; //!< hold the x coordinate of the previous touched location
int16_t Value=127; //!< Value of the current parameter being editted

// Functions
/**************************************************************************/
/*!
    \brief  Clear the buffer from the touchscreen.
    *
    *This function clears the buffer (all unhandled touched points) 
    *from the touchscreen. 
*/
/**************************************************************************/
void clearBufferTS()
{
    while ( ! ts.bufferEmpty() ) {
        p = ts.getPoint();//!< p will keep the last position of the buffer
    }
    ts.writeRegister8(STMPE_INT_STA, 0xFF);    //!< clear the 'touched' interrupts  
}

/**************************************************************************/
/*!
    \brief  Handles all touch actions on the touchscreen.
    *
    *As long as the screen is being touched, the function will determine
    *where it is touched and if any drag actions are performed.
*/
/**************************************************************************/
void touchActions()
{
  if(ts.touched())
  { 
    clearBufferTS();//!< remove any previous touched locations
    p_released=ts.getPoint(); //!< retrieve last touched point
   
    while(ts.touched())
    {
      p=ts.getPoint();//!< retrieve current touch locaction
      
      if(p.x!=p_released.x && p.y!=p_released.y)
      {
        // Scale from ~0->4000 to tft.width using the calibration #'s
        int16_t x=p.x;
        p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.width()); 
        p.y = tft.height()-map(x, TS_MINX, TS_MAXX, 0, tft.height());

        if(SHOWTOUCH) tft.fillCircle(p.x, p.y, 3, ILI9341_DARKGREY);//shows touches on the screen.
        
        if(touchd)//still touching, so not a new touch
        {
            dragTouchActions(p);
        }
        else //new touch
        {
            newTouchActions(p);
            touchd=true;
        }
        
      }
    }
    p_released=ts.getPoint(); //last release point
    releaseTouchActions(p);
    touchd=false;
  }
}  

 

/*!
    \brief  Handle a new touch action
*/
void newTouchActions(TS_Point p)
{
  myPage.Touched(p);//!< let the page determine which element is touched
  prevXval=p.x;//!< save current X location as previous X location
  Value=myPage.Value();//!<retrieve the value of the current selected knob
}
/*!
    \brief  Handle dragging over the screen
    *
    *Only dragging in X direction is currently used.
    *The value of the selected knob is changed according
    *to the dragged distance in X direction.
*/
void dragTouchActions(TS_Point p)
{
  int diff=(p.x-prevXval)/2;//!<determine the X distance between current and previous position

  if(diff!=0)
  {
    Value=Value+diff; //!<calculate the new value
    prevXval=p.x;//!<set the current X location as the previous X location
    if(Value>MIDI_MAXVALUE) Value=MIDI_MAXVALUE;//!<check  the value doesn't exceed the max midi value of 127.
    if(Value<MIDI_MINVALUE) Value=MIDI_MINVALUE;//!<check  the value doesn't exceed the min midi value of 0.
    myPage.Value(Value);//!<update the selected knob with the new value.
  }
}
/*!
    \brief  Handle releasing the screen
    *
    *Currently not much to do. Just setting touchd to false.
    *It might seem a good idea to handle the last location as a dragTouchAction, 
    *but it is not.
    *In practice it can cause the previous selected knob to change value when
    *selecting a different knob.
*/
void releaseTouchActions(TS_Point p)
{
   touchd=false;//!<set touchd to false, since it has been released.
}

/*!
    \brief  Setting everything ready for running the program.
*/
void setup()
{
    touchd=false;//!< initially screen is not touched.
    Serial.begin(115200);//!< Set Serial if in DEBUG mode
    pinMode(13, OUTPUT); 
    usbMIDI.setHandleControlChange(handleControlChange);//!< set midi handler for receiving control change messages.
    usbMIDI.setHandleSystemExclusive(handleSysEx);//!< set midi handler for receiving system exclusive messages.

    delay(10);
    Serial.println("FeatherWing TFT");
    if (!ts.begin())
    {
        Serial.println("Couldn't start touchscreen controller");
        while (1);
    }
    Serial.println("Touchscreen started");

    tft.begin();//!<start the touch screen
    tft.setRotation(1);//! rotate the touch screen to landscape mode
        
    tft.fillScreen(ILI9341_BLACK);//!< clear the touchscreen
    myPage.Draw();//!< draw the first page*/
}

/*!
    \brief  The program itself (is a loop).
*/
void loop()
{
    touchActions();//!<check for, and handle any touch actions on the touch screen
}
