
#ifndef PJPTchPage_h
#define PJPTchPage_h

#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>


#include "PJPTchObject.h"
#include "PJPTchButton.h"
#include "PJPRadioGroup.h"

namespace PJPTch
{
    
    class PJPTchPage : public TchObject
    {
      public:
        //constructors
        PJPTchPage(const char*,Adafruit_ILI9341&);

        //getters & setters
        uint16_t Value( ) const;
        uint16_t Value(uint16_t );

        //operators

        //functions
        void Draw();
        boolean Touched(TS_Point p);

      private:
        String pageName_;
        TSize pageSize_ = TSize(320,240);
        uint16_t xoffst_=50;
        uint16_t yoffst_=25;
        PJPRadioGroup tabs_;
        PJPRadioGroup knobs_;
        PJPRadioGroup items_;
    };
}


#endif /* PJPTchPage_h */
