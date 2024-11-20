#pragma once

#include <jibbs/vector/vector2.h>
#include <jibbs/vector/vector3.h>
#include <jibbs/vector/vector4.h>
#include <jibbs/matrix/Matrix3x3.h>
#include <jibbs/matrix/Matrix4x4.h>
#include <jibbs/math/Quarternion.h>

#include <jibbs/boundary/BoundingBox.h>
#include <jibbs/gps/GPSLocation.h>

#include <jibbs/math/Frustum.h>
#include <jibbs/boundary/BoundaryHelperT.h>
#include <jibbs/math/Plane.h>
#include <jibbs/matrix/MatrixArrayCoords.h>

#include "HeightData.h"
#include "MeshHelper.h"

struct IReadStream
{
    virtual void Read(bool&) = 0;
    virtual void Read(char&) = 0;
    virtual void Read(short&) = 0;
    virtual void Read(int&) = 0;
    virtual void Read(long&) = 0;
    virtual void Read(float&) = 0;
    virtual void Read(double&) = 0;
    virtual void Read(size_t&) = 0;
    virtual void Read(unsigned short&) = 0;
    virtual void Read(GPSLocation &) = 0;
    virtual void Read(std::string&) = 0;
    virtual void Read(Vector3F&) = 0;
    virtual void Read(Vector4F&) = 0;
    virtual void Read(Vector3D&) = 0;
    virtual void Read(Vector2D&) = 0;
    virtual void Read(Vector2F&) = 0;
    virtual void Read(Matrix3x3F&) = 0;
    virtual void Read(Matrix3x3D&) = 0;
    virtual void Read(Matrix4x4F&) = 0;
    virtual void Read(Matrix4x4D&) = 0;
    virtual void Read(QuarternionF&) = 0;
    virtual void Read(QuarternionD&) = 0;
    virtual void Read(BoundingBox&) = 0;
    virtual void Read(MatrixArrayCoords&) = 0;
    virtual void Read(PlaneF&) = 0;
};


struct IWriteStream
{
    virtual void Write( const bool& ) = 0;
    virtual void Write(const char&) = 0;
    virtual void Write(const short&) = 0;
    virtual void Write(const int&) = 0;
    virtual void Write(const long&) = 0;
    virtual void Write(const float&) = 0;
    virtual void Write(const double&) = 0;
    virtual void Write(const size_t&) = 0;
    virtual void Write(const unsigned short&) = 0;
    virtual void Write(const GPSLocation &) = 0;
    virtual void Write(const std::string&) = 0;
    virtual void Write(const Vector3F&) = 0;
    virtual void Write(const Vector4F&) = 0;
    virtual void Write(const Vector3D&) = 0;
    virtual void Write(const Vector2D&) = 0;
    virtual void Write(const Vector2F&) = 0;
    virtual void Write(const Matrix3x3F&) = 0;
    virtual void Write(const Matrix3x3D&) = 0;
    virtual void Write(const Matrix4x4F&) = 0;
    virtual void Write(const Matrix4x4D&) = 0;
    virtual void Write(const QuarternionF&) = 0;
    virtual void Write(const QuarternionD&) = 0;
    virtual void Write(const BoundingBox&) = 0;
    virtual void Write(const MatrixArrayCoords&) = 0;
    virtual void Write(const PlaneF&) = 0;
};


struct ISerializable
{
    void Write( IWriteStream* ) const {}
    void Read( IReadStream* ) {}
};


#include "SerializableMacros.h"
