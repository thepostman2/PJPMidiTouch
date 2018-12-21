#ifndef PJPRadioGroup_h
#define PJPRadioGroup_h

#include <stdio.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <memory>
#include <vector>

#include "PJPTchObject.h"
#include "PJPButton.h"

using namespace std;

namespace PJP
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
          void Add(shared_ptr<TchObject>);
          void SetActive(uint16_t index);
          void SetItemName(uint16_t index,char oname[16]);

          boolean Touched(TS_Point p);
  
    protected:
      vector<shared_ptr<TchObject>> TchObjects_;
      uint16_t selectedItem_=0;
  
      void SetBoundaries(shared_ptr<TchObject>  obj);
  };
}
#endif /* PJPPage_h */

