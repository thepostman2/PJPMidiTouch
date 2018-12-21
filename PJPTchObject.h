///
/// @file		PJPTchObject.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project PJPMidiTouch
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Peter
/// @author		PJP
///
/// @date		19-04-18 21:09
/// @version	<#version#>
///
/// @copyright	(c) Peter, 2018
/// @copyright	<#licence#>
///
/// @see		ReadMe.txt for references
///

// Core library for code-sense - IDE-based
// Help: http://bit.ly/2AdU7cu
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(SPARK) // Spark specific
#include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#ifndef PJPTchObject_cpp
#define PJPTchObject_cpp

#include "Arduino.h"
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>

using namespace std;


namespace PJP{
    
    bool between(int16_t val, int16_t first, int16_t last);
    

    
    struct TSize{
        uint16_t width;
        uint16_t height;
        
        //constructors
        TSize();
        TSize(uint16_t s);
        TSize(uint16_t w, uint16_t h);

        //operators
        TSize operator*(float f) const;
    };
    
    struct TVector{
        int16_t dx;
        int16_t dy;
        
        //constructors
        TVector();
        TVector(int16_t s);
        TVector(int16_t w, int16_t h);
        TVector(TSize s);
        
        //operators
        TVector operator*(float)const;
    };

    class TPoint: public TS_Point{
    public:
        //constructors
        TPoint();
        TPoint(TS_Point);
        TPoint(int16_t cx, int16_t cy);
        
        //operators
        TPoint operator+(const TVector&)const;
        TPoint operator+(const TSize&)const;
        TPoint operator-(const TVector&)const;
        TVector operator-(const TPoint&)const;
        void operator=(const TS_Point&);

        TSize Distance(const TPoint&) const;
     };
    
    struct TBoundary{
        TPoint UL;
        TPoint LR;
        
        //constructors
        TBoundary();
        TBoundary(const TBoundary & );
        TBoundary(const TPoint& ul,const TPoint& lr);
        TBoundary(const TPoint& loc, const TSize& s);
        TBoundary(const TSize& s, const TPoint& ul);
        
        //operators
        TBoundary& operator=(const TBoundary&);
        
        //functions
        bool Inside(const TPoint& p )const;
        uint16_t Width()const;
        uint16_t Height()const;
    };
    
    class TchObject{
    public:

        //constructors
        TchObject(Adafruit_ILI9341&, const TS_Point& loc, const TSize& s,const char* oname="");
        TchObject(Adafruit_ILI9341&, const TSize& s, const TS_Point& ul,const char* oname="");


    
        //get & set functions
        virtual TchObject& get_this() {return *this;}
        TBoundary Boundary() const;//get boundary of the object
        void Boundary(const TBoundary& val);//set boundary of the object
        void Boundary(const TPoint& UL, const TPoint& LR);//set boundary of the object
        void Boundary(TPoint location, const TSize& tsize);//set boundary of the object based on centre and size
        void Boundary(const TSize& tsize, TPoint UL);//set boundary of the object based on size and location UL

        boolean WasBeingTouched()const;

        const char* Name() const;
        void Name(const char* oname);
        boolean On()const;
        void On(boolean on);

        int16_t Value() const;
        int16_t Value(int16_t val);
        


        //functions
        TPoint UL()const;
        void UL(TPoint pt);
        TPoint LR()const;
        void LR(TPoint pt);
        
        ///get centre of the object
        TPoint Location() const;
        void Location(TPoint val);
        void Location(uint16_t x,uint16_t y);
        void Move(const TVector&);

        uint16_t Width()const;
        uint16_t Height()const;
        TSize Size() const;//get size of the object
        void Size(TSize val);//set size of the object
        void Size(uint16_t width,uint16_t height); //set size of the object
        
        const Adafruit_ILI9341& TFT()const;
        
        //functions
        bool Inside(TPoint val)const;
        virtual void Draw()=0;//{tft_.fillCircle(20,20,2,ILI9341_RED);} 
        
        TBoundary boundary_;
    protected:
        int16_t value_=0;
        char name_[16]="";
        boolean wasBeingTouched_=false;
        boolean active_=false;
        TchObject* parent_=NULL;
        Adafruit_ILI9341& tft_;
        boolean on_=false;

    };
}

#endif // PJPTchObject_cpp
