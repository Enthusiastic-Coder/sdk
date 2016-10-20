#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include <limits>

#include "vector3.h"
#include "Plane.h"

class BoundaryHelperT
{
public:
    static Vector3F calcRealT(const Vector3F boundary[4], float u, float v )
    {
        Vector3F u1 = boundary[1] - boundary[0];
        Vector3F u2 = boundary[2] - boundary[3];

        Vector3F uu1 = u1 * u + boundary[0];
        Vector3F uu2 = u2 * u + boundary[3];

        return (uu2 - uu1) * v + uu1;
    }

    static void calcFractionT(const Vector3F boundary[4], const Vector3F& p, float *u, float *v)
    {
        Vector3F a = boundary[1] - boundary[0];
        Vector3F b = boundary[2] - boundary[3];
        Vector3F c = boundary[3] - boundary[0];

        Vector3F d = (c ^ a ) ^ c;
        d.Normalize();

        float d1 = a * d.Unit();
        float d2 = b * d.Unit();

        float u1 = 0.0f;
        float u2 = 0.0f;

        if( fabs(d1) < std::numeric_limits<float>::epsilon() )
            u1 = 0.0f;
        else
            u1 = ((p - boundary[0])*d.Unit()) / d1;

        if( fabs(d2) < std::numeric_limits<float>::epsilon() )
            u2 = 0.0f;
        else
            u2 = ((p - boundary[3])*d.Unit()) / d2;

        Vector3F pa = calcRealT(boundary, u1, 0.0f);
        Vector3F pb = calcRealT(boundary, u2, 1.0f);

        Vector3F vNorm = (pb-pa).Unit();

        *v = ((p - pa)*vNorm) / ((pb-pa)* vNorm );
        *u = (u2 - u1 ) * *v + u1;
    }

    static Vector3F calcNormal(const Vector3F boundary[3])
    {
        Vector3F a = boundary[1] - boundary[0];
        Vector3F b = boundary[2] - boundary[0];
        Vector3F normal = a ^ b;
        return normal.Unit();
    }

    static float calcArea(const Vector3F boundary[4])
    {
        float area = 0.0;

        for( int i=0; i < 3; i++ )
            area += ((boundary[i] - boundary[0]) ^ ( boundary[i+1] - boundary[0])).Magnitude();

        return area / 2.0f;
    }

    static bool PointIsContained( const Vector3F boundary[4], const Vector3F &pt )
    {
        float u, v;
        calcFractionT(boundary, pt, &u, &v);
        return u>= 0 && u <= 1 && v >=0 && v <= 1;
    }

    ///////////////////////////////////////////////////////////////////////////////

    static Vector3F calc3RealT(const Vector3F boundary[3], float u, float v )
    {
        Vector3F a = boundary[1] - boundary[0];
        Vector3F b = boundary[2] - boundary[0];

        return boundary[0] + a * u + b * v;
    }

    static void calc3FractionT(const Vector3F boundary[3], const Vector3F& p, float *u, float *v)
    {
        Vector3F v0 = boundary[1] - boundary[0];
        Vector3F v1 = boundary[2] - boundary[0];
        Vector3F v2 = p - boundary[0];

        // Compute dot products
        float dot00 = v0 * v0;
        float dot01 = v0 * v1;
        float dot02 = v0 * v2;
        float dot11 = v1 * v1;
        float dot12 = v1 * v2;

        // Compute barycentric coordinates
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        *u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        *v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    }

    static float calc3Area(const Vector3F boundary[3])
    {
        float area( 0.0f);

        for( int i=0; i < 2; i++ )
            area += ((boundary[i] - boundary[0]) ^ ( boundary[i+1] - boundary[0])).Magnitude();

        return area / 2.0f;
    }

    static bool Point3IsContained( const Vector3F boundary[3], const Vector3F &pt )
    {
        float u(0.0f), v(0.0f);
        calc3FractionT(boundary, pt, &u, &v);
        return (u >= 0.0f) && (v >= 0.0f) && ((u + v) <= 1.0f);
    }
};

