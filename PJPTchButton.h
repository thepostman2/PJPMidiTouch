/*!
* \file PJPTchButton.h
*  contains the definition of a PJPTchButton class. Part of the PJPMidiTouch project.
*
* Created by Peter on 28-04-18.
* Copyright © 2018 PJP. All rights reserved.
*/

#ifndef PJPTchButton_h
#define PJPTchButton_h

#include <stdio.h>
#include "PJPTchObject.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

namespace PJP
{
  /*! \class PJPTchButton
   *  \brief Generic class for all types of buttons on a touchscreen.
   */
    class PJPTchButton : public TchObject
    {
      public:
        /*! 
         * \brief Enumerator for button types
         */
        enum class eButtonType: uint8_t  {push, //!< push button on/off
                                          toggle, //!< toggle button on/off
                                          rotate, //!< round knob
                                          vslidr, //!< vertical slider
                                          hslidr //!< horizontal slider
                                                };

    
        //! \brief constructor based on location (centre of the button), and size.
        /* 
         * \param loc location (centre) of the button
         * \param s the size of the button
         * \param t type of the button
         * \param tft reference to the touchscreen
         * \parom oname name of the button
         * \param color integer value for the active color of the button
         * \param bcolor integer value for the background color of the button
         * \param textsize integer value for the size of the text on the button
         * 
        */
        PJPTchButton(TS_Point loc, TSize s, eButtonType t, Adafruit_ILI9341& tft,const char* oname="",uint16_t color=ILI9341_RED,
          uint16_t bcolor=ILI9341_BLACK,uint8_t textsize=2);
        //! constructor based on a size and the upper left corner of the button.
        /* 
         * \param s the size of the button
         * \param ul the location of the upper left corner of the button
         * \param t type of the button
         * \param tft reference to the touchscreen
         * \parom oname name of the button
         * \param color integer value for the active color of the button
         * \param bcolor integer value for the background color of the button
         * \param textsize integer value for the size of the text on the button
         */
        PJPTchButton(TSize s, TS_Point ul, eButtonType t, Adafruit_ILI9341& tft,const char* oname="",uint16_t color=ILI9341_RED,
          uint16_t bcolor=ILI9341_BLACK,uint8_t textsize=2);
        
        //getters and setters
        virtual PJPTchButton& get_this() {return *this;}
        
        //operators
        
        //functions
        virtual void Draw();
        void hideButton(uint16_t bcolor);
        boolean touch(TS_Point p);
        void releaseButton();
        
    private:
        eButtonType buttonType_;
        uint16_t color_;//!<color when active
        uint16_t backcolor_;//!<background color
        uint16_t inactiveColor_=  ILI9341_DARKGREY;//!<color if not active (default: RGB565 color dark grey)
        boolean hidden_;//!<determines if button is hidden
        uint8_t textsize_=2;//!<size of text presented on the button

        //private functions
        //! Set the buttonstate to on or off
        void setButtonState(boolean);
        
    };
}


#endif /* PJPTchButton_h */
