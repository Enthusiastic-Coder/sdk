#pragma once

#include "Quarternion.h"
#include <jibbs/vector/vector3.h>

template<typename T>
class Plane
{
public:

    Vector3<T> N;
    T D;
    Quarternion<T> Q;

    Plane()
    {
        Set(0,0,0,0);
    }

    Plane( float a, float b, float c, float d = 0)
    {
        Set(a,b,c,d);
    }

    Plane(const Plane &rhs )
    {
        N = rhs.N;
        D = rhs.D;
        Q = rhs.Q;
    }

    Plane( const Vector3F &n, const Vector3F &pt=Vector3F() )
    {
        Set( n, pt );
    }

    void Set( const Vector3F& n, const Vector3F &pt=Vector3F() )
    {
        Vector3F N = n;
        N.Normalize();
        Set( N.x, N.y, N.z, -N * pt );
    }

    void Set( float a, float b, float c, float d )
    {
        N = Vector3<T>(a,b,c);
        D = d;
        Q = MathSupport<T>::MakeQ( *this );
    }

    float Magnitude() const
    {
        return N.Magnitude();
    }

    Plane operator*(float t) const
    {
        Plane copy(*this);
        copy *= t;
        return copy;
    }

    Plane& operator*=(float t)
    {
        N *= t;
        D *= t;
        return *this;
    }

    Plane operator/(float t ) const
    {
        Plane ret(*this);
        return ret/=t;
    }

    Plane& operator/=(float t )
    {
        N /= t;
        D /= t;
        return *this;
    }

    Plane& operator-=(const Plane & rhs )
    {
        N -= rhs.N;
        D -= rhs.D;
    }

    Plane operator-(const Plane & rhs ) const
    {
        Plane copy(*this);
        copy.N -= rhs.N;
        copy.D -= rhs.D;
        return copy;
    }

    Plane& operator+=(const Plane & rhs )
    {
        N += rhs.N;
        D += rhs.D;
    }

    Plane operator+(const Plane & rhs ) const
    {
        Plane copy(*this);
        copy.N += rhs.N;
        copy.D += rhs.D;
        return copy;
    }

    Plane Unit() const
    {
        return operator/(Magnitude() );
    }

    void Normalize()
    {
        operator/=(Magnitude() );
    }

    float Height(const Vector3F& pos) const
    {
        return (N.x * pos.x + N.z * pos.z + D ) / -N.y;
    }

    float HeightAbovePlane(const Vector3F& pos) const
    {
        return QVRotate( ~GetQ(), Vector3F(0, pos.y - Height(pos), 0)  ).y;
    }

    const Quarternion<float>& GetQ() const
    {
        return Q;
    }
};


typedef Plane<float> PlaneF;
typedef Plane<double> PlaneD;
