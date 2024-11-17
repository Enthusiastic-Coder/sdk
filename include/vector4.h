#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Vector4 class
//
////////////////////////////////////////////////////////////////////////////////////////////////
#include <limits>
#include <vector3.h>

template<class T>
class Matrix4x4;


template<class T>
class Vector4
{
    static Vector4<T> _s_NULL;
public:
    T x;
    T y;
    T z;
    T w;

    static Vector4<T>& Null()	{ return _s_NULL; }

    Vector4()               { *this = Vector4(0,0,0,1); }
    Vector4(T x, T y)       { *this = Vector4(x,y,0,1); }
    Vector4(T x, T y, T z)  { *this = Vector4(x,y,z,1); }
    Vector4(Vector3<T> v, T w)	{ *this = Vector4(v.x, v.y, v.z, w); }
    Vector4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vector4(const Vector3<T> &rhs)
    {
        operator=(rhs);
    }

    T Magnitude() const
    {
        return sqrt( x * x + y * y + z * z + w * w );
    }

    void Normalize()
    {
        T magnitude = Magnitude();
        if( fabs(magnitude) < std::numeric_limits<float>::epsilon() )
            return;

        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
        w /= magnitude;
    }

    Vector4<T> Unit()
    {
        Vector4<T> t(*this);
        t.Normalize();
        return t;
    }

    Vector4<T>& operator=( const Vector3<T> &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = T(0.0);
        return *this;
    }

    bool operator==(const Vector4<double>& v)
    {
        return fabs(x - v.x) < std::numeric_limits<double>::epsilon() && fabs(y - v.y) < std::numeric_limits<double>::epsilon() && fabs(z - v.z ) < std::numeric_limits<double>::epsilon() && fabs(w - v.w ) < std::numeric_limits<double>::epsilon();
    }

    bool operator==(const Vector4<float>& v)
    {
        return fabs(x - v.x) < std::numeric_limits<float>::epsilon() && fabs(y - v.y) < std::numeric_limits<float>::epsilon() && fabs(z - v.z ) < std::numeric_limits<float>::epsilon() && fabs(w - v.w ) < std::numeric_limits<float>::epsilon();
    }

    bool operator!=(const Vector4<T>& v)
    {
        return !operator==(v);
    }

    Vector4<T> operator-()
    {
        return Vector4<T>(-x, -y, -z, w );
    }

    Vector4<T>& operator-=(const Vector4<T> &v)
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    Vector4<T> operator+(const Vector4<T>& v)
    {
        return Vector4<T>(x + v.x, y + v.y, z + v.z, w );
    }

    Vector4<T>& operator+=(const Vector4<T> &v)
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector4<T> operator-(const Vector4<T>& v)
    {
        return Vector4<T>(x - v.x, y - v.y, z - v.z, w );
    }

    Vector4<T> operator*(T val)
    {
        return Vector4<T>(x * val, y * val, z * val, w * val );
    }

    Vector4<T>& operator*=(T val)
    {
        x *= val; y *= val; z *= val; w *= val;
        return *this;
    }

    Vector4<T> operator/(T val)
    {
        return Vector4<T>(x / val, y / val, z / val, w / val);
    }

    Vector4<T>& operator/=(T val)
    {
        return x /= val, y /= val, z /= val, w /= val,*this;
    }

    T operator*(const Vector4<T>& v) const	//Dot Product
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    Vector4<T> operator^(const Vector4<T>& v) //Cross Product
    {
        return Vector4<T>(this->y * v.z - this->z * v.y,
                          this->z * v.x - this->x * v.z,
                          this->x * v.y - this->y * v.x,
                          1 );
    }

    T* ptr()
    {
        return reinterpret_cast<T*>(this);
    }

    const T* ptr() const
	{
		return reinterpret_cast<const T*>(this);
	}

    Vector4<double> toDouble()
    {
        return Vector4<double>(x, y, z, w);
    }

};
template<typename T>
Vector4<T> Vector4<T>::_s_NULL = Vector4<T>();

typedef Vector4<float> Vector4F;
typedef Vector4<double> Vector4D;
