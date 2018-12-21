//
//  PJPPage.cpp
//  PJPMidiTouch
//
//  Created by Peter on 09-05-18.
//  Copyright © 2018 PJP. All rights reserved.
//
#include "PJPRadioGroup.h"
#include <cmath>


namespace PJP
{
  //constructors
  PJPRadioGroup::PJPRadioGroup(Adafruit_ILI9341& tft):TchObject(tft,TS_Point(0,0,1),TSize(1,1))
  {
    
  }



  //getters & setters
  uint16_t PJPRadioGroup::Value()const
  {
    return TchObjects_[selectedItem_]->Value();
  }

  uint16_t PJPRadioGroup::Value(uint16_t val)
  {
    TchObjects_[selectedItem_]->Value(val);
    TchObjects_[selectedItem_]->Draw();
    return TchObjects_[selectedItem_]->Value();
  }

  //operators

  //functions

  void PJPRadioGroup::Add(shared_ptr<TchObject> obj)
  {
    if(TchObjects_.size()<1) boundary_.operator=(obj->boundary_);
    TchObjects_.push_back(obj);
    SetBoundaries(obj);
  }

  void PJPRadioGroup::Draw()
  {
    TchObjects_[selectedItem_]->On(true);
    for(uint16_t i=0;i<TchObjects_.size();i++)
    {
      TchObjects_[i]->Draw();
    }
  }

  bool PJPRadioGroup::Inside(TPoint p)const
  {
    boolean inside=false;
    for(uint16_t i=0;i<TchObjects_.size();i++)
    {
      if(TchObjects_[i]->Inside(p))
        inside=true;
    }
    return inside;
  }

  void PJPRadioGroup::SetActive(uint16_t index)
  {
    if(index<TchObjects_.size() )
    {
      TchObjects_[selectedItem_]->On(false);
      TchObjects_[selectedItem_]->Draw();
      selectedItem_=index;
      TchObjects_[selectedItem_]->On(true);
      TchObjects_[selectedItem_]->Draw();
    }      
  }

  void PJPRadioGroup::SetItemName(uint16_t index,char oname[16])
  {
    if(index<TchObjects_.size())
    {
      TchObjects_[index]->Name(oname);
    }
      
  }

  boolean PJPRadioGroup::Touched(TS_Point p)
  {
    if(selectedItem_>-1)
    {
      int16_t item=-1;;
      for(uint16_t i=0;i<TchObjects_.size();i++)
      {
        if(TchObjects_[i]->Inside(p))
        {
          item=i;
          SetActive(i);
          break;
        }
      }
      return (item > -1);
    }
    return false;
  }

//protected functions
  void PJPRadioGroup::SetBoundaries(shared_ptr<TchObject>  obj)
  {
    boundary_.UL.x=fmin(obj->boundary_.UL.x,boundary_.UL.x);
    boundary_.UL.y=fmin(obj->boundary_.UL.y,boundary_.UL.y);
    boundary_.LR.x=fmax(obj->boundary_.LR.x,boundary_.LR.x);
    boundary_.LR.y=fmax(obj->boundary_.LR.y,boundary_.LR.y);
  }

}



