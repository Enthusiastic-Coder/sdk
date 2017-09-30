#pragma once

#include <sstream>
#include "Quarternion.h"
#include "vector3.h"
#include "degreesRadians.h"

class GPSLocation
{
public:
    double _lat;
    double _lng;
    double _height;

    const static Vector3D& E0()
    {
        const static Vector3D euler = GPSLocation().makeEuler();
        return euler;
    }

    static GPSLocation FromDegMin(const std::string & str)
    {
        //N5944.46	E00526.39
        if( str.length() <16)
            return GPSLocation();

        GPSLocation loc;
        loc._lat = std::atof(str.substr(1,2).c_str());
        loc._lat += std::atof(str.substr(3,4).c_str())/60.0;

        loc._lng = std::atof(str.substr(10,3).c_str());
        loc._lng += std::atof(str.substr(13, 4).c_str())/60.0;

        if( str[0] != 'N')
            loc._lat = -loc._lat;

        if( str[9] == 'W')
            loc._lng = -loc._lng;

        return loc;
    }

    static GPSLocation FromDegMinSec(const std::string& str)
    {
        //N520555 W0005756 - 16 chars long4
        if( str.length() < 15)
            return GPSLocation();

        bool bNarrow = str.length() == 15;

        GPSLocation loc;
        loc._lat = std::atof(str.substr(1, 2).c_str());
        loc._lat += std::atof(str.substr(3, 2).c_str())/60.0;
        loc._lat += std::atof(str.substr(5,2).c_str())/3600.0;

        loc._lng = std::atof(str.substr(9, bNarrow?2:3).c_str());
        loc._lng += std::atof(str.substr(bNarrow?11:12,2).c_str())/60.0;
        loc._lng += std::atof(str.substr(bNarrow?13:14,2).c_str())/3600.0;

        if( str[0] != 'N')
            loc._lat = -loc._lat;

        if( str[8] == 'W')
            loc._lng = -loc._lng;

        return loc;
    }

    GPSLocation(double fLat, double fLng, double fHeight = 0.0 )
        : _lat(fLat), _lng(fLng), _height(fHeight)
    {
    }

    GPSLocation()
        : _lat(0), _lng(0), _height(0)
    {
    }

    GPSLocation( const Vector3D& p)
    {
        setFromPosition(p);
    }

    GPSLocation(const GPSLocation &p)
        :GPSLocation(p, p._height)
    {
    }

    GPSLocation( const GPSLocation &p, double fHeight)
        : _lat(p._lat), _lng(p._lng), _height(fHeight)
    {

    }

    GPSLocation(std::string str)
    {
        std::stringstream ss(str);
        _lat = _lng = _height = 0;
        ss.precision(9);
        ss >> _lat >> _lng >> _height;
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss.precision(9);
        ss << _lat << " " << _lng << " " << _height;
        return ss.str();
    }

    void setFromPosition(const Vector3D& p)
    {
        Vector3D unitP = p.Unit();
        _lat = asin(unitP.y);
        double cosfLat = cos(_lat);

        if( fabs( cosfLat ) < std::numeric_limits<float>::epsilon() )
            _lng = double(0);
        else
        {
            double ratio = unitP.x/cosfLat;

            if( ratio < -1 )
                ratio = -1;
            else if( ratio > 1 )
                ratio = 1;

            _lng = asin(ratio);
        }

        _lat = RadiansToDegrees(_lat);
        _lng = RadiansToDegrees(_lng);
        if( p.z < 0 )
            _lng = 180 - _lng;

        _height = p.Magnitude() - EarthRadius();
    }

    double LatRad() const
    {
        return DegreesToRadians( _lat );
    }

    double LngRad() const
    {
        return DegreesToRadians( _lng );
    }

    static double EarthRadius()
    {
        return 6371000.0;
    }

    Vector3D makeEuler() const
    {
        return Vector3D( -(90-_lat), -_lng, 0);
    }

