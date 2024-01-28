#pragma once
#include "Frustum.h"
#include "GPSLocation.h"
#include "vector3.h"
#include <limits>
#include "Matrix3x3.h"

template<class T=MathSupportType>
class Matrix4x4
{
public:
    T m11;
    T m12;
    T m13;
    T m14;
    T m21;
    T m22;
    T m23;
    T m24;
    T m31;
    T m32;
    T m33;
    T m34;
    T m41;
    T m42;
    T m43;
    T m44;

    int frustumScale[6];

    Matrix3x3<T> to3x3() const
    {
        Matrix3x3<T> m;
        m.e11 = m11;
        m.e12 = m12;
        m.e13 = m13;

        m.e21 = m21;
        m.e22 = m22;
        m.e23 = m23;

        m.e31 = m31;
        m.e32 = m32;
        m.e33 = m33;

        return m;
    }

    Matrix4x4<float> toFloat() const
    {
        Matrix4x4<float> m;

        m.m11 = static_cast<float>(m11);
        m.m12 = static_cast<float>(m12);
        m.m13 = static_cast<float>(m13);
        m.m14 = static_cast<float>(m14);

        m.m21 = static_cast<float>(m21);
        m.m22 = static_cast<float>(m22);
        m.m23 = static_cast<float>(m23);
        m.m24 = static_cast<float>(m24);

        m.m31 = static_cast<float>(m31);
        m.m32 = static_cast<float>(m32);
        m.m33 = static_cast<float>(m33);
        m.m34 = static_cast<float>(m34);

        m.m41 = static_cast<float>(m41);
        m.m42 = static_cast<float>(m42);
        m.m43 = static_cast<float>(m43);
        m.m44 = static_cast<float>(m44);

        return m;
    }

    T operator[](int i) const
    {
        return (&m11)[i];
    }

    Matrix4x4()
    {
        Reset();
    }

    Matrix4x4<T>(T* values)
    {
        m11 = values[0];
        m12 = values[1];
        m13 = values[2];
        m14 = values[3];

        m21 = values[4];
        m22 = values[5];
        m23 = values[6];
        m24 = values[7];

        m31 = values[8];
        m32 = values[9];
        m33 = values[10];
        m34 = values[11];

        m41 = values[12];
        m42 = values[13];
        m43 = values[14];
        m44 = values[15];
    }

    void Reset()
    {
        m11 = 0;
        m12 = 0;
        m13 = 0;
        m14 = 0;
        m21 = 0;
        m22 = 0;
        m23 = 0;
        m24 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 0;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 0;

        frustumScale[Frustum::Left] = 1;
        frustumScale[Frustum::Right] = -1;
        frustumScale[Frustum::Top] = 2;
        frustumScale[Frustum::Bottom] = -2;
        frustumScale[Frustum::Near] = 3;
        frustumScale[Frustum::Far] = -3;
    }

