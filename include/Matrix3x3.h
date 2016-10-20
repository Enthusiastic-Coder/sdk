#pragma once

template<typename T> class Matrix4x4;
////////////////////////////////////////////////////////////////////////////////////////////////
//
// 3 x 3 Matrix class
//
////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Matrix3x3
{
public:
    // elements eij: i -> row, j -> column
    T	e11, e12, e13, e21, e22, e23, e31, e32, e33;

    // constructor
    Matrix3x3(void);
    Matrix3x3(	T r1c1, T r1c2, T r1c3,
                T r2c1, T r2c2, T r2c3,
                T r3c1, T r3c2, T r3c3 );
    Matrix3x3(	const Matrix4x4<T> &);

    T		det(void);
    Matrix3x3<T>	Transpose(void) const;
    Matrix3x3<T>	Inverse(void) const;
    void		Reset();

    Matrix3x3& LoadIdentity();

    const T* ptr() const
    {
        return reinterpret_cast<const T*>(this);
    }

    Matrix3x3& operator+=(const Matrix3x3<T> &m);
    Matrix3x3& operator-=(const Matrix3x3<T> &m);
    Matrix3x3& operator*=(const Matrix3x3<T> &m);
    Matrix3x3& operator*=(T s);
    Matrix3x3& operator/=(T s);

    Matrix3x3<double> toDouble()
    {
        Matrix3x3<double> mat;
        mat.e11 = (double)e11;
        mat.e12 = (double)e12;
        mat.e13 = (double)e13;

        mat.e21 = (double)e21;
        mat.e22 = (double)e22;
        mat.e23 = (double)e23;

        mat.e31 = (double)e31;
        mat.e32 = (double)e32;
        mat.e33 = (double)e33;

        return mat;
    }

    Matrix3x3<float> toFloat()
    {
        Matrix3x3<float> mat;
        mat.e11 = (float)e11;
        mat.e12 = (float)e12;
        mat.e13 = (float)e13;

        mat.e21 = (float)e21;
        mat.e22 = (float)e22;
        mat.e23 = (float)e23;

        mat.e31 = (float)e31;
        mat.e32 = (float)e32;
        mat.e33 = (float)e33;

        return mat;
    }
};

template<typename T>
inline Matrix3x3<T>::Matrix3x3(void)
{
    Reset();
}

template<typename T>
inline Matrix3x3<T>::Matrix3x3(	T r1c1, T r1c2, T r1c3,
                                T r2c1, T r2c2, T r2c3,
                                T r3c1, T r3c2, T r3c3 )
{
    e11 = r1c1;
    e12 = r1c2;
    e13 = r1c3;
    e21 = r2c1;
    e22 = r2c2;
    e23 = r2c3;
    e31 = r3c1;
    e32 = r3c2;
    e33 = r3c3;
}

#include <Matrix4x4.h>

template<typename T>
inline Matrix3x3<T>::Matrix3x3(	const Matrix4x4<T> &mat4 )
{
    e11 = mat4.m11;
    e12 = mat4.m12;
    e13 = mat4.m13;

    e21 = mat4.m21;
    e22 = mat4.m22;
    e23 = mat4.m23;

    e31 = mat4.m31;
    e32 = mat4.m32;
    e33 = mat4.m33;
}

template<typename T>
inline void	Matrix3x3<T>::Reset()
{
    e11 = 0;
    e12 = 0;
    e13 = 0;
    e21 = 0;
    e22 = 0;
    e23 = 0;
    e31 = 0;
    e32 = 0;
    e33 = 0;
}

template<typename T>
inline T Matrix3x3<T>::det(void)
{
    return	e11*e22*e33 -
            e11*e32*e23 +
            e21*e32*e13 -
            e21*e12*e33 +
            e31*e12*e23 -
            e31*e22*e13;
}

template<typename T>
inline Matrix3x3<T> Matrix3x3<T>::Transpose(void) const
{
    return Matrix3x3<T>(e11,e21,e31,e12,e22,e32,e13,e23,e33);
}

