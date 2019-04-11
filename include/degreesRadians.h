#pragma once


#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

// standard angle measures
const float FLOAT_PI = 3.14159265359f;
const float ONEDEGREE = FLOAT_PI/180.00f;
const float FOURDEGREES = ONEDEGREE * 4.0f;
const float _0DEGREES = 0.0f * ONEDEGREE;
const float _90DEGREES = 90.0f * ONEDEGREE;
const float _180DEGREES = 180.0f * ONEDEGREE;
const float _270DEGREES = 270.0f * ONEDEGREE;
const float _360DEGREES = 360.0f * ONEDEGREE;

#ifndef M_PI
#define M_PI 3.14159265359
#endif

//
// inline float DegreesToRadians(float deg)
//
// converts degrees to radians
template<class T>
inline T DegreesToRadians(T deg)
{
    return deg * (FLOAT_PI / 180.0f);
}

inline float D2R(float deg)
{
    return DegreesToRadians(deg);
}

inline double D2R(double deg)
{
    return DegreesToRadians(deg);
}

//
// converts radians to degrees
template<class T>
inline T RadiansToDegrees(T rad)
{
    return rad * (180.0f / FLOAT_PI);
}

inline float R2D(float rad)
{
    return RadiansToDegrees(rad);
}

inline double R2D(double rad)
{
    return RadiansToDegrees(rad);
}

