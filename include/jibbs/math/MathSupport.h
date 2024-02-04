#pragma once

#include <math.h>
#include <limits>

#define MathSupportType double

#include "degreesRadians.h"

template<typename T> class Quarternion;
template<typename T> class Vector3;
template<typename T> class Plane;

template<typename T>
class MathSupport
{
public:
    struct size
    {
        T width;
        T height;
    };

    struct point
    {
        T x;
        T y;
    };

    static Quarternion<T> MakeQPitch(T ang)
    {
        T fRad = DegreesToRadians( -ang/2 );
        return Quarternion<T>( cos( fRad ), sin( fRad ), 0, 0 );
    }

    static Quarternion<T> MakeQBank(T ang)
    {
        T fRad = DegreesToRadians( -ang/2 );
        return Quarternion<T>( cos( fRad), 0, 0, sin( fRad ) );
    };

    static Quarternion<T> MakeQHeading(T ang)
    {
        T fRad = DegreesToRadians( -ang/2 );
        return Quarternion<T>( cos( fRad ), 0, sin( fRad ), 0 );
    };

    static Quarternion<T> MakeQ(const Vector3<T> &v )
    {
        return MakeQ(v.x, v.y, v.z);
    }

    static Quarternion<T> MakeQ( const Plane<T> &p )
    {
        return MakeQ(MakeEulerFromNormal(p.N));
    }

    static Quarternion<T> MakeQ(T x, T y, T z )
    {
        Quarternion<T>	q;

        T	roll = DegreesToRadians(-z);
        T	pitch = DegreesToRadians(-x);
        T	yaw = DegreesToRadians(-y);

        T	cyaw, cpitch, croll, syaw, spitch, sroll;
        T	cyawcpitch, syawspitch, cyawspitch, syawcpitch;

        cyaw = cos(yaw/2);
        cpitch = cos(pitch/2);
        croll = cos(roll/2);

        syaw = sin(yaw/2);
        spitch = sin(pitch/2);
        sroll = sin(roll/2);

        cyawcpitch = cyaw*cpitch;
        syawspitch = syaw*spitch;
        cyawspitch = cyaw*spitch;
        syawcpitch = syaw*cpitch;

        q.n = cyawcpitch * croll + syawspitch * sroll;
        q.v.x = cyawspitch * croll + syawcpitch * sroll;
        q.v.y = syawcpitch * croll - cyawspitch * sroll;
        q.v.z = cyawcpitch * sroll - syawspitch * croll;

        return q;
    }

    static Vector3<T> MakeEulerFromLookAt(const Vector3<T> & lookat)
    {
        T xz = sqrt(lookat.x*lookat.x + lookat.z*lookat.z);

        if (fabs(xz) < std::numeric_limits<float>::epsilon())
            return Vector3<T>();

        T pitch = -R2D(atan(lookat.y / xz));
        T hdg = -R2D(acos(-lookat.z / xz));

        if (lookat.x > 0.0)
            hdg = -hdg;

        return Vector3<T>(pitch, hdg, 0);
    }

    static Vector3<T> MakeEulerFromNormal(const Vector3<T> & n)
    {
        T xy = sqrt(n.x*n.x + n.y*n.y);

        if( fabs(xy) < std::numeric_limits<float>::epsilon() )
            return Vector3<T>();

        T zy = sqrt(n.z*n.z + n.y*n.y);

        if( fabs(zy) < std::numeric_limits<float>::epsilon() )
            return Vector3<T>();

        Vector3<T> ret(R2D(atan(n.z / xy)), 0, R2D(atan(n.x / zy)));

        if( n.y < 0 )
            ret.x -= 180;
        else
            ret.x = -ret.x;

        return ret;
    }

    static Vector3<T> MakeEuler(const Quarternion<T> & q)
    {
        T	r11, r21, r31, r32, r33, r12, r13;
        T	q00, qxx, qyy, qzz;
        T	tmp;
        Vector3<T>	u;

        q00 = q.n * q.n;
        qxx = q.v.x * q.v.x;
        qyy = q.v.y * q.v.y;
        qzz = q.v.z * q.v.z;

        r11 = q00 - qxx - qyy + qzz;
        r21 = 2 * (q.v.z*q.v.x + q.n*q.v.y);
        r31 = 2 * (q.v.z*q.v.y - q.n*q.v.x);
        r32 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
        r33 = q00 - qxx + qyy - qzz;

        tmp = fabs(r31);
        if(tmp > 0.999999)
        {
            r12 = 2 * (q.v.z*q.v.x - q.n*q.v.y);
            r13 = 2 * (q.v.z*q.v.y + q.n*q.v.x);

            u.x = -RadiansToDegrees((T) (-(M_PI/2) * r31/tmp)); // pitch
            u.y = -RadiansToDegrees((T) atan2(-r12, -r31*r13)); // yaw
            u.z = -RadiansToDegrees(T(0.0)); //roll
        }
        else
        {
            u.x = -RadiansToDegrees((T) asin(-r31));		 // pitch
            u.y = -RadiansToDegrees((T) atan2(r21, r11)); // yaw
            u.z = -RadiansToDegrees((T) atan2(r32, r33)); // roll
        }

        while( u.y < 0 )
            u.y += 360;

        while( u.y >=360 )
            u.y -= 360;

        while( u.x < -180 )
            u.x += 360;

        while( u.x > 180 )
            u.x -= 360;

        while( u.z < -180 )
            u.z += 360;

        while( u.z > 180 )
            u.z -= 360;

        return u;
    }

    static Vector3<T> debugVFromQ(Quarternion<T> q)
    {
        Vector3<T> v = MakeEuler(q);
        v.y = -v.y;
        if( v.y < 0 ) v.y+=360;
        return v;
    }

    static T normAng(T angle)
    {
        T x = fmod(angle, 360);
        if( x < 0)
            x +=360;
        return x;
    }

    static T normAngWithMinus(T angle)
    {
        T x= fmod(angle, 360);
        if( x < -180)
            x += 360;
        return x;
    }
};


