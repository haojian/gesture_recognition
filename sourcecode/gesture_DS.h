#ifndef GESTURE_DS_H
#define GESTURE_DS_H

#include <vector>
#include <iostream>
#include "gesture_Math.h"

using namespace std;




/////////////////////////////////////////
///Basic Graphic DataStructure
/////////////////////////////////////////

class Point
{
public:
    Point(float xval=0.0, float yval=0.0)
    {x = xval; y= yval;}
    float x, y;

    inline float dist(int xval, int yval) const
    {
        return sqrt(sq(x-xval) + sq(y-yval) );
    }
};

///////////////////////////////////////////////////////////
//Point Mathmatic Calculation
///////////////////////////////////////////////////////////
inline bool normalize(Point &a)
{
    float len =sqrt(sq(a.x) + sq(a.y));
    if(len >= 0)
    {
        a = Point(a.x/len, a.y/len);
        return true;
    }
    return false;
}

inline float dot(const Point &a, const Point &b)
{
    return a.x*b.x + a.y*b.y;
}

class BoundingBox
{
public:
    BoundingBox()
    {
        reset();
    }
    inline void addPoint(float xval, float yval)
    {
        minx = minx >0 ? min2( minx, xval ): xval;
        miny = miny >0 ? min2( miny, yval ): yval;
        maxx = maxx >0 ? max2( maxx, xval ): xval;
        maxy = maxy >0 ? max2( maxy, yval ): yval;
    }

    inline Point getCenter() const
    {
        return Point(0.5f*(minx+maxx), 0.5f*(miny+maxy));
    }

    inline Point getMinCornerPoint(){ return Point(minx , miny);}
    inline Point getMaxCornerPoint(){ return Point(maxx , maxy);}

    inline void reset(){minx = 0;
                        miny = 0;
                        maxx = 0;
                        maxy = 0;}
private:
    float minx, miny;
    float maxx, maxy;

};

/////////////////////////////////////////
///Gesture Render and utilities
/////////////////////////////////////////
enum E_GESTURE_TYPE
{
    EGES_INVALID = 0,
    EGES_POINT,                 // The gesture is point type
    EGES_LINE,                  // The gesture is line type
    EGES_CIRCLE,                // The gesture is circle type
    EGES_ERASE,
    EGES_POLYLINE
};

/////////////////////////////////////////
///Gesture Recognizer Data Structure
/////////////////////////////////////////

enum Direction_Type
{
    Up = 0,
    Down,
    Left,
    Right,
    Up_Left,
    Up_Right,
    Down_Right,
    Down_Left,
    AnyHorizontal, AnyVertical, NoMatch
};


typedef std::vector<Point> PosList;
typedef std::vector<Direction_Type> DirectionList;
#endif // GESTURE_DS_H
