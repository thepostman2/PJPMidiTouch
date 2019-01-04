#ifndef PJPRadioGroup_h
#define PJPRadioGroup_h

#include <stdio.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include "varList.h"

#include "PJPTchObject.h"
#include "PJPTchButton.h"


namespace PJPTch
{
  class PJPRadioGroup: public TchObject
  {

    public:
          //constructors
          PJPRadioGroup(Adafruit_ILI9341&);
  
          //getters & setters
          uint16_t Value( )const;
          uint16_t Value(uint16_t );
          
          //operators
  
          //functions
          bool Inside(TPoint val)const;
          
          void Draw();
          void Add(PJPTchButton& obj);
          void SetActive(uint16_t index, boolean draw);
          void SetItemName(uint16_t index,String oname);

          boolean Touched(TS_Point p);
  
    protected:
      VarList<PJPTchButton> TchButtons_;
      int count_;
      uint16_t selectedItem_=0;
  
      void SetBoundaries();
  };
}
#endif /* PJPTchPage_h */