    Vector3D position(double radius, double dHeight) const
    {
        return (radius + dHeight) * normal();
    }

    Vector3D position(double dHeight) const
    {
        return position(EarthRadius(), dHeight);
    }

    Vector3D position() const
    {
        return position(_height);
    }

    Vector3D normal() const
    {
        double fRadLat = DegreesToRadians(_lat);
        double fRadLng = DegreesToRadians(_lng);

        return Vector3D(sin(fRadLng) * cos( fRadLat ),sin(fRadLat), cos(fRadLng) * cos( fRadLat )).Unit();
    }

    Vector3D Orientation() const
    {
        double fRadLat = DegreesToRadians(_lat);
        double fRadLng = DegreesToRadians(_lng);

        Vector3D n;
        n.x = -sin(fRadLat) * sin(fRadLng);
        n.y = cos(fRadLat);
        n.z = -sin(fRadLat) * cos(fRadLng);

        n.Normalize();
        return n;
    }

    Quarternion<double> makeQ( const Vector3D& euler) const
    {
        return makeQ() * MathSupport<double>::MakeQ(euler);
    }

    Quarternion<double> makeQ( double x, double y, double z) const
    {
        return makeQ() * MathSupport<double>::MakeQ(x,y,z);
    }

    Quarternion<double> makeQ() const
    {
        return MathSupport<double>::MakeQ( makeEuler() );
    }

    static float calcBearingXZ( const Vector3F& fromV, const Vector3F& toV )
    {
        return RadiansToDegrees(atan2f(toV.z - fromV.z, toV.x - fromV.x)) + 90.0f;
    }

    static float calcBearingXZ( const Vector3D& fromV, const Vector3D& toV )
    {
        return (float) RadiansToDegrees(atan2(toV.z - fromV.z, toV.x - fromV.x)) + 90.0f;
    }

    template<class T>
    GPSLocation operator+( T deltaHeight ) const
    {
        return GPSLocation( _lat, _lng, _height + deltaHeight );
    }

    template<class T>
    GPSLocation operator-( T deltaHeight ) const
    {
        return operator+(-deltaHeight);
    }

    template<class T>
    GPSLocation operator+(const Vector3<T> & p) const
    {
        return GPSLocation(*this) += p;
    }

    template<class T>
    GPSLocation operator-(const Vector3<T> & p) const
    {
        return GPSLocation(*this) += -p;
    }

    GPSLocation operator+(const GPSLocation &gps ) const
    {
        GPSLocation loc(*this);
        return loc += gps;
    }

    GPSLocation operator-(const GPSLocation &gps ) const
    {
        GPSLocation loc(*this);
        return loc -= gps;
    }

    template<class T>
    GPSLocation operator*(T value) const
    {
        GPSLocation loc(*this);
        return loc *= value;
    }

    template<class T>
    GPSLocation& operator*=(T value)
    {
        _lat *= value;
        _lng *= value;
        _height *= value;
        return *this;
    }

    template<class T>
    GPSLocation& operator-=(const Vector3<T> & p)
    {
        return operator+=(-p);
    }

    template<class T>
    GPSLocation& operator+=(const Vector3<T> & p)
    {
        double lat1 = LatRad();
        double lng1 = LngRad();
        double bearing  = DegreesToRadians(calcBearingXZ( Vector3<T>(), p )) ;
        double radDist = p.MagnitudeXZ() / EarthRadius();

        double lat2 = asin( sin(lat1) * cos( radDist) + cos(lat1) * sin( radDist ) * cos(bearing) );
        double lng2 = lng1 + atan2( sin(bearing) * sin( radDist ) * cos (lat1), cos ( radDist ) - sin( lat1) * sin (lat2 ) );

        _lat = RadiansToDegrees( lat2 );
        _lng = RadiansToDegrees( lng2 );
        _height += p.y;
        return *this;
    }

    GPSLocation& operator+=(const GPSLocation & gps)
    {
        _lat += gps._lat;
        _lng += gps._lng;
        _height += gps._height;
        return *this;
    }

