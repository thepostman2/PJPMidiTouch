/*!
* \file PJPMidiTouch
*  PJPTchButton.cpp
*  PJPMidiTouch
*
* Created by Peter on 28-04-18.
* Copyright © 2018 PJP. All rights reserved.
*/

#include "PJPTchButton.h"
#include "RoundKnob.h"

namespace PJPTch {
//constructors
PJPTchButton::PJPTchButton(TS_Point loc, TSize s, eButtonType t, Adafruit_ILI9341& tft, const char* oname, uint16_t color, uint16_t bcolor, uint8_t textsize)
  : TchObject(tft, loc, s, oname), buttonType_(t), color_(color), backcolor_(bcolor), hidden_(false), textsize_(textsize)
{}
PJPTchButton::PJPTchButton(TSize s, TS_Point ul, eButtonType t, Adafruit_ILI9341& tft, const char* oname, uint16_t color, uint16_t bcolor, uint8_t textsize)
  : TchObject(tft, s, ul, oname), buttonType_(t), color_(color), backcolor_(bcolor), hidden_(false), textsize_(textsize)
{}

PJPTchButton::PJPTchButton(const PJPTchButton& obj):TchObject(obj),buttonType_(obj.buttonType_),
  inactiveColor_(obj.inactiveColor_)
{
  color_=obj.color_;
  backcolor_=obj.backcolor_;
  hidden_=obj.hidden_;
  textsize_=obj.textsize_;
}

PJPTchButton& PJPTchButton::operator=(const PJPTchButton& obj)
{
  TchObject::operator=(obj);
  buttonType_=obj.buttonType_;
  color_=obj.color_;
  backcolor_=obj.backcolor_;
  inactiveColor_= obj.inactiveColor_;
  hidden_=obj.hidden_;
  textsize_=obj.textsize_;
  return *this;
}


//getters and setters


//operators

//functions
void PJPTchButton::Draw()
{
  char sval[3];
  uint16_t ty = 0.5 * boundary_.Height() - textsize_ * 4;
  uint16_t xcentr = (textsize_ * 5 * 3 + 2) / 2;
  uint16_t ycentr = textsize_ * 8 / 2;
  uint16_t mask = value_ / (128 / RoundKnob_steps);

  tft_.setTextSize(textsize_);
  if (hidden_ == false)
  {
    switch (buttonType_)
    {
      case eButtonType::push:
      case eButtonType::toggle:
        tft_.setCursor(boundary_.UL.x + 5, boundary_.UL.y + ty);
        if (on_)
        {
          tft_.fillRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), color_);
          tft_.setTextColor(backcolor_, color_);
        }
        else
        {
          tft_.fillRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), backcolor_);
          tft_.setTextColor(color_, backcolor_);
        }
        tft_.drawRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), color_);
        tft_.print(name_);
        break;
      case eButtonType::rotate:
        {
          TS_Point loc = Location();
          //tft_.drawCircle(loc.x, loc.y, Width()/2,  color_);

          sprintf(sval, "%03d", value_);
          tft_.setTextColor(color_, backcolor_);
          tft_.setCursor(loc.x - xcentr, loc.y - ycentr);
          tft_.print(sval);

          tft_.drawXBitmap(boundary_.UL.x, boundary_.UL.y, RoundKnob_bits[mask], RoundKnob_width, RoundKnob_height, color_);
          tft_.drawXBitmap(boundary_.UL.x, boundary_.UL.y, RoundKnobMask_bits[mask], RoundKnob_width, RoundKnob_height, backcolor_);
          tft_.drawXBitmap(boundary_.UL.x, boundary_.UL.y, RoundKnobIna_bits[mask], RoundKnob_width, RoundKnob_height, inactiveColor_);
          if (on_)
            tft_.drawRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), color_);
          else
            tft_.drawRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), backcolor_);

          break;
        }
      case eButtonType::vslidr:
      case eButtonType::hslidr:
        break;
      default:
        break;
    }
  }
}

void PJPTchButton::hideButton(uint16_t bcolor)
{
  tft_.fillRect(boundary_.UL.x, boundary_.UL.y, Width(), Height(), bcolor);
  hidden_ = true;
}


boolean PJPTchButton::touch(TS_Point p)
{
  if (Inside(p))
  {
    setButtonState(true);
    return true;
  }
  else
  {
    setButtonState(false);
    return false;
  }

}

void PJPTchButton::releaseButton()
{
  setButtonState(false);
  //drawButton();
}



//private functions
void PJPTchButton::setButtonState(boolean beingTouched)
{
  switch (buttonType_)
  {
    case eButtonType::push:
      on_ = beingTouched;
      break;
    case eButtonType::toggle:
      if (beingTouched)
      {
        if (on_ && wasBeingTouched_ == false)
        {
          on_ = false;
        }
        else if (on_ == false  && wasBeingTouched_ == false)
        {
          on_ = true;
        }
        wasBeingTouched_ = true;
      }
      else
      {
        wasBeingTouched_ = false;
      }
      break;
    case eButtonType::rotate:
    case eButtonType::vslidr:
    case eButtonType::hslidr:
      break;

  }

}
}