    Matrix4x4<T> Inverse() const
    {
        Matrix4x4<T> m;

        T det = m11 * (m22*(m33*m44 - m43*m34) - m23*(m32*m44 - m42*m34) + m24*(m32*m43 - m42*m33))
                - m12 * (m21*(m33*m44 - m43*m34) - m23*(m31*m44 - m41*m34) + m24*(m31*m43 - m41*m33))
                + m13 * (m21*(m32*m44 - m42*m34) - m22*(m31*m44 - m41*m34) + m24 *(m31*m42 - m41*m32))
                - m14 * (m21*(m32*m43 - m42*m33) - m22*(m31*m43 - m41*m33) + m23 *(m31*m42 - m41*m32));

        if( fabs(det) < std::numeric_limits<T>::epsilon() )
            return m;

        m.m11=m22*m33*m44+m23*m34*m42+m24*m32*m43-m22*m34*m43-m23*m32*m44-m24*m33*m42;
        m.m21=m12*m34*m43+m13*m32*m44+m14*m33*m42-m12*m33*m44-m13*m34*m42-m14*m32*m43;
        m.m31=m12*m23*m44+m13*m24*m42+m14*m22*m43-m12*m24*m43-m13*m22*m44-m14*m23*m42;
        m.m41=m12*m24*m33+m13*m22*m34+m14*m23*m32-m12*m23*m34-m13*m24*m32-m14*m22*m33;

        m.m12=m21*m34*m43+m23*m31*m44+m24*m33*m41-m21*m33*m44-m23*m34*m41-m24*m31*m43;
        m.m22=m11*m33*m44+m13*m34*m41+m14*m31*m43-m11*m34*m43-m13*m31*m44-m14*m33*m41;
        m.m32=-1*(m11*m23*m44-m13*m21*m44+m14*m21*m43-m11*m24*m43+m13*m41*m24-m14*m23*m41);
        m.m42=m11*m23*m34+m13*m24*m31+m14*m21*m33-m11*m24*m33-m13*m21*m34-m14*m23*m31;

        m.m13=m21*m32*m44+m22*m34*m41+m24*m31*m42-m21*m34*m42-m22*m31*m44-m24*m32*m41;
        m.m23=m11*m34*m42+m12*m31*m44+m14*m32*m41-m11*m32*m44-m12*m34*m41-m14*m31*m42;
        m.m33=m11*m22*m44+m12*m24*m41+m14*m21*m42-m11*m24*m42-m12*m21*m44-m14*m22*m41;
        m.m43=m11*m24*m32+m12*m21*m34+m14*m22*m31-m11*m22*m34-m12*m24*m31-m14*m21*m32;

        m.m14=m21*m33*m42+m22*m31*m43+m23*m32*m41-m21*m32*m43-m22*m33*m41-m23*m31*m42;
        m.m24=m11*m32*m43+m12*m33*m41+m13*m31*m42-m11*m33*m42-m12*m31*m43-m13*m32*m41;
        m.m34=m11*m23*m42+m12*m21*m43+m13*m22*m41-m11*m22*m43-m12*m23*m41-m13*m21*m42;
        m.m44=m11*m22*m33+m12*m23*m31+m13*m21*m32-m11*m23*m32-m12*m21*m33-m13*m22*m31;

        m *= ((T)1.0/det);
        return m.Transpose();
    }

    template<class U=T>
    Matrix4x4<U> Transpose() const
    {
        Matrix4x4<U> m;
        m.m11 = m11;
        m.m12 = m21;
        m.m13 = m31;
        m.m14 = m41;

        m.m21 = m12;
        m.m22 = m22;
        m.m23 = m32;
        m.m24 = m42;

        m.m31 = m13;
        m.m32 = m23;
        m.m33 = m33;
        m.m34 = m43;

        m.m41 = m14;
        m.m42 = m24;
        m.m43 = m34;
        m.m44 = m44;

        return m;
    };

    void ExtractPlane(Frustum &frustum, int face) const
    {
        PlaneF& plane = frustum.plane[face];
        int row = frustumScale[face];
        int scale = (row < 0) ? -1 : 1;
        row = (std::abs(row) - 1) * 4;

        const Matrix4x4<T>& mat = *this;

        plane.N.x = mat[12] + scale * mat[row];
        plane.N.y = mat[13] + scale * mat[row + 1];
        plane.N.z = mat[14] + scale * mat[row + 2];
        plane.D = mat[15] + scale * mat[row + 3];

        plane.Normalize();
    }

    void LoadIdentity()
    {
        Reset();
        m11 = 1;
        m22 = 1;
        m33 = 1;
        m44 = 1;
    }

    void PreScale(T x, T y, T z)
    {
        Matrix4x4<T> mat;

        mat.m11 = x;
        mat.m22 = y;
        mat.m33 = z;
        mat.m44 = 1;
        *this = mat * *this;
    }

