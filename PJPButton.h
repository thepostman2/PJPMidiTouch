//
//  PJPButton.h
//  PJPMidiTouch
//
//  Created by Peter on 28-04-18.
//  Copyright © 2018 PJP. All rights reserved.
//

#ifndef PJPButton_h
#define PJPButton_h

#include <stdio.h>
#include "PJPTchObject.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

namespace PJP
{
    class PJPButton : public TchObject
    {
      public:
        enum class eButtonType: uint8_t  {push, toggle,rotate, slidr};

    
        //constructors
        PJPButton(TS_Point loc, TSize s, eButtonType t, Adafruit_ILI9341& tft,const char* oname="",uint16_t color=ILI9341_RED,
          uint16_t bcolor=ILI9341_BLACK,uint8_t textsize=2);
        PJPButton(TSize s, TS_Point ul, eButtonType t, Adafruit_ILI9341& tft,const char* oname="",uint16_t color=ILI9341_RED,
          uint16_t bcolor=ILI9341_BLACK,uint8_t textsize=2);
        
        //getters and setters
        virtual PJPButton& get_this() {return *this;}
        
        //operators
        
        //functions
        virtual void Draw();
        void hideButton(uint16_t bcolor);
        boolean touch(TS_Point p);
        void releaseButton();
        
    private:
        eButtonType buttonType_;
        uint16_t color_;
        uint16_t backcolor_;
        uint16_t inactiveColor_=  0x4208;//RGB565 color dark grey
        boolean hidden_;
        uint8_t textsize_=2;

        //private functions
        void setButtonState(boolean);
        
    };
}


#endif /* PJPButton_h */
