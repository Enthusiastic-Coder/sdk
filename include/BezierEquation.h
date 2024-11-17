#pragma once

#include "MathSupport.h"

template<typename T>
class BezierEquation
{
public:
    BezierEquation()
        : _ax(0.0)
        , _bx(0.0)
        , _cx(0.0)
        , _ay(0.0)
        , _by(0.0)
        , _cy(0.0)
    {
    }

    void calculate( Vector<T> p0, Vector<T> p1, Vector<T> p2, Vector<T> p3)
    {
        _p0 = p0;

        _cx = 3 * (p1.x - p0.x);
        _bx = 3 * (p2.x - p1.x) - _cx;
        _ax = p3.x - p0.x - _cx - _bx;

        _cy = 3 * (p1.y - p0.y);
        _by = 3 * (p2.y - p1.y) - _cy;
        _ay = p3.y - p0.y - _cy - _by;
    }

    Vector<T> getValue( T t )
    {
        return Vector<T>(  _ax * t * t * t + _bx * t * t + _cx * t + _p0.x, 0,
                           _ay * t * t * t + _by * t * t + _cy * t + _p0.y);
    }

private:
    T _ax, _bx, _cx;
    T _ay, _by, _cy;

    Vector<T> _p0;
};