    void Scale(T x, T y, T z)
    {
        Matrix4x4<T> mat;

        mat.m11 = x;
        mat.m22 = y;
        mat.m33 = z;
        mat.m44 = 1;

        operator*=(mat);
    }

    void RotateX(T val)
    {
        Matrix4x4<T> mat;
        val = DegreesToRadians(val);

        mat.m11 = 1;
        mat.m22 = cos(val);
        mat.m23 = -sin(val);
        mat.m32 = sin(val);
        mat.m33 = cos(val);
        mat.m44 = 1;

        operator*=(mat);
    }

    void RotateY(T val)
    {
        Matrix4x4<T> mat;
        val = DegreesToRadians(val);

        mat.m11 = cos(val);
        mat.m13 = sin(val);
        mat.m22 = 1;
        mat.m31 = -sin(val);
        mat.m33 = cos(val);
        mat.m44 = 1;

        operator*=(mat);
    }

    void RotateZ(T val)
    {
        Matrix4x4<T> mat;
        val = DegreesToRadians(val);

        mat.m11 = cos(val);
        mat.m12 = -sin(val);
        mat.m21 = sin(val);
        mat.m22 = cos(val);
        mat.m33 = 1;
        mat.m44 = 1;

        operator*=(mat);
    }

    ///////////////////////////////////////////////////////
    template<typename U>
    void RotateModel( U val, const Vector3<U>& v )
    {
        RotateModel( val * v );
    }

    void RotateModel( T val, T x, T y, T z )
    {
        RotateModel( val, Vector3<T>(x,y,z) );
    }

    void RotateModel( T x, T y, T z )
    {
        RotateY( -y );
        RotateX( -x );
        RotateZ( -z );
    }

    template<typename U>
    void RotateModel( const Vector3<U>& e )
    {
        RotateModel(e.x, e.y, e.z );
    }

    template<typename U>
    void RotateView(const Vector3<U>& e)
    {
        RotateView(e.x, e.y, e.z);
    }

    void RotateView( T x, T y, T z )
    {
        RotateZ( z );
        RotateX( x );
        RotateY( y );
    }
    ///////////////////////////////////////////////////////

    template<typename U>
    void Translate(const Vector3<U> &t)
    {
        Translate( t.x, t.y, t.z );
    }

    void Translate(T dx, T dy, T dz)
    {
        Matrix4x4<T> mat;

        mat.m11 = 1;
        mat.m22 = 1;
        mat.m33 = 1;
        mat.m44 = 1;

        mat.m14 = dx;
        mat.m24 = dy;
        mat.m34 = dz;

        operator*=(mat);
    }

    Matrix4x4<T> operator*(const Matrix4x4<T>& m) const
    {
        Matrix4x4 result;

        result.m11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41;
        result.m12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42;
        result.m13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43;
        result.m14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44;

        result.m21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41;
        result.m22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42;
        result.m23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43;
        result.m24 = m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44;

        result.m31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41;
        result.m32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42;
        result.m33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43;
        result.m34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44;

        result.m41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41;
        result.m42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42;
        result.m43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43;
        result.m44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44;

        return result;
    }

    Matrix4x4<T> operator*(T val) const
    {
        Matrix4x4<T> m;
        m.m11 = m11 * val;
        m.m12 = m12 * val;
        m.m13 = m13 * val;
        m.m14 = m14 * val;
        m.m21 = m21 * val;
        m.m22 = m22 * val;
        m.m23 = m23 * val;
        m.m24 = m24 * val;
        m.m31 = m31 * val;
        m.m32 = m32 * val;
        m.m33 = m33 * val;
        m.m34 = m34 * val;
        m.m41 = m41 * val;
        m.m42 = m42 * val;
        m.m43 = m43 * val;
        m.m44 = m44 * val;
        return m;
    }

    Matrix4x4<T>& operator*=(const Matrix4x4<T>& m)
    {
        return *this = *this * m, *this;
    }

