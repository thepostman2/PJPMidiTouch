/*!
 \file PJPTchObject.cpp

 \brief description of generic touch object for GUI on a touchscreen
*/


// Library header
#include "PJPTchObject.h"
#include "math.h"


// Code
namespace PJPTch{
    
    /**
     * @brief  between checks if a val is the same or in between two other values
     *
     * @param  uint16_t val: the value that is checked
     * @param  uint16_t first: the first value between which val should be
     * @param  uint16_t last: the last value between which val should be
     * @return bool: true if val is the same or in between first and last, otherwise false
     */
    bool between(int16_t val, int16_t first, int16_t last)
    {
        if(val >= first and val <= last)
            return true;
        else if(val <= first and val >= last)
            return true;
        else
            return false;
    }

    //TVector ============================
    //constructors
    TVector::TVector():dx(1),dy(1){}
    TVector::TVector(int16_t ds):dx(ds),dy(ds){}
    TVector::TVector(int16_t vdx, int16_t vdy):dx(vdx),dy(vdy){}
    TVector::TVector(TSize s):dx(s.width),dy(s.height){}

    /**
     * @brief  operator*
     *
     * @param  nothing
     * @return resized version
     */
    TVector TVector::operator*(float f) const
    {
        return TVector(f*dx, f*dy);
    }
    
    //TSize ============================

    //constructors
    TSize::TSize():width(),height(){}
    TSize::TSize(uint16_t s):width(s),height(s){}
    TSize::TSize(uint16_t w, uint16_t h):width(w),height(h){}
    
    /**
     * @brief  operator*
     *
     * @param  nothing
     * @return resized version
     */
    TSize TSize::operator*(float f) const
    {
        return TSize(f*width, f*height);
    }
    
    
    //TPoint ============================
    
    //constructors
    TPoint::TPoint() : TS_Point(){}
    TPoint::TPoint(TS_Point p): TS_Point(p){}
    TPoint::TPoint(int16_t cx, int16_t cy):TS_Point(cx,cy,1){}
    
    //operators
    TPoint TPoint::operator+(const TVector& v) const
    {
        return TPoint(x+v.dx,y+v.dy);
    }

    TPoint TPoint::operator+(const TSize& s) const
    {
        return TPoint(x+s.width,y+s.height);
    }
    
    TPoint TPoint::operator-(const TVector& v)const
    {
        return TPoint(x-v.dx,y-v.dy);
    }
    TVector TPoint::operator-(const TPoint& p)const
    {
        return TVector(x-p.x,y-p.y);
    }
    
    void TPoint::operator=(const TS_Point& p)
    {
      x=p.x;
      y=p.y;
      z=p.z;  
    
    }
   
    //functions
    TSize TPoint::Distance(const TPoint& p) const
    {
        return TSize(abs(x-p.x),abs(y-p.y));
    }
    
    //TBoundary ============================
    TBoundary::TBoundary()
    {}
    
    TBoundary::TBoundary(const TBoundary& val):UL(val.UL),LR(val.LR)
    {}
    
    TBoundary::TBoundary(const TPoint& ul, const TPoint& lr):UL(ul),LR(lr)
    {}
    TBoundary::TBoundary(const TPoint& loc, const TSize& s): UL(loc.operator+(TVector(s).operator*(-0.5))),
      LR(loc.operator+(TVector(s).operator*(0.5)))

    {
    }
    TBoundary::TBoundary(const TSize& s,const TPoint& ul): UL(ul),LR(ul.operator+(s))
    {
    }
    
    TBoundary& TBoundary::operator=(const TBoundary& other)
    {
        UL.operator=(other.UL);
        LR.operator=(other.LR);
        return *this;
    }
    bool TBoundary::Inside(const TPoint& p )const
    {
     if (between(p.x,UL.x,LR.x) and between(p.y,UL.y,LR.y))
            return true;
        else
            return false;
    }
    