class TrianglePlaneBoundaryPrimitiveT
{
public:
    TrianglePlaneBoundaryPrimitiveT( Vector3F* pV1, Vector3F* pV2, Vector3F* pV3)
        : _area(0.0)
    {
        _pBoundary[0] = pV1;
        _pBoundary[1] = pV2;
        _pBoundary[2] = pV3;
    }

    virtual void setBoundaryT(const Vector3F vec4[3])
    {
        for( int i=0; i < 3; i++ )
            *_pBoundary[i] = vec4[i];

        calcPlane();
        calcArea();
    }

    void getBoundaryT(Vector3F boundary[3]) const
    {
        for( int i=0; i < 3; i++ )
            boundary[i] = *_pBoundary[i];
    }

    const Vector3F& getBoundary(size_t idx) const
    {
        return *_pBoundary[idx];
    }

    Vector3F& operator[]( int idx )
    {
        return *_pBoundary[idx];
    }

    const Vector3F& operator[]( int idx ) const
    {
        return *_pBoundary[idx];
    }

    const Vector3F& getNormal() const
    {
        return _plane.N;
    }

    void setNormal(const Vector3F & n)
    {
        (_plane.N = n).Normalize();
    }

    Vector3F calcRealT(float u, float v ) const
    {
        return BoundaryHelperT::calc3RealT(_pBoundary[0], u,v);
    }

    void calcFractionT(const Vector3F& p, float *u, float *v) const
    {
        BoundaryHelperT::calc3FractionT( _pBoundary[0], p, u, v );
    }

    void calcArea()
    {
        _area = BoundaryHelperT::calc3Area( _pBoundary[0] );
    }

    void calcPlane()
    {
        _plane = PlaneF(BoundaryHelperT::calcNormal( _pBoundary[0] ), *_pBoundary[0] );
    }

    bool PointIsContained( const Vector3F &pt ) const
    {
        return BoundaryHelperT::Point3IsContained( _pBoundary[0], pt );
    }

    float getArea() const
    {
        return _area;
    }

    const PlaneF& getPlane() const
    {
        return _plane;
    }

private:
    Vector3F *_pBoundary[3];
    float _area;
    PlaneF _plane;
};

class TrianglePlaneBoundaryArrayT 
{
public:
    TrianglePlaneBoundaryArrayT(std::vector<Vector3F>& pts) : _pts(pts) {}

    bool calcPlanes()
    {
        std::vector<unsigned int> indices;
        indices.resize( _pts.size() );

        for( size_t i=0; i < indices.size(); ++i )
            indices[i] = i;

        return calcPlanes(indices);
    }

    bool calcStripPlanes()
    {
        std::vector<unsigned int> indices;
        indices.resize( (_pts.size() -2 ) *3 );

        for( size_t i=0; i < _pts.size()-2; i++ )
        {
            indices[i*3] = i;

            if( (i % 2) == 0 )
            {
                indices[i*3+1] = i+1;
                indices[i*3+2] = i+2;
            }
            else
            {
                indices[i*3+1] = i+2;
                indices[i*3+2] = i+1;
            }
        }

        return calcPlanes(indices);
    }

    bool calcPlanes(std::vector<unsigned int>& indices)
    {
        _boundaries.clear();

        if( indices.size() < 3 || (indices.size() % 3) )
            return false;

        for( size_t i = 0; i < indices.size()-2; i += 3 )
        {
            if( indices[i] >= _pts.size())
                return false;

            if( indices[i+1] >= _pts.size())
                return false;

            if( indices[i+2] >= _pts.size())
                return false;

            _boundaries.push_back( std::unique_ptr<TrianglePlaneBoundaryPrimitiveT>(
                                       new TrianglePlaneBoundaryPrimitiveT(	& _pts[ indices[i] ],
                                       & _pts[ indices[i+1] ],
                    & _pts[ indices[i+2] ]) ) );

            _boundaries.back()->calcPlane();
        }

        return true;
    }

