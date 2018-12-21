#ifndef PJPPage_h
#define PJPPage_h

#include <stdio.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <string>

#include "PJPTchObject.h"
#include "PJPButton.h"
#include "PJPRadioGroup.h"

using namespace std;

namespace PJP
{
    
    class PJPPage : public TchObject
    {
      public:
        //constructors
        PJPPage(/*const string&,*/Adafruit_ILI9341&);

        //getters & setters
        uint16_t Value( ) const;
        uint16_t Value(uint16_t );

        //operators

        //functions
        void Draw();
        boolean Touched(TS_Point p);

      private:
        TSize pageSize_ = TSize(320,240);
        uint16_t xoffst_=50;
        uint16_t yoffst_=25;
        PJPRadioGroup tabs_;
        PJPRadioGroup knobs_;
        PJPRadioGroup items_;
    };
}


#endif /* PJPPage_h */