    GPSLocation& operator-=(const GPSLocation & gps)
    {
        _lat -= gps._lat;
        _lng -= gps._lng;
        _height -= gps._height;
        return *this;
    }

    bool operator==(const GPSLocation& gps) const
    {
        return fabs(_lat - gps._lat) < std::numeric_limits<float>::epsilon()
                && fabs(_lng - gps._lng) < std::numeric_limits<float>::epsilon()
                && fabs(_height - gps._height) < std::numeric_limits<float>::epsilon();
    }

    bool operator!=(const GPSLocation& gps) const
    {
        return !operator==(gps);
    }

    GPSLocation operator-()
    {
        GPSLocation negGPS;
        negGPS._lat = -_lat;
        negGPS._lng = -_lng;
        return negGPS;
    }

    double operator*(const GPSLocation &rhs ) const
    {
        Vector3D a(_lat, _lng, _height *0);
        Vector3D b(rhs._lat, rhs._lng, rhs._height *0);
        return a * b;
    }

    GPSLocation operator^(const GPSLocation &rhs ) const
    {
        Vector3D a(_lng, _height*0, _lat );
        Vector3D b(rhs._lng, rhs._height*0, rhs._lat );
        Vector3D c = a ^ b;
        return GPSLocation(c.x, c.y, c.z);
    }

    static double calcBearing( const GPSLocation &fromGps, const GPSLocation &toGps )
    {
        double lat1 = fromGps.LatRad();
        double lng1 = fromGps.LngRad();
        double lat2 = toGps.LatRad();
        double lng2 = toGps.LngRad();
        double dLng = lng2 - lng1;

        double y = sin( dLng ) * cos( lat2 );
        double x = cos( lat1 ) * sin( lat2 ) - sin( lat1 ) * cos( lat2 ) * cos( dLng );
        double brg = RadiansToDegrees( atan2( y, x ) );
        if (brg < 0.0)
            brg += 360.0;
        return brg;
    }

    double bearingTo( const GPSLocation &gps ) const
    {
        return calcBearing( *this, gps);
    }

    double bearingFrom( const GPSLocation &gps ) const
    {
        return calcBearing( gps, *this);
    }

    double distanceTo( const GPSLocation& gpsLocation ) const
    {
        double dLat = DegreesToRadians( gpsLocation._lat - _lat );
        double dLng = DegreesToRadians( gpsLocation._lng - _lng );
        double lat1 = LatRad();
        double lat2 = gpsLocation.LatRad();

        double a = sin( dLat/2 ) * sin( dLat/2) + sin( dLng/2) * sin( dLng/2) * cos( lat1 ) * cos( lat2 );
        double c = 2 * atan2( sqrt(a), sqrt(1-a) );

        return EarthRadius() * c;
    }

    Vector3F offSetTo( const GPSLocation & gpsLocation ) const
    {
        double dDist = distanceTo( gpsLocation );
        double dBear = DegreesToRadians( bearingTo( gpsLocation ) );

        return Vector3F( float(dDist * sin(dBear)), float(gpsLocation._height - _height), float(-dDist * cos(dBear)) );
    }

    template<class T>
    GPSLocation GetShadowLocation( T heightAbovePlane, const Vector4<T>& lightDir ) const
    {
        Vector3D offset;
        offset.y = -heightAbovePlane;
        offset.x = offset.y * lightDir.x / sqrt(1- lightDir.x * lightDir.x);
        offset.z = offset.y * lightDir.z / sqrt(1- lightDir.z * lightDir.z);
        return operator+( offset );
    }

    bool ptInBounds(const GPSLocation& topLeft, const GPSLocation& bottomRight) const
    {
        if (_lat > topLeft._lat)
            return false;

        if (_lng < topLeft._lng)
            return false;

        if (_lat < bottomRight._lat)
            return false;

        if (_lng > bottomRight._lng)
            return false;

        return true;
    }
};
