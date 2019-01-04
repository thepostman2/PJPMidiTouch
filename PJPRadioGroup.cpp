//
//  PJPTchPage.cpp
//  PJPMidiTouch
//
//  Created by Peter on 09-05-18.
//  Copyright © 2018 PJP. All rights reserved.
//
#include "PJPRadioGroup.h"
#include <cmath>


namespace PJPTch
{
  //constructors
  PJPRadioGroup::PJPRadioGroup(Adafruit_ILI9341& tft):TchObject(tft,TS_Point(0,0,1),TSize(1,1)),count_(0)
  {
    
  }



  //getters & setters
  uint16_t PJPRadioGroup::Value()const
  {
    return TchButtons_[selectedItem_].Value();
  }

  uint16_t PJPRadioGroup::Value(uint16_t val)
  {
    TchButtons_[selectedItem_].Value(val);
    TchButtons_[selectedItem_].Draw();
    return TchButtons_[selectedItem_].Value();
  }

  //operators

  //functions

  void PJPRadioGroup::Add(PJPTchButton& obj)
  {
    TchButtons_.push_back(obj);

    
    if(TchButtons_.size==1) boundary_.operator=(obj.boundary_);
    //TchButtons_.push_back(obj);
    SetBoundaries();
  }

  void PJPRadioGroup::Draw()
  {
    TchButtons_[selectedItem_].On(true);
    for(uint16_t i=0;i<TchButtons_.size;i++)
    {
      TchButtons_[i].Draw();
    }
  }

  bool PJPRadioGroup::Inside(TPoint p)const
  {
    boolean inside=false;
    for(uint16_t i=0;i<TchButtons_.size;i++)
    {
      if(TchButtons_[i].Inside(p))
        inside=true;
    }
    return inside;
  }

  void PJPRadioGroup::SetActive(uint16_t index, boolean draw)
  {
    if(index<TchButtons_.size)
    {
      TchButtons_[selectedItem_].On(false);
      if(draw) TchButtons_[selectedItem_].Draw();
      selectedItem_=index;
      TchButtons_[selectedItem_].On(true);
      if(draw) TchButtons_[selectedItem_].Draw();
    }      
  }

  void PJPRadioGroup::SetItemName(uint16_t index,String oname)
  {
    if(index<TchButtons_.size)
    {
      TchButtons_[index].Name(oname);
    }
      
  }

  boolean PJPRadioGroup::Touched(TS_Point p)
  {
    if(selectedItem_>-1)
    {
      int16_t item=-1;;
      for(uint16_t i=0;i<TchButtons_.size;i++)
      {
        if(TchButtons_[i].Inside(p))
        {
          item=i;
          SetActive(i,true);
          break;
        }
      }
      return (item > -1);
    }
    return false;
  }

//protected functions
  void PJPRadioGroup::SetBoundaries()
  {
    for(uint16_t i=0;i<TchButtons_.size;i++)
    {
      boundary_.UL.x=fmin(TchButtons_[i].boundary_.UL.x,boundary_.UL.x);
      boundary_.UL.y=fmin(TchButtons_[i].boundary_.UL.y,boundary_.UL.y);
      boundary_.LR.x=fmax(TchButtons_[i].boundary_.LR.x,boundary_.LR.x);
      boundary_.LR.y=fmax(TchButtons_[i].boundary_.LR.y,boundary_.LR.y);
    }
  }
}