    uint16_t TBoundary::Width()const{return LR.x-UL.x;}
    uint16_t TBoundary::Height()const{return LR.y-UL.y;}
    //TchObject ============================
    
    //constructors
    TchObject::TchObject(Adafruit_ILI9341& tft, const TS_Point& loc, const TSize& s,const String oname):boundary_(loc,s),name_(oname),tft_(tft)
    {
    }
    TchObject::TchObject(Adafruit_ILI9341& tft, const TSize& s, const TS_Point& ul,const String oname):boundary_(s,ul),name_(oname),tft_(tft)
    {
    }
    
    TchObject::TchObject(const TchObject& obj):boundary_(obj.boundary_),value_(obj.value_),name_(obj.name_),
    wasBeingTouched_(obj.wasBeingTouched_),active_(obj.active_),parent_(obj.parent_),tft_(obj.tft_),on_(obj.on_)
    {}

    TchObject& TchObject::operator=(const TchObject& obj)
    {
      boundary_=obj.boundary_;
      value_=obj.value_;
      name_=obj.name_;
      wasBeingTouched_=obj.wasBeingTouched_;
      active_=obj.active_;
      parent_=obj.parent_;
      //tft_=obj.tft_;
      on_=obj.on_;
      return *this;
    }
    //get & set functions=====================================
    
    /**
     * @brief  get boundary of the object
     *
     * @param  nothing
     * @return boundary of the object
     */
    TBoundary TchObject::Boundary() const
    {
        return boundary_;
    }
    /**
     * @brief  set boundary of the object
     *
     * @param  val is a TBoundary, de boundary of the object
     * @return nothing
     */
    void TchObject::Boundary(const TBoundary& val)
    {
        boundary_.operator=(val);
    }
    /**
     * @brief  set boundary of the object
     *
     * @param  UL is a TPoint, the upper left corner of the object
     * @param  LR is a TPoint, the lower right corner of the object
     * @return nothing
     */
    void TchObject::Boundary(const TPoint& ul, const TPoint& lr) 
    {
      boundary_.UL=ul;
      boundary_.LR=lr;
    }
    
    /**
     * @brief  set boundary of the object based on given location and size
     *
     * @param  location is a TPoint, the centre of the TchObject
     * @param  tsize is a TSize, the size of the TchObject
     * @return nothing
     */
    void TchObject::Boundary(TPoint location,const TSize& tsize)
    {
      boundary_.UL=location.operator+(TVector(tsize).operator*(-0.5));
      boundary_.LR=location.operator+(TVector(tsize).operator*(0.5));
    }
    
    /**
     * @brief  set boundary of the object based on size and location UL
     *
     * @param  tsize is a TSize, the size of the TchObject
     * @param  TPoint ul is the upper left corner of the TchObject
     * @return nothing
     */
    void TchObject::Boundary(const TSize& tsize,TPoint ul)
    {
      boundary_.UL=ul;
      boundary_.LR=ul.operator+(tsize);
    }


    
    /**
     * @brief  returns if the tchObject was already being touched
     *
     * @param  nothing
     * @return boolean: true=object was being touched allready, false=object wasn't touched yet
     */
    boolean TchObject::WasBeingTouched()const
    {
       return wasBeingTouched_;
    }

    const String TchObject::Name() const
    {
      return name_;
    }
    void TchObject::Name(const String oname)
    {
      name_=oname;
    }

    const Adafruit_ILI9341& TchObject::TFT()const
    {
      return tft_;
    }
    boolean TchObject::On()const {return on_;}
    void TchObject::On(boolean on){on_=on;}