    size_t size() const
    {
        return _boundaries.size();
    }

    const TrianglePlaneBoundaryPrimitiveT& operator[](size_t idx) const
    {
        return *_boundaries[idx];
    }

private:
    std::vector<Vector3F>& _pts;
    std::vector<std::unique_ptr<TrianglePlaneBoundaryPrimitiveT>> _boundaries;
};

class QuadPlaneBoundaryPrimitiveT
{
public:
    QuadPlaneBoundaryPrimitiveT( Vector3F* pV1, Vector3F* pV2, Vector3F* pV3, Vector3F* pV4)
        : _area(0.0)
    {
        _pBoundary[0] = pV1;
        _pBoundary[1] = pV2;
        _pBoundary[2] = pV3;
        _pBoundary[3] = pV4;
    }

    virtual void setBoundaryT(const Vector3F vec4[4])
    {
        for( int i=0; i < 4; i++ )
            *_pBoundary[i] = vec4[i];

        calcPlane();
        calcArea();
    }

    void getBoundaryT(Vector3F boundary[4]) const
    {
        for( int i=0; i < 4; i++ )
            boundary[i] = *_pBoundary[i];
    }

    const Vector3F& getBoundary(size_t idx) const
    {
        return *_pBoundary[idx];
    }

    const Vector3F* getBoundaryPtr(size_t idx) const
    {
        return _pBoundary[idx];
    }

    Vector3F& operator[]( int idx )
    {
        return *_pBoundary[idx];
    }

    const Vector3F& operator[]( int idx ) const
    {
        return *_pBoundary[idx];
    }

    const Vector3F& getNormal() const
    {
        return _plane.N;
    }

    void setNormal(const Vector3F & n)
    {
        (_plane.N = n).Normalize();
    }

    Vector3F calcRealT(float u, float v ) const
    {
        return BoundaryHelperT::calcRealT(_pBoundary[0], u,v);
    }

    void calcFractionT(const Vector3F& p, float *u, float *v) const
    {
        BoundaryHelperT::calcFractionT( _pBoundary[0], p, u, v );
    }

    void calcArea()
    {
        _area = BoundaryHelperT::calcArea( _pBoundary[0] );
    }

    void calcPlane()
    {
        _plane = PlaneF(BoundaryHelperT::calcNormal( _pBoundary[0] ), *_pBoundary[0] );
    }

    bool PointIsContained( const Vector3F &pt ) const
    {
        return BoundaryHelperT::PointIsContained( _pBoundary[0], pt );
    }

    float getArea() const
    {
        return _area;
    }

    const PlaneF& getPlane() const
    {
        return _plane;
    }

private:
    Vector3F *_pBoundary[4];
    float _area;
    PlaneF _plane;
};


class QuadPlaneBoundaryArrayT 
{
public:
    QuadPlaneBoundaryArrayT(std::vector<Vector3F>& pts, std::vector<unsigned int>& indices)
        :_pts(pts), _indices(indices)
    {
    }

    bool makeStripPlanes()
    {
        return false;
    }

    bool makePlanes()
    {
        _boundaries.clear();

        if( _indices.size() < 4 || (_indices.size() % 4) )
            return false;

        for( size_t i = 0; i < _indices.size()-3; i += 4 )
        {
            if( _indices[i] >= _pts.size())
                return false;

            if( _indices[i+1] >= _pts.size())
                return false;

            if( _indices[i+2] >= _pts.size())
                return false;

            if( _indices[i+3] >= _pts.size())
                return false;

            _boundaries.push_back( std::unique_ptr<QuadPlaneBoundaryPrimitiveT>(
                                       new QuadPlaneBoundaryPrimitiveT(
                                           & _pts[ _indices[i] ],
                                       & _pts[ _indices[i+1] ],
                    & _pts[ _indices[i+2] ],
                    & _pts[ _indices[i+3] ]) ) );

            _boundaries.back()->calcPlane();
        }

        return true;
    }

