#pragma once

#include <utility>

template<typename T>
struct Vector2
{
    Vector2();
    Vector2( std::pair<T,T> v );
    Vector2(T x,T z);
    Vector2 operator/(T n) const;
    Vector2& operator/=(T n);
    Vector2& operator+=(const Vector2 &rhs);
    Vector2 operator+(const Vector2 &rhs) const;
    Vector2& operator-=(const Vector2 &rhs);
    Vector2 operator-(const Vector2 &rhs) const;
    bool operator==(const Vector2& rhs) const;
    T x,z;
};

//////////////////////////////////////////////////
template<typename T>
Vector2<T>::Vector2()
{
    x = z = 0;
}

template<typename T>
Vector2<T>::Vector2( std::pair<T,T> v )
    :x(v.first), z(v.second )
{
}

template<typename T>
Vector2<T>::Vector2(T x,T z)
{
    this->x = x;
    this->z = z;
}

template<typename T>
Vector2<T> Vector2<T>::operator/(T n) const
{
    Vector2<T> ret(*this);
    ret /= n;
    return ret;
}

template<typename T>
Vector2<T>& Vector2<T>::operator/=(T n)
{
    x /= n;
    z /= n;
    return *this;
}

template<typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2 &rhs)
{
    x += rhs.x;
    z += rhs.z;
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2 &rhs) const
{
    Vector2 ret(*this);
    return ret += rhs;
}

template<typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2 &rhs)
{
    x -= rhs.x;
    z -= rhs.z;
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2 &rhs) const
{
    Vector2<T> ret(*this);
    return ret -= rhs;
}

template<typename T>
bool Vector2<T>::operator==(const Vector2& rhs) const
{
    return this->x == rhs.x && this->z == rhs.z;
}


typedef Vector2<float> Vector2F;
typedef Vector2<int> Vector2I;
typedef Vector2<double> Vector2D;
