#pragma once

#include <utility>
#include "Serializable.h"
#include <Quarternion.h>
#include <vector3.h>

class GPSModelTransform
{
public:

    GPSModelTransform()
    {
    }

    GPSModelTransform( const GPSLocation & location )
    {
        setLocation(location );
    }

    void setLocation(const GPSLocation & location)
    {
        _Location = location;
        _qAntiOrientation = ~_Location.makeQ();
    }

    Vector3F LocalFlatToLocal( Vector3F pt, QuarternionF qOrientation = QuarternionF::Q0() ) const
    {
        return ToLocal( _Location + QVRotate( qOrientation, pt ) );
    }

    std::vector<Vector3F> ToLocal( const std::vector<GPSLocation> &gpsPts ) const
    {
        std::vector<Vector3F> pts;
        pts.resize( gpsPts.size() );

        for( size_t i = 0; i < gpsPts.size(); ++i )
            pts[i] = ToLocal( gpsPts[i].position() );

        return pts;
    }

    Vector3F ToLocal( GPSLocation gps ) const
    {
        return ToLocal( gps.position() );
    }

    Vector3F ToLocal( Vector3D pt ) const
    {
        pt = QVRotate( _qAntiOrientation, pt );
        pt.y -= GPSLocation::EarthRadius();
        return pt.toFloat();
    }

    const GPSLocation& Location() const
    {
        return _Location;
    }

protected:
    GPSLocation _Location;
    QuarternionD _qAntiOrientation;
};