    size_t size() const
    {
        return _boundaries.size();
    }

    QuadPlaneBoundaryPrimitiveT& operator[](size_t idx)
    {
        return *_boundaries[idx];
    }

private:
    std::vector<Vector3F>& _pts;
    std::vector<unsigned int>& _indices;
    std::vector<std::unique_ptr<QuadPlaneBoundaryPrimitiveT>> _boundaries;
};


class QuadPlaneBoundaryT : public QuadPlaneBoundaryPrimitiveT
{
public:
    QuadPlaneBoundaryT()
        : QuadPlaneBoundaryPrimitiveT(_boundary, _boundary+1, _boundary+2, _boundary+3)
    {}

    QuadPlaneBoundaryT(const QuadPlaneBoundaryT & rhs )
        : QuadPlaneBoundaryPrimitiveT(_boundary, _boundary+1, _boundary+2, _boundary+3)
    {
        setBoundaryT(rhs._boundary);
    }

private:
    Vector3F _boundary[4];
};


template<class ParentT>
class ChildQuadBoundaryT : public QuadPlaneBoundaryT
{
public:
    void setBoundaryFromParentT(float u[4], float v[4])
    {
        ParentT* t = static_cast<ParentT*>(this);

        if( t->parent() )
        {
            Vector3F boundary[4];

            for( int i=0; i < 4; i++ )
                boundary[i] = t->parent()->calcRealT(u[i], v[i]);

            setBoundaryT(boundary);
        }
    }
};

class EmptyQuadBoundaryT : public ChildQuadBoundaryT<EmptyQuadBoundaryT>
{
public:
    void* parent()
    {
        return nullptr;
    }

    bool containsInX( const Vector3F &pt )
    {
        return pt.x >= operator[](0).x && pt.x <= operator[](1).x;
    }

    bool containsInX( const Vector3F vec4[4] )
    {
        return vec4[0].x >= operator[](0).x && vec4[1].x <= operator[](1).x;
    }

    bool leftIntersectsInX( const Vector3F vec4[4] )
    {
        return vec4[0].x <= operator[](0).x && vec4[1].x >= operator[](0).x;
    }

    bool rightIntersectsInX( const Vector3F vec4[4] )
    {
        return vec4[0].x <= operator[](1).x && vec4[1].x >= operator[](1).x;
    }
};


template<class ParentT>
class AeroSectionBoundary : public ChildQuadBoundaryT<ParentT>
{
public:

    AeroSectionBoundary()
        : _chord(0.0)
    {
    }

    void setBoundaryT( const Vector3F vec4[4]) override
    {
        ChildQuadBoundaryT<ParentT>::setBoundaryT(vec4);
        calcFlowNormal();
        calcChord();
    }

    void calcFlowNormal()
    {
        Vector3F center = ( (*this)[0] + (*this)[1] + (*this)[2] + (*this)[3]) / 4.0f;
        Vector3F forward_center = ((*this)[3] +  (*this)[2])/2.0f;
        _flow_normal = (forward_center - center).toDouble();
        _flow_normal.Normalize();
    }

    void calcChord()
    {
        _chord = (this->calcRealT(0.5f, 1.0f) - this->calcRealT(0.5f, 0.0f)).Magnitude();
    }

    float span()  const
    {
        return (this->calcRealT(1.0f, 0.0f) - this->calcRealT(0.0f, 0.0f) ).Magnitude();
    }

    float getChord()  const
    {
        return _chord;
    }

    const Vector3D& getNormalFlow() const
    {
        return _flow_normal;
    }

    bool behind(const Vector3F& pt ) const
    {
        //return pt.x >= operator[](0).x && pt.x <= operator[](1).x && pt.z >= (operator[](2).z+operator[](3).z)/2.0f;
        return pt.x >= (*this)[0].x && pt.x <= (*this)[1].x && pt.z >= ((*this)[2].z+(*this)[3].z)/2.0f;
    }

private:
    Vector3D _flow_normal;
    float _chord;
};


