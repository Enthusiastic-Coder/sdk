#pragma once

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quarternion.h"
#include "BoundingBox.h"
#include "GPSLocation.h"

#include "Frustum.h"
#include "HeightData.h"
#include "MeshHelper.h"
#include "BoundaryHelperT.h"
#include "BoundingBox.h"
#include "MatrixArrayCoords.h"
#include "Plane.h"


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