    Matrix4x4<T>& operator*=(T val)
    {
        m11 *= val;
        m12 *= val;
        m13 *= val;
        m14 *= val;
        m21 *= val;
        m22 *= val;
        m23 *= val;
        m24 *= val;
        m31 *= val;
        m32 *= val;
        m33 *= val;
        m34 *= val;
        m41 *= val;
        m42 *= val;
        m43 *= val;
        m44 *= val;
        return *this;
    }

    bool operator==(const Matrix4x4<double>& m) const
    {
        return fabs(this->m11 - m.m11) < std::numeric_limits<double>::epsilon()
                && fabs(this->m12 - m.m12) < std::numeric_limits<double>::epsilon()
                && fabs(this->m13 - m.m13) < std::numeric_limits<double>::epsilon()
                && fabs(this->m14 - m.m14) < std::numeric_limits<double>::epsilon()
                && fabs(this->m21 - m.m21) < std::numeric_limits<double>::epsilon()
                && fabs(this->m22 - m.m22) < std::numeric_limits<double>::epsilon()
                && fabs(this->m23 - m.m23) < std::numeric_limits<double>::epsilon()
                && fabs(this->m24 - m.m24) < std::numeric_limits<double>::epsilon()
                && fabs(this->m31 - m.m31) < std::numeric_limits<double>::epsilon()
                && fabs(this->m32 - m.m32) < std::numeric_limits<double>::epsilon()
                && fabs(this->m33 - m.m33) < std::numeric_limits<double>::epsilon()
                && fabs(this->m34 - m.m34) < std::numeric_limits<double>::epsilon()
                && fabs(this->m41 - m.m41) < std::numeric_limits<double>::epsilon()
                && fabs(this->m42 - m.m42) < std::numeric_limits<double>::epsilon()
                && fabs(this->m43 - m.m43) < std::numeric_limits<double>::epsilon()
                && fabs(this->m44 - m.m44) < std::numeric_limits<double>::epsilon();
    }

    bool operator==(const Matrix4x4<float>& m) const
    {
        return fabs(this->m11 - m.m11) < std::numeric_limits<float>::epsilon()
                && fabs(this->m12 - m.m12) < std::numeric_limits<float>::epsilon()
                && fabs(this->m13 - m.m13) < std::numeric_limits<float>::epsilon()
                && fabs(this->m14 - m.m14) < std::numeric_limits<float>::epsilon()
                && fabs(this->m21 - m.m21) < std::numeric_limits<float>::epsilon()
                && fabs(this->m22 - m.m22) < std::numeric_limits<float>::epsilon()
                && fabs(this->m23 - m.m23) < std::numeric_limits<float>::epsilon()
                && fabs(this->m24 - m.m24) < std::numeric_limits<float>::epsilon()
                && fabs(this->m31 - m.m31) < std::numeric_limits<float>::epsilon()
                && fabs(this->m32 - m.m32) < std::numeric_limits<float>::epsilon()
                && fabs(this->m33 - m.m33) < std::numeric_limits<float>::epsilon()
                && fabs(this->m34 - m.m34) < std::numeric_limits<float>::epsilon()
                && fabs(this->m41 - m.m41) < std::numeric_limits<float>::epsilon()
                && fabs(this->m42 - m.m42) < std::numeric_limits<float>::epsilon()
                && fabs(this->m43 - m.m43) < std::numeric_limits<float>::epsilon()
                && fabs(this->m44 - m.m44) < std::numeric_limits<float>::epsilon();
    }

    Vector4<T> operator*(const Vector4<T>& v) const
    {
        return Vector4<T>(  m11 * v.x + m12 * v.y + m13 * v.z + m14 * v.w,
                            m21 * v.x + m22 * v.y + m23 * v.z + m24 * v.w,
                            m31 * v.x + m32 * v.y + m33 * v.z + m34 * v.w,
                            m41 * v.x + m42 * v.y + m43 * v.z + m44 * v.w );
    }

