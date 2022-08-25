#pragma once

#include "BoundingBox.h"
#include "MathSupport.h"
#include <limits>

class MeshHelper
{
public:
    static void normalizeCentroid( Vector3F& centroidLocation, float fTotalArea, size_t polyCount )
    {
        if( fabs(fTotalArea) > std::numeric_limits<float>::epsilon() )
        {
            centroidLocation.y /= fTotalArea;
        }

        if( polyCount )
        {
            centroidLocation.x /= float( polyCount );
            centroidLocation.z /= float( polyCount );
        }
    }

    template<typename T>
    static T CalcArea(const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3)
    {
        return ((p2 - p1)^(p3-p1)).Magnitude()/T(2.0);
    }

    template<typename T>
    static T CalcVolume(const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3)
    {
        return p1 * (p2^p3) / T(6.0);
    }

    template<typename T>
    static T CalcVolume( const std::vector<Vector3<T>>& vertexData )
    {
        T volume(0);

        for( size_t i = 0; i < vertexData.size()-3; i += 3 )
            volume += CalcVolume( vertexData[i], vertexData[i+1], vertexData[i+2] );

        return volume;
    }

    static void calcBoundingBox( const Vector3F& vertex, Vector3F& minV, Vector3F& maxV )
    {
        minV.x = MIN(vertex.x, minV.x);
        minV.y = MIN(vertex.y, minV.y);
        minV.z = MIN(vertex.z, minV.z);

        maxV.x = MAX(vertex.x, maxV.x);
        maxV.y = MAX(vertex.y, maxV.y);
        maxV.z = MAX(vertex.z, maxV.z);
    }

    static BoundingBox calcBoundingBox( const Vector3F* vertexData, size_t size, const Vector3F &extraSpace = Vector3F() )
    {
        Vector3F minV, maxV;

        for( size_t i =0; i < size; ++i )
        {
            if( !i )
                minV = maxV = vertexData[i];
            else
                calcBoundingBox( vertexData[i], minV, maxV );
        }

        return BoundingBox( minV - extraSpace, maxV + extraSpace);
    }

    template<typename T>
    static bool PointInMesh( T VertexData[], int Indices[], size_t count, size_t offset, const Vector3<T>& pt )
    {
        for( size_t i=offset; i < (offset + count - 2); i += 3 )
        {
            Vector3<T> a(VertexData[Indices[i]*3], VertexData[Indices[i]*3+1], VertexData[Indices[i]*3+2] );
            Vector3<T> b(VertexData[Indices[i+1]*3], VertexData[Indices[i+1]*3+1], VertexData[Indices[i+1]*3+2] );
            Vector3<T> c(VertexData[Indices[i+2]*3], VertexData[Indices[i+2]*3+1], VertexData[Indices[i+2]*3+2] );

            Vector3<T> v0 = b-a;
            Vector3<T> v1 = c-a;

            PlaneF p( v0 ^ v1, a );

            T fHeightAbovePlane = p.HeightAbovePlane(pt);
            if( fHeightAbovePlane > 0 )
                return false;
        }
        return true;
    }
};
