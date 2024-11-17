#pragma once

template<typename T>
class Euler
{
public:
    Euler() :
        _pitch(0),_heading(0),_bank(0)
    {}
    Euler(T p, T h, T b)
    {
        _pitch = p;
        _heading = h;
        _bank = b;
    }
    T _pitch;
    T _heading;
    T _bank;
};

typedef Euler<float> EulerF;
typedef Euler<double> EulerD;