    void Mult(const Matrix4x4<T> &m)
    {
        operator *=(m);
    }

    void PreMult(const Matrix4x4<T> &m)
    {
        *this = m * *this;
    }

    void LookAt(const Vector3<T>& eye, const Vector3<T>& center, Vector3<T> up = Vector3<T>(0, 1, 0))
    {
        Vector3<T> forward = center - eye;
        LookAtHelper(forward, up);
        Translate(-eye.x, -eye.y, -eye.z);
    }

    void LookAt(const GPSLocation& eye, const GPSLocation& center, Vector3<T> up);

    void SetPerspective(T fovy, T aspect, T zNear, T zFar)
    {
        T h = 1.0 / tan(fovy * M_PI / 360.0);
        T neg_depth = zNear - zFar;
        Matrix4x4<T> m;
        m.m11 = h / aspect;
        m.m22 = h;
        m.m33 = (zFar + zNear) / neg_depth;
        m.m34 = 2.0 * (zNear*zFar) / neg_depth;
        m.m43 = -1;
        operator*=(m);
    }

    void SetOrthographic(T zLeft, T zRight, T zTop, T zBottom, T zNear, T zFar)
    {
        Matrix4x4<T> m;
        m.m11 = 2.0f / (zRight - zLeft);
        m.m14 = -(zRight + zLeft) / (zRight - zLeft);
        m.m22 = -2.0f / (zTop - zBottom);
        m.m24 = (zTop + zBottom) / (zTop - zBottom);
        m.m33 = -2.0f / (zFar - zNear);
        m.m34 = -(zFar + zNear) / (zFar - zNear);
        m.m44 = 1.0f;
        operator*=( m);
    }

    void ApplyShadowModelMatrix(const GPSLocation& loc, float heightAbovePlane, const Vector4<T>& lightDir, const Quarternion<T>& qPlane)
    {
        ApplyModelMatrix(loc);
        Vector4<T> plane = QVRotate(qPlane, Vector4<T>(0, 1, 0, 0));
        double dot = plane * lightDir;
        Matrix4x4<T> m;

        //// first column
        m.m11 = dot - lightDir.x * plane.x;
        m.m12 = 0.0f - lightDir.x * plane.y;
        m.m13 = 0.0f - lightDir.x * plane.z;
        m.m14 = 0.0f - lightDir.x * plane.w;

        // second column
        m.m21 = 0.0f - lightDir.y * plane.x;
        m.m22 = dot - lightDir.y * plane.y;
        m.m23 = 0.0f - lightDir.y * plane.z;
        m.m24 = 0.0f - lightDir.y * plane.w;

        // third column
        m.m31 = 0.0f - lightDir.z * plane.x;
        m.m32 = 0.0f - lightDir.z * plane.y;
        m.m33 = dot - lightDir.z * plane.z;
        m.m34 = 0.0f - lightDir.z * plane.w;

        // forth column
        m.m41 = 0.0f - lightDir.w * plane.x;
        m.m42 = 0.0f - lightDir.w * plane.y;
        m.m43 = 0.0f - lightDir.w * plane.z;
        m.m44 = dot - lightDir.w * plane.w;

        operator*=(m);
    }

    void ApplyModelMatrix(const GPSLocation& loc, bool ignoreHeight=false);
    void ApplyViewMatrix(const GPSLocation& loc, bool ignoreHeight=false);

    const T* ptr() const
    {
        return reinterpret_cast<const T*>(this);
    }

protected:
    void LookAtHelper(Vector3<T> &forward, Vector3<T> & up)
    {
        forward.Normalize();
        Vector3<T> side = forward ^ up;
        side.Normalize();
        up = side ^ forward;

        Matrix4x4<T> m;
        m.LoadIdentity();
        m.m11 = side.x;
        m.m12 = side.y;
        m.m13 = side.z;

        m.m21 = up.x;
        m.m22 = up.y;
        m.m23 = up.z;

        m.m31 = -forward.x;
        m.m32 = -forward.y;
        m.m33 = -forward.z;

        operator *=( m);
    }

};

