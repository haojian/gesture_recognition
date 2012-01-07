#ifndef GESTURE_MATH_H
#define GESTURE_MATH_H

#include <math.h>
#include <limits>
#include <cmath>

#include "gesture_DS.h"


const float PI = 3.14159f;
const float PI_2 = PI * 2.0f;

//! minimum of two
template<typename T>
inline const T min2( const T& a, const T& b )
{
    return a < b ? a : b;
}

//! maximum of two
template<typename T>
inline const T max2( const T& a, const T& b )
{
    return a > b ? a : b;
}

//! minimum of three
template<typename T>
inline const T min3( const T& a, const T& b, const T& c )
{
    return min2( min2(a, b), c );
}

//! maximum of three
template<typename T>
inline const T max3( const T& a, const T& b, const T& c )
{
    return max2( max2(a, b), c );
}

//! absolute value
template<typename T>
inline const T abs_( const T& x )
{
    return x > 0 ? x : -x;
}

//! sign of the value
template<typename T>
inline const T sign( const T& x )
{
    return x>0 ? 1 : ((x==0) ? 0 : -1);
}

//! square
template<typename T>
inline const T sq( const T& x )
{
    return x * x;
}


#endif // GESTURE_MATH_H