    int16_t TchObject::Value() const {return value_;}
    int16_t TchObject::Value(int16_t val)  
    {
      value_=val;
      return value_;
    }

//functions
    /**
     * @brief  get upper left corner of boundary
     *
     * @param  nothing
     * @return const TPoint&, upper left corner of boundary
     */
    TPoint TchObject::UL() const
    {
        return boundary_.UL;
    }
    
    
    /**
     * @brief  set upper left corner of boundary, without changing size
     *
     * @param TPoint ul: new location of upper left corner
     * @return nothing
     */
    void TchObject::UL(TPoint ul)
    {
        Move(TVector(ul.x-boundary_.UL.x,ul.y-boundary_.UL.y));
    }
    
    /**
     * @brief  get lower right corner of boundary
     *
     * @param  nothing
     * @return const TPoint&, lower right corner of boundary
     */
    TPoint TchObject::LR()const
    {
        return boundary_.LR;
    }
    
    /**
     * @brief  set upper left corner of boundary, without changing size
     *
     * @param TPoint lr: new location of lower right corner
     * @return nothing
     */
    void TchObject::LR(TPoint lr)
    {
        Move(TVector(lr.x-boundary_.LR.x,lr.y-boundary_.LR.y));
    }
   
    
    /**
     * @brief  get location (centre) of TchObject
     *
     * @param  nothing
     * @return TPoint, centre of TchObject
     */
    TPoint TchObject::Location() const
    {
        return TPoint(boundary_.UL.x+Width()/2,boundary_.UL.y+Height()/2);
    }
    
    /**
     * @brief  set location (centre) of TchObject
     *
     * @param  TPoint, centre of TchObject
     * @return Nothing
     */
    void TchObject::Location(TPoint val)
    {
        TPoint centre=Location();
        Move(TVector(val.x-centre.x,val.y-centre.y));
    }
    
    /**
     * @brief  set location (centre) of TchObject
     *
     * @param  uint16_t x, x value of the coordinate of the centre
     * @param  uint16_t y, y value of the coordinate of the centre
     * @return Nothing
     */
    void TchObject::Location(uint16_t x,uint16_t y)
    {
        TPoint centre(Location());
        Move(TVector(x-centre.x,y-centre.y));
    }
    
    /**
     * @brief  moves the TchObject according to TSize l
     *
     * @param  TSize val, containing the width and height for moving the TchObject
     * @return Nothing
     */
    void TchObject::Move(const TVector& v)
    {
        boundary_.UL.operator=(boundary_.UL.operator+(v));
        boundary_.LR.operator=(boundary_.LR.operator+(v));
    }

    uint16_t TchObject::Width()const
    {
      return abs(boundary_.LR.x-boundary_.UL.x);
    }
    uint16_t TchObject::Height()const
    {
      return abs(boundary_.LR.y-boundary_.UL.y);
    }
 
    /**
     * @brief  returns the current size of the TchObject
     *
     * @param  nothing
     * @return TSize, size of the tchobject
     */
    TSize TchObject::Size() const
    {
        return boundary_.LR.Distance(boundary_.UL);
    }
    /**
     * @brief  resizes the TchObject according to TSize, while maintaining the same location (centre)
     *
     * @param  TSize val, containing the width and height for resizing the TchObject
     * @return Nothing
     */
    void TchObject::Size(TSize val)
    {
        TPoint centre=Location();
        boundary_.UL=centre.operator-(TVector(val).operator*(0.5));
        boundary_.LR=centre.operator+(TVector(val).operator*(0.5));
    }
    /**
     * @brief  resizes the TchObject according to given width and height, while maintaining the same location (centre)
     *
     * @param  TSize val, containing the width and height for resizing the TchObject
     * @return Nothing
     */
    void TchObject::Size(uint16_t width,uint16_t height)
    {
        Size(TSize(width, height));
    }
    
    
    //functions
    /**
     * @brief  checks wether a point is inside the boundaries
     *
     * @param  TPoint p: the locationt to be checked
     * @return bool, true if p is inside (or on the edge) and false if p is located outside
     */
    bool TchObject::Inside(TPoint p)const
    {
        return boundary_.Inside(p);
    }
}
