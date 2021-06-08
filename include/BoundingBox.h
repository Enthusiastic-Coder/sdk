#pragma once

#include <utility>
#include <MathSupport.h>
#include <Frustum.h>

class SphereVector : public Vector3F
{
public:
    SphereVector() : _radius(0.0f)
    {
    }

    SphereVector(float radius, float x=0, float y=0, float z=0) :
        Vector3F(x,y,z),
        _radius(radius)
    {
    }

    float _radius;
};

class BoundingSphere : public SphereVector
{
public:

    BoundingSphere() : SphereVector()
    {
    }

    BoundingSphere(float radius, float x=0, float y=0, float z=0) : SphereVector(radius,x,y,z)
    {
    }

    bool InFrustum(const Frustum& frustum) const
    {
        float dist;

        for (int i = 0; i < 6; ++i)
        {
            dist = frustum.plane[i].N.x * x + frustum.plane[i].N.y * y + frustum.plane[i].N.z * z +frustum.plane[i].D;

            if (dist <= -_radius)
                return false;
        }
        return true;
    }
};


class BoundingPoint : public Vector3F
{
public:

    BoundingPoint() : Vector3F()
    {
    }

    BoundingPoint(float x, float y, float z) : Vector3F(x,y,z)
    {
    }

    bool InFrustum(const Frustum& frustum, int face ) const
    {
        return frustum.plane[face].N * *this + frustum.plane[face].D >= 0;
    }

    bool InFrustum(const Frustum& frustum) const
    {
        for (int i = 0; i < Frustum::Frustrum_Plane_Count; ++i)
        {
            if( !InFrustum( frustum, i ) )
                return false;
        }
        return true;
    }
};


class BoundingBox
{
public:
    BoundingBox()
    {
        _minX = 0.0;
        _minY = 0.0;
        _minZ = 0.0;
        _maxX = 0.0;
        _maxY = 0.0;
        _maxZ = 0.0;
    }

    BoundingBox( const Vector3F& minpt, const Vector3F& maxpt )
    {
        setLimits( minpt, maxpt );
    }

    static const int _dims=8;

    void setLimits( const std::pair<Vector3F,Vector3F> & box )
    {
        setLimits( box.first, box.second );
    }

    void setLimits( const Vector3F& minpt, const Vector3F& maxpt )
    {
        setLimits( minpt.x, minpt.y, minpt.z, maxpt.x, maxpt.y, maxpt.z );
    }

    void setLimits( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
    {
        float finalMinX = MIN(minX,maxX);
        float finalMinY = MIN(minY,maxY);
        float finalMinZ = MIN(minZ,maxZ);

        float finalMaxX = MAX(minX,maxX);
        float finalMaxY = MAX(minY,maxY);
        float finalMaxZ = MAX(minZ,maxZ);

        _minX = finalMinX;
        _minY = finalMinY;
        _minZ = finalMinZ;

        _maxX = finalMaxX;
        _maxY = finalMaxY;
        _maxZ = finalMaxZ;

        pts[0] = BoundingPoint(_minX, _minY, _minZ);
        pts[1] = BoundingPoint(_minX, _minY, _maxZ);
        pts[2] = BoundingPoint(_maxX, _minY, _maxZ);
        pts[3] = BoundingPoint(_maxX, _minY, _minZ);

        pts[4] = BoundingPoint(_minX, _maxY, _minZ);
        pts[5] = BoundingPoint(_minX, _maxY, _maxZ);
        pts[6] = BoundingPoint(_maxX, _maxY, _maxZ);
        pts[7] = BoundingPoint(_maxX, _maxY, _minZ);
    }

    bool PointIsContained( const Vector3F &pt ) const
    {
        if( pt.x < _minX )
            return false;

        if( pt.x > _maxX )
            return false;

        if( pt.y < _minY )
            return false;

        if( pt.y > _maxY )
            return false;

        if( pt.z < _minZ )
            return false;

        if( pt.z > _maxZ )
            return false;

        return true;
    }

    bool InFrustum( const Frustum& frustum ) const
    {
        bool bInsideLeftRight = false;
        bool bInsideTopBottom = false;
        bool bInsideFarNear = false;

        int leftCount(0), rightCount(0), topCount(0), bottomCount(0), nearCount(0), farCount(0);

        for( int i=0;i < _dims;++i )
        {
            bool bLeft(false), bRight(false), bTop(false), bBottom(false), bNear(false), bFar(false);

            if( !pts[i].InFrustum( frustum, Frustum::Left ) )
                leftCount++;
            else
                bLeft = true;

            if( !pts[i].InFrustum( frustum, Frustum::Right ) )
                rightCount++;
            else
                bRight = true;

            if( !pts[i].InFrustum( frustum, Frustum::Top ) )
                topCount++;
            else
                bTop = true;

            if( !pts[i].InFrustum( frustum, Frustum::Bottom ) )
                bottomCount++;
            else
                bBottom = true;

            if( !pts[i].InFrustum( frustum, Frustum::Far ) )
                farCount++;
            else
                bFar = true;

            if( !pts[i].InFrustum( frustum, Frustum::Near ) )
                nearCount++;
            else
                bNear = true;

            bool bLeftRight = bLeft && bRight;
            bool bTopBottom = bTop && bBottom;
            bool bFarNear = bNear && bFar;

            if(bLeftRight && bTopBottom && bFarNear)
                return true;

            bInsideLeftRight |= bLeftRight;
            bInsideTopBottom |= bTopBottom;
            bInsideFarNear |= bFarNear;
        }

        bool bLeftRightCount = leftCount && rightCount;
        bool bTopBottomCount = topCount && bottomCount;
        bool bNearFarCount = nearCount && farCount;

        if ((bLeftRightCount || bInsideLeftRight)
                && (bTopBottomCount || bInsideTopBottom)
                && (bNearFarCount || bInsideFarNear))
            return true;

        return false;
    }

    Vector3F getMin() const
    {
        return Vector3F(_minX,_minY,_minZ);
    }

    Vector3F getMax() const
    {
        return Vector3F(_maxX,_maxY,_maxZ);
    }

    Vector3F operator[](int idx)
    {
        return pts[idx];
    }

    float getMinX() const
    {
        return _minX;
    }

    float getMaxX() const
    {
        return _maxX;
    }

    float getMinY() const
    {
        return _minY;
    }

    float getMaxY() const
    {
        return _maxY;
    }

    float getMinZ() const
    {
        return _minZ;
    }

    float getMaxZ() const
    {
        return _maxZ;
    }


private:
    BoundingPoint pts[_dims];
    float _minX;
    float _minY;
    float _minZ;
    float _maxX;
    float _maxY;
    float _maxZ;
};
