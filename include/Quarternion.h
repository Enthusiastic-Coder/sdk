#pragma once
#include <math.h>
#include "vector3.h"
#include "vector4.h"

#include "MathSupport.h"
#include <limits>
#include <sstream>

template<typename T>
class Quarternion
{
public:
    T	n;	// number (scalar) part
    Vector3<T>	v;	// vector part: v.x, v.y, v.z

    const static Quarternion<T>& Q0()
    {
        static Quarternion<T> q0 = MathSupport<T>::MakeQ(0,0,0);
        return q0;
    }

    Quarternion(void);
    Quarternion(T e0, T e1, T e2, T e3);
    Quarternion(std::string str);

    std::string toString() const;

    void Reset();
    T	Magnitude(void) const;
    Vector3<T>	GetVector(void);
    T	GetScalar(void);
    Quarternion& operator+=(Quarternion q);
    Quarternion& operator-=(Quarternion q);
    Quarternion& operator*=(Quarternion q);
    Quarternion& operator*=(T s);
    Quarternion& operator/=(T s);
    Quarternion	operator~(void) const { return Quarternion(n, -v.x, -v.y, -v.z);}
    bool operator==(const Quarternion &q);
    Quarternion<T>& Normalize(void);
    bool IsNan() const;

    Quarternion<double> toDouble() const;
    Quarternion<float> toFloat() const;
};

template<typename T>
inline Quarternion<T>::Quarternion(void)
{
    n = 0;
    v.x = 0;
    v.y = 0;
    v.z = 0;
}

template<typename T>
inline Quarternion<T>::Quarternion(T e0, T e1, T e2, T e3)
{
    n = e0;
    v.x = e1;
    v.y = e2;
    v.z = e3;
}

template<typename T>
Quarternion<T>::Quarternion(std::string str)
{
    std::stringstream ss(str);
    ss.precision(9);
    ss >> n >> v.x >> v.y >> v.z;
}

template<typename T>
std::string Quarternion<T>::toString() const
{
    std::stringstream ss;
    ss.precision(9);
    ss << n << " " << v.x << " " << v.y << " " << v.z;
    return ss.str();
}

template<typename T>
inline void Quarternion<T>::Reset()
{
    n = 0;
    v.x = 0;
    v.y = 0;
    v.z = 0;
}

template<typename T>
inline Quarternion<double> Quarternion<T>::toDouble() const
{
    Quarternion<double> q;
    q.n = n;
    q.v = v.toDouble();
    return q;
}

template<typename T>
inline Quarternion<float> Quarternion<T>::toFloat() const
{
    Quarternion<float> q;
    q.n = n;
    q.v = v.toFloat();
    return q;
}


template<typename T>
inline T Quarternion<T>::Magnitude(void) const
{
    return (T) sqrt(n*n + v.x*v.x + v.y*v.y + v.z*v.z);
}

template<typename T>
inline Vector3<T> Quarternion<T>::GetVector(void)
{
    return Vector3<T>(v.x, v.y, v.z);
}

template<typename T>
inline T Quarternion<T>::GetScalar(void)
{
    return n;
}

template<typename T>
inline Quarternion<T>& Quarternion<T>::operator+=(Quarternion<T> q)
{
    n += q.n;
    v.x += q.v.x;
    v.y += q.v.y;
    v.z += q.v.z;
    return *this;
}

template<typename T>
inline Quarternion<T>& Quarternion<T>::operator-=(Quarternion<T> q)
{
    n -= q.n;
    v.x -= q.v.x;
    v.y -= q.v.y;
    v.z -= q.v.z;
    return *this;
}

template<typename T>
inline Quarternion<T>& Quarternion<T>::operator*=(Quarternion q)
{
    *this = *this * q;
    return *this;
}

template<typename T>
inline Quarternion<T>& Quarternion<T>::operator*=(T s)
{
    n *= s;
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return *this;
}

template<typename T>
inline Quarternion<T>& Quarternion<T>::operator/=(T s)
{
    n /= s;
    v.x /= s;
    v.y /= s;
    v.z /= s;
    return *this;
}

template<>
inline Quarternion<double>& Quarternion<double>::Normalize(void)
{
    double m = Magnitude();
    if(m <= std::numeric_limits<double>::epsilon()) m = 1;
    return *this /= m;
}

template<>
inline Quarternion<float>& Quarternion<float>::Normalize(void)
{
    float m = Magnitude();
    if(m <= std::numeric_limits<float>::epsilon()) m = 1;
    return *this /= m;
}

template<typename T>
inline Quarternion<T> operator+(Quarternion<T> q1, Quarternion<T> q2)
{
    return	Quarternion<T>(	q1.n + q2.n,
                                q1.v.x + q2.v.x,
                                q1.v.y + q2.v.y,
                                q1.v.z + q2.v.z);
}

