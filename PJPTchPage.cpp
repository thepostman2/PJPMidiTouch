//
//  PJPTchPage.cpp
//  PJPMidiTouch
//
//  Created by Peter on 28-04-18.
//  Copyright © 2018 PJP. All rights reserved.
//

#include "PJPTchPage.h"
#include "PJPTchButton.h"
#include "RoundKnob.h"
#include "PJPRadioGroup.h"

namespace PJPTch{
  
  /*!
   * \brief constructor
   * 
   * \param 
   */

  PJPTchPage::PJPTchPage(const char* pageName, Adafruit_ILI9341& tft):TchObject(tft,TS_Point(0,0,1),pageSize_),pageName_(pageName),pageSize_(TSize(tft.height(),tft.width())),
    xoffst_(50),yoffst_(25),tabs_(tft),knobs_(tft),items_(tft)
  {
    uint16_t xdistKnobs=(pageSize_.width-xoffst_)/4;
    uint16_t ydistKnobs=pageSize_.height-yoffst_-RoundKnob_height;
    uint16_t buttonX=pageSize_.width/4;
    uint16_t buttonY=20;
    uint16_t buttonYdist=pageSize_.height-buttonY;
    uint16_t itemKnob=25;

    for(int i=0;i<4;i++)
    {
      PJPTchButton knob(TSize(RoundKnob_width,RoundKnob_height),
        TS_Point(xoffst_+i*xdistKnobs,yoffst_,1),PJPTchButton::eButtonType::rotate,tft,"",ILI9341_RED,ILI9341_BLACK,2);
      knobs_.Add(knob);
      PJPTchButton tab(TSize(buttonX,buttonY),
        TS_Point(i*buttonX,0,1),PJPTchButton::eButtonType::toggle,tft,"",ILI9341_RED,ILI9341_BLACK,1);
      tabs_.Add(tab);
    }
    
    for(int i=0;i<4;i++)
    {
      PJPTchButton knob(TSize(RoundKnob_width,RoundKnob_height),
        TS_Point(xoffst_+i*xdistKnobs,ydistKnobs,1),PJPTchButton::eButtonType::rotate,tft,"",ILI9341_RED,ILI9341_BLACK,2);
      knobs_.Add(knob);
      PJPTchButton tab(TSize(buttonX,buttonY),
        TS_Point(i*buttonX,buttonYdist,1),PJPTchButton::eButtonType::toggle,tft,"",ILI9341_RED,ILI9341_BLACK,1);
      tabs_.Add(tab);
    }
  
    tabs_.SetItemName(0,"Oscillator");
    tabs_.SetItemName(1,"Filter");
    tabs_.SetItemName(2,"Amp");
    tabs_.SetItemName(3,"Envelope");
    tabs_.SetItemName(4,"LFO");
    tabs_.SetItemName(5,"Arp/Seq");
    tabs_.SetItemName(6,"Modulation");
    tabs_.SetItemName(7,"Effects/Mix");
    
    for(int i=0;i<6;i++)
    {
      String oname(i+1);
      PJPTchButton item(TSize(itemKnob,itemKnob),
        TS_Point(0.5*itemKnob,yoffst_+i*(1.3*itemKnob),1),PJPTchButton::eButtonType::toggle,tft,oname,ILI9341_RED,ILI9341_BLACK,2);
      items_.Add(item);
    }
    tabs_.SetActive(0,false);
    knobs_.SetActive(0,false);
    items_.SetActive(0,false);
  }
  
  //getters & setters
  uint16_t PJPTchPage::Value()const
  {
    return knobs_.Value();
  }

  uint16_t PJPTchPage::Value(uint16_t val)
  {
    return knobs_.Value(val);
  }

  //operators
  
  //functions
  void PJPTchPage::Draw()
  {
      tabs_.Draw();
      knobs_.Draw();
      items_.Draw();
  }

  boolean PJPTchPage::Touched(TS_Point p)
  {
    if(tabs_.Touched(p))
    {
      return true;
    }
    if(knobs_.Touched(p))
    {
      return true;
    }
    if(items_.Touched(p))
    {
      return true;
    }
    return false;
  }

  
  

}
