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


struct IReadStream
{
    virtual ~IReadStream() = default;

    // Abstract Read function for raw data
    virtual void Read(void* data, size_t size) = 0;

    // Template Read function for generic types
    template<typename T>
    void Read(T& var)
    {
        Read(static_cast<void*>(&var), sizeof(T));
    }

    // Overload for std::string
    virtual void Read(std::string& str) = 0;
};


struct IWriteStream
{
    virtual ~IWriteStream() = default;

    // Abstract Write function with void* to handle any type
    virtual void Write(const void* data, size_t size) = 0;

    // Template Write for general types
    template<typename T>
    void Write(const T& var)
    {
        Write(static_cast<const void*>(&var), sizeof(T));
    }

    // Overload specifically for std::string
    virtual void Write(const std::string& str) = 0;
};


struct ISerializable
{
    void Write( IWriteStream* ) const {}
    void Read( IReadStream* ) {}
};


#include "SerializableMacros.h"