template<typename T>
inline Quarternion<T> operator-(Quarternion<T> q1, Quarternion<T> q2)
{
    return	Quarternion<T>(	q1.n - q2.n,
                                q1.v.x - q2.v.x,
                                q1.v.y - q2.v.y,
                                q1.v.z - q2.v.z);
}

template<typename T>
inline Quarternion<T> operator*(Quarternion<T> q1, Quarternion<T> q2)
{
    return	Quarternion<T>(	q1.n*q2.n - q1.v.x*q2.v.x - q1.v.y*q2.v.y - q1.v.z*q2.v.z,
                                q1.n*q2.v.x + q1.v.x*q2.n + q1.v.y*q2.v.z - q1.v.z*q2.v.y,
                                q1.n*q2.v.y + q1.v.y*q2.n + q1.v.z*q2.v.x - q1.v.x*q2.v.z,
                                q1.n*q2.v.z + q1.v.z*q2.n + q1.v.x*q2.v.y - q1.v.y*q2.v.x);
}

template<typename T>
inline Quarternion<T> operator*(const Quarternion<T> & q, T s)
{
    return	Quarternion<T>(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

template<typename T>
inline Quarternion<T> operator*(T s, const Quarternion<T> & q)
{
    return	Quarternion<T>(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

template<typename T>
inline Quarternion<T> operator*(const Quarternion<T> & q, const Vector3<T> & v)
{
    return Quarternion<T>(	-(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
                                q.n*v.x + q.v.y*v.z - q.v.z*v.y,
                                q.n*v.y + q.v.z*v.x - q.v.x*v.z,
                                q.n*v.z + q.v.x*v.y - q.v.y*v.x);
}

template<typename T>
inline Quarternion<T> operator*(Vector3<T> v, Quarternion<T> q)
{
    return	Quarternion<T>(	-(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
                                q.n*v.x + q.v.z*v.y - q.v.y*v.z,
                                q.n*v.y + q.v.x*v.z - q.v.z*v.x,
                                q.n*v.z + q.v.y*v.x - q.v.x*v.y);
}

template<typename T>
inline Quarternion<T> operator/(const Quarternion<T> & q, T s)
{
    return	Quarternion<T>(q.n/s, q.v.x/s, q.v.y/s, q.v.z/s);
}

template<>
inline bool Quarternion<double>::operator==(const Quarternion<double> &q)
{
    return fabs(n - q.n) < std::numeric_limits<double>::epsilon()
            && fabs(v.x - q.v.x) < std::numeric_limits<double>::epsilon()
            && fabs(v.y - q.v.y) < std::numeric_limits<double>::epsilon()
            && fabs(v.z - q.v.z) < std::numeric_limits<double>::epsilon();
}

template<>
inline bool Quarternion<float>::operator==(const Quarternion<float> &q)
{
    return fabs(n - q.n) < std::numeric_limits<float>::epsilon()
            && fabs(v.x - q.v.x) < std::numeric_limits<float>::epsilon()
            && fabs(v.y - q.v.y) < std::numeric_limits<float>::epsilon()
            && fabs(v.z - q.v.z) < std::numeric_limits<float>::epsilon();
}

template<typename T>
inline bool Quarternion<T>::IsNan() const
{
    return std::isnan(n) || v.IsNan();
}


template<typename T>
inline T QGetAngle(const Quarternion<T> & q)
{
    return	(T) RadiansToDegrees((2*acos(q.n)));
}

template<typename T>
inline Vector3<T> QGetAxis(const Quarternion<T> &q)
{
    Vector3<T> v;
    T m;

    v = q.GetVector();
    m = v.Magnitude();

    if (fabs(m) <= std::numeric_limits<float>::epsilon())
        return Vector3<T>();
    else
        return v/m;
}

template<typename T>
inline Quarternion<T> QRotate(const Quarternion<T>& q1, const Quarternion<T> & q2)
{
    return	q1*q2*(~q1);
}

template<typename T>
inline Vector3<T> QVRotate(const Quarternion<T>& q, const Vector3<T>& v)
{
    Quarternion<T> t;
    t = q*v*(~q);
    return t.GetVector();
}

template<typename T>
inline Vector3<T> QVRotate(const Quarternion<T>& q, const Vector4<T>& v4)
{
    Vector3<T> v(v4.x, v4.y, v4.z);
    Quarternion<T> t;
    t = q*v*(~q);
    return t.GetVector();
}




typedef Quarternion<float> QuarternionF;
typedef Quarternion<double> QuarternionD;
