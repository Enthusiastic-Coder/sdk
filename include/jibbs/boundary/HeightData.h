#pragma once

#include "MathSupport.h"
#include <algorithm>
#include <Quarternion.h>
#include <vector>
#include <Plane.h>

class HeightData
{
public:
    HeightData() :
        _dHeight(0.0),
        _qNormal(QuarternionF::Q0())
    {}

    void setData( float dHeightAbovePlane, const QuarternionF &qNormal=QuarternionF::Q0(), const Vector3D& velocity = Vector3D() )
    {
        _dHeight = dHeightAbovePlane;
        _qNormal = qNormal;
        _velocity = velocity;
    }

    void setData( const Vector3F & pt, const PlaneF& plane, const Vector3D& velocity = Vector3D() )
    {
        setData( plane.HeightAbovePlane( pt ), plane.GetQ(), velocity );
    }

    float Height() const
    {
        return _dHeight -0.15f;//MOJ_JEB
    }

    const QuarternionF& qNormal() const
    {
        return _qNormal;
    }

    const Vector3D& Velocity() const
    {
        return _velocity;
    }

private:
    float _dHeight;
    QuarternionF _qNormal;
    Vector3D _velocity;
};

inline size_t GetHeightIndexFromPlanes( std::vector<HeightData>& planes, double dMinCeiling = 2.0)
{
    std::sort( planes.begin(), planes.end(), []( const HeightData& lhs, const HeightData &rhs )
    {
        return lhs.Height() > rhs.Height();
    }
    );

    size_t idx = planes.size() - 1;

    for( size_t i = 1 ; i < planes.size(); ++i )
    {
        if( planes[i].Height() >= 0.0 )
            continue;

        idx = i;

        if( planes[i].Height() < -dMinCeiling ) // maximum Jump
            --idx;

        break;
    }

    return idx;
}