#include "GPSLocation.h"

typedef Matrix4x4<float> Matrix4x4F;
typedef Matrix4x4<double> Matrix4x4D;

class ToLocalGPS0Matrix4x4 : public Matrix4x4D
{
    ToLocalGPS0Matrix4x4()
    {
        const Vector3D euler = GPSLocation::E0();
        LoadIdentity();
        Translate(0, -GPSLocation::EarthRadius(), 0);
        RotateX(euler.x);
        RotateY(euler.y);
    }

public:
    static const ToLocalGPS0Matrix4x4& get()
    {
        static const ToLocalGPS0Matrix4x4 singleton;
        return singleton;
    }
};

class ToNonLocalGPS0Matrix4x4 : public Matrix4x4D
{
    ToNonLocalGPS0Matrix4x4()
    {
        const Vector3D euler = GPSLocation::E0();
        LoadIdentity();
        RotateY(-euler.y);
        RotateX(-euler.x);
        Translate(0, GPSLocation::EarthRadius(), 0);
    }

public:
    static const ToNonLocalGPS0Matrix4x4& get()
    {
        static const ToNonLocalGPS0Matrix4x4 singleton;
        return singleton;
    }
};

#include <type_traits>


template<class T>
inline void Matrix4x4<T>::LookAt(const GPSLocation& eye, const GPSLocation& center, Vector3<T> up)
{
    Vector3D forward = center.position() - eye.position();
    forward = QVRotate(~eye.makeQ(), forward);
    if constexpr (std::is_same<T, double>::value)
    {
        LookAtHelper(forward, up);
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        Vector3F forwardF = forward.toFloat();
        LookAtHelper(forwardF, up);
    }
}

template<class T>
inline void Matrix4x4<T>::ApplyModelMatrix(const GPSLocation& loc, bool ignoreHeight)
{
    Matrix4x4D m;
    m.LoadIdentity();
    m *= ToLocalGPS0Matrix4x4::get();
    m.RotateY(loc._lng);
    m.RotateX(-loc._lat);
    m *= ToNonLocalGPS0Matrix4x4::get();
    if( !ignoreHeight) m.Translate(0, loc._height, 0);

    if constexpr (std::is_same<T, double>::value)
    {
        operator *= (m);
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        operator *= (m.toFloat());
    }
}

template<class T>
inline void Matrix4x4<T>::ApplyViewMatrix(const GPSLocation& loc, bool ignoreHeight)
{
    Matrix4x4D m;
    m.LoadIdentity();
    if( !ignoreHeight) m.Translate(0, -loc._height, 0);
    m *= ToLocalGPS0Matrix4x4::get();
    m.RotateX(loc._lat);
    m.RotateY(-loc._lng);
    m *= ToNonLocalGPS0Matrix4x4::get();
    if constexpr (std::is_same<T, double>::value)
    {
        operator *= (m);
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        operator *= (m.toFloat());
    }
}


template<class T>
class BiasMatrix4x4 : public Matrix4x4<T>
{
public:
    static const BiasMatrix4x4& get()
    {
        static const BiasMatrix4x4 singleton;
        return singleton;
    }
private:
    BiasMatrix4x4()
    {
        this->m11 = 0.5;
        this->m22 = 0.5;
        this->m33 = 0.5;
        this->m44 = 1.0;

        this->m14 = 0.5;
        this->m24 = 0.5;
        this->m34 = 0.5;
    }

};

typedef BiasMatrix4x4<float> BiasMatrix4x4F;
typedef BiasMatrix4x4<double> BiasMatrix4x4D;