template<typename T>
inline Matrix3x3<T> Matrix3x3<T>::Inverse(void) const
{
    T	d = e11*e22*e33 -
            e11*e32*e23 +
            e21*e32*e13 -
            e21*e12*e33 +
            e31*e12*e23 -
            e31*e22*e13;

    if (fabs(d) < std::numeric_limits<float>::epsilon() )
        d = 1;

    return	Matrix3x3<T>(	(e22*e33-e23*e32)/d,
                                -(e12*e33-e13*e32)/d,
                                (e12*e23-e13*e22)/d,
                                -(e21*e33-e23*e31)/d,
                                (e11*e33-e13*e31)/d,
                                -(e11*e23-e13*e21)/d,
                                (e21*e32-e22*e31)/d,
                                -(e11*e32-e12*e31)/d,
                                (e11*e22-e12*e21)/d );
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::LoadIdentity()
{
    e11 = 1;
    e12 = 0;
    e13 = 0;
    e21 = 0;
    e22 = 1;
    e23 = 0;
    e31 = 0;
    e32 = 0;
    e33 = 1;
    return *this;
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T> &m)
{
    e11 = e11 * m.e11 + e12 * m.e21 + e13 * m.e31;
    e12 = e11 * m.e12 + e12 * m.e22 + e13 * m.e32;
    e13 = e11 * m.e13 + e12 * m.e23 + e13 * m.e33;

    e21 = e21 * m.e11 + e22 * m.e21 + e23 * m.e31;
    e22 = e21 * m.e12 + e22 * m.e22 + e23 * m.e32;
    e23 = e21 * m.e13 + e22 * m.e23 + e23 * m.e33;

    e31 = e31 * m.e11 + e32 * m.e21 + e33 * m.e31;
    e32 = e31 * m.e12 + e32 * m.e22 + e33 * m.e32;
    e33 = e31 * m.e13 + e32 * m.e23 + e33 * m.e33;

    return *this;
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T> &m)
{
    e11 += m.e11;
    e12 += m.e12;
    e13 += m.e13;
    e21 += m.e21;
    e22 += m.e22;
    e23 += m.e23;
    e31 += m.e31;
    e32 += m.e32;
    e33 += m.e33;
    return *this;
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T> &m)
{
    e11 -= m.e11;
    e12 -= m.e12;
    e13 -= m.e13;
    e21 -= m.e21;
    e22 -= m.e22;
    e23 -= m.e23;
    e31 -= m.e31;
    e32 -= m.e32;
    e33 -= m.e33;
    return *this;
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::operator*=(T s)
{
    e11 *= s;
    e12 *= s;
    e13 *= s;
    e21 *= s;
    e22 *= s;
    e23 *= s;
    e31 *= s;
    e32 *= s;
    e33 *= s;
    return *this;
}

template<typename T>
inline Matrix3x3<T>& Matrix3x3<T>::operator/=(T s)
{
    e11 /= s;
    e12 /= s;
    e13 /= s;
    e21 /= s;
    e22 /= s;
    e23 /= s;
    e31 /= s;
    e32 /= s;
    e33 /= s;
    return *this;
}

template<typename T>
inline Matrix3x3<T> operator+(Matrix3x3<T> m1, Matrix3x3<T> m2)
{
    return	Matrix3x3<T>(	m1.e11+m2.e11,
                                m1.e12+m2.e12,
                                m1.e13+m2.e13,
                                m1.e21+m2.e21,
                                m1.e22+m2.e22,
                                m1.e23+m2.e23,
                                m1.e31+m2.e31,
                                m1.e32+m2.e32,
                                m1.e33+m2.e33);
}

template<typename T>
inline Matrix3x3<T> operator-(Matrix3x3<T> m1, Matrix3x3<T> m2)
{
    return	Matrix3x3<T>(	m1.e11-m2.e11,
                                m1.e12-m2.e12,
                                m1.e13-m2.e13,
                                m1.e21-m2.e21,
                                m1.e22-m2.e22,
                                m1.e23-m2.e23,
                                m1.e31-m2.e31,
                                m1.e32-m2.e32,
                                m1.e33-m2.e33);
}

template<typename T>
inline Matrix3x3<T> operator/(Matrix3x3<T> m, T s)
{
    return	Matrix3x3<T>(	m.e11/s,
                                m.e12/s,
                                m.e13/s,
                                m.e21/s,
                                m.e22/s,
                                m.e23/s,
                                m.e31/s,
                                m.e32/s,
                                m.e33/s);
}

template<typename T>
inline Matrix3x3<T> operator*(Matrix3x3<T> m1, Matrix3x3<T> m2)
{
    return Matrix3x3<T>(m1.e11*m2.e11 + m1.e12*m2.e21 + m1.e13*m2.e31,
                        m1.e11*m2.e12 + m1.e12*m2.e22 + m1.e13*m2.e32,
                        m1.e11*m2.e13 + m1.e12*m2.e23 + m1.e13*m2.e33,
                        m1.e21*m2.e11 + m1.e22*m2.e21 + m1.e23*m2.e31,
                        m1.e21*m2.e12 + m1.e22*m2.e22 + m1.e23*m2.e32,
                        m1.e21*m2.e13 + m1.e22*m2.e23 + m1.e23*m2.e33,
                        m1.e31*m2.e11 + m1.e32*m2.e21 + m1.e33*m2.e31,
                        m1.e31*m2.e12 + m1.e32*m2.e22 + m1.e33*m2.e32,
                        m1.e31*m2.e13 + m1.e32*m2.e23 + m1.e33*m2.e33 );
}

template<typename T>
inline Matrix3x3<T> operator*(Matrix3x3<T> m, T s)
{
    return	Matrix3x3<T>(m.e11*s,
                             m.e12*s,
                             m.e13*s,
                             m.e21*s,
                             m.e22*s,
                             m.e23*s,
                             m.e31*s,
                             m.e32*s,
                             m.e33*s);
}

template<typename T>
inline Matrix3x3<T> operator*(T s, Matrix3x3<T> m)
{
    return	Matrix3x3<T>(	m.e11*s,
                                m.e12*s,
                                m.e13*s,
                                m.e21*s,
                                m.e22*s,
                                m.e23*s,
                                m.e31*s,
                                m.e32*s,
                                m.e33*s);
}

template<typename T>
inline Vector3<T> operator*(Matrix3x3<T> m, Vector3<T> u)
{
    return Vector3<T>(	m.e11*u.x + m.e12*u.y + m.e13*u.z,
                        m.e21*u.x + m.e22*u.y + m.e23*u.z,
                        m.e31*u.x + m.e32*u.y + m.e33*u.z);
}

template<typename T>
inline Vector3<T> operator*(Vector3<T> u, Matrix3x3<T> m)
{
    return Vector3<T>(	u.x*m.e11 + u.y*m.e21 + u.z*m.e31,
                        u.x*m.e12 + u.y*m.e22 + u.z*m.e32,
                        u.x*m.e13 + u.y*m.e23 + u.z*m.e33);
}


typedef Matrix3x3<float> Matrix3x3F;
typedef Matrix3x3<double> Matrix3x3D;
