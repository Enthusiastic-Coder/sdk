#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Vector class
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <cmath>

template<typename T>
class Vector3
{
public:
    // magnitude along i, j, k axes.
    T x;
    T y;
    T z;

    // constructor
    Vector3(void);
    Vector3(T xi, T yi, T zi);

    T Magnitude(void) const;
    T MagnitudeXZ(void) const;
    Vector3<T>& Normalize(void);
    Vector3<T> Unit() const;
    void  Reverse(void);
    T SumComponents();
    void Reset();

    Vector3<T>& operator+=(const Vector3<T>& u);	// vector addition
    Vector3<T>& operator-=(const Vector3<T>& u);	// vector subtraction
    Vector3<T>& operator*=(T s);	// scalar multiply
    Vector3<T>& operator/=(T s);	// scalar divide

    Vector3<T> operator-(void) const;
    bool operator<(const Vector3<T> &rhs) const;
    bool operator==(const Vector3<T> &rhs) const;
    bool operator!();

    Vector3<T>& get()
    {
        return *this;
    }

    bool IsNan() const
    {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    Vector3<float> toFloat() const
    {
        return Vector3<float>(float(x), float(y), float(z) );
    }

    Vector3<double> toDouble() const
    {
        return Vector3<double>(x, y, z );
    }

    T* ptr()
    {
        return reinterpret_cast<T*>(this);
    }

    static Vector3<T>& Null() { return _s_NULL; }
    static Vector3<T> _s_NULL;
    static Vector3<T> _s_i;
    static Vector3<T> _s_j;
    static Vector3<T> _s_k;
};

template<typename T>
Vector3<T> Vector3<T>::_s_NULL = Vector3<T>();

template<typename T>
Vector3<T> Vector3<T>::_s_i = Vector3<T>(1,0,0);

template<typename T>
Vector3<T> Vector3<T>::_s_j = Vector3<T>(0,1,0);

template<typename T>
Vector3<T> Vector3<T>::_s_k = Vector3<T>(0,0,1);

template<typename T>
inline Vector3<T>::Vector3(void)
{
    Reset();
}

template<typename T>
inline Vector3<T>::Vector3(T xi, T yi, T zi)
{
    x = xi;
    y = yi;
    z = zi;
}

template <typename T>
inline void Vector3<T>::Reset()
{
    x = 0;
    y = 0;
    z = 0;
}

template<typename T>
inline	T Vector3<T>::Magnitude(void) const
{
    return sqrt(x*x + y*y + z*z);
}

template<typename T>
inline	T Vector3<T>::MagnitudeXZ(void) const
{
    return sqrt(x*x + z*z);
}

template<typename T>
inline	Vector3<T> Vector3<T>::Unit() const
{
    return Vector3<T>(*this).Normalize();
}

template<>
inline	Vector3<double>&  Vector3<double>::Normalize(void)
{
    double m = Magnitude();
    if(m <= std::numeric_limits<double>::epsilon()) m = 1;
    return operator/=(m);
}

template<>
inline	Vector3<float>&  Vector3<float>::Normalize(void)
{
    float m = Magnitude();
    if(m <= std::numeric_limits<float>::epsilon()) m = 1;
    return operator/=(m);
}

template<typename T>
inline	void  Vector3<T>::Reverse(void)
{
    x = -x;
    y = -y;
    z = -z;
}

template<typename T>
inline T Vector3<T>::SumComponents()
{
    return x + y + z;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& u)
{
    x += u.x;
    y += u.y;
    z += u.z;
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& u)
{
    x -= u.x;
    y -= u.y;
    z -= u.z;
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(T s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-(void) const
{
    return Vector3(-x, -y, -z);
}

template<typename T>
inline Vector3<T> operator+(const Vector3<T>& u, const Vector3<T>& v)
{
    return Vector3<T>(u.x + v.x, u.y + v.y, u.z + v.z);
}

template<typename T>
inline Vector3<T> operator-(const Vector3<T>& u, const Vector3<T>& v)
{
    return Vector3<T>(u.x - v.x, u.y - v.y, u.z - v.z);
}

template<typename T>
// Vector cross product (u cross v)
inline Vector3<T> operator^(const Vector3<T>& u, const Vector3<T>& v)
{
    return Vector3<T>(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x );
}

template<typename T>
// Vector cross product (u cross v) amended for flight sim
inline Vector3<T> operator%(Vector3<T> u, Vector3<T> v)
{
    v.z = -v.z;
    return u ^ v;
}

template<typename T>
// Vector dot product
inline T operator*(const Vector3<T>& u, const Vector3<T>& v)
{
    return (u.x*v.x + u.y*v.y + u.z*v.z);
}

template<typename T>
inline Vector3<T> operator*(const Vector3<T>& u, T s)
{
    return Vector3<T>(u.x*s, u.y*s, u.z*s);
}

template<typename T>
inline Vector3<T> operator*(T s, const Vector3<T>& u)
{
    return Vector3<T>(u.x*s, u.y*s, u.z*s);
}

template<typename T>
inline Vector3<T> operator/(const Vector3<T>& u, T s)
{
    return Vector3<T>(u.x/s, u.y/s, u.z/s);
}

template<typename T>
inline bool Vector3<T>::operator<(const Vector3<T> &rhs) const
{
    return Magnitude() < rhs.Magnitude();
}

template<>
inline bool Vector3<double>::operator==(const Vector3<double> &rhs) const
{
    return fabs(x - rhs.x) < std::numeric_limits<double>::epsilon() && fabs(y - rhs.y) < std::numeric_limits<double>::epsilon() && fabs(z - rhs.z) < std::numeric_limits<double>::epsilon();
}

template<>
inline bool Vector3<float>::operator==(const Vector3<float> &rhs) const
{
    return fabs(x - rhs.x) < std::numeric_limits<float>::epsilon() && fabs(y - rhs.y) < std::numeric_limits<float>::epsilon() && fabs(z - rhs.z) < std::numeric_limits<float>::epsilon();
}

template<typename T>
inline bool Vector3<T>::operator !()
{
    return x == T(0) && y == T(0) && z == T(0);
}

// triple scalar product (u dot (v cross w))
template<typename T>
inline T TripleScalarProduct(const Vector3<T>& u, const Vector3<T>& v, const Vector3<T>& w)
{
    return T(	(u.x * (v.y*w.z - v.z*w.y)) +
                (u.y * (-v.x*w.z + v.z*w.x)) +
                (u.z * (v.x*w.y - v.y*w.x)) );
}

//
// inline float VectorIntersectsPlane(...)
//
// returns true if vector between 2 position
// vectors intersects a plabne (specified by three points)
template<typename T>
bool VectorIntersectsPlane(Vector3<T> &v1, Vector3<T> &v2,
                                                   T &x1, T &y1, T &z1,
                                                   T &x2, T &y2, T &z2,
                                                   T &x3, T &y3, T &z3)
{
        // WIP
    return false;
}

typedef Vector3<float> Vector3F;
typedef Vector3<double> Vector3D;
