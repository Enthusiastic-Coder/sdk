#pragma once

#include <MathSupport.h>
#include <Serializable.h>
#include <fstream>

#define IMPL( var )  override { Write( _os, var ); }

class BinaryWriteStream : public IWriteStream
{
public:
    BinaryWriteStream( std::ofstream& os) : _os(os ) {}

    void Write( const bool& b) IMPL(b)
    void Write( const char& ch) IMPL(ch)
    void Write( const short& s) IMPL(s)
    void Write( const int& i) IMPL(i)
    void Write( const long& l) IMPL(l)
    void Write( const size_t& size) IMPL(size)
    void Write( const float& f) IMPL(f)
    void Write( const double& d) IMPL(d)
    void Write( const unsigned short& d) IMPL(d)
    void Write( const std::string& str) IMPL(str)
    void Write( const GPSLocation &g ) IMPL(g)
    void Write( const Vector3F& v) IMPL(v)
    void Write( const Vector4F& v) IMPL(v)
    void Write( const Vector3D& v) IMPL(v)
    void Write( const Vector2D& v ) IMPL(v)
    void Write( const Vector2F& v ) IMPL(v)
    void Write( const Matrix3x3F& m) IMPL(m)
    void Write( const Matrix3x3D& m) IMPL(m)
    void Write( const Matrix4x4F& m) IMPL(m)
    void Write( const Matrix4x4D& m) IMPL(m)
    void Write( const QuarternionF& q) IMPL(q)
    void Write( const QuarternionD& q) IMPL(q)
    void Write( const BoundingBox& b) IMPL(b)
    void Write( const MatrixArrayCoords& m ) IMPL(m)
    void Write( const PlaneF& p ) IMPL(p)

    protected:
        template<typename T>
        void Write( std::ofstream& os, const T& t )
    {
        os.write( (const char*) &t, sizeof(T ) );
    }

    void Write( std::ofstream& os, const std::string& str )
    {
        size_t length( str.length() );
        os.write((const char*)&length, sizeof(length));
        os.write((const char*)str.c_str(), length);
    }

private:
    std::ofstream& _os;
};

#undef IMPL
#define IMPL( var )  override { return Read( _is, var ); }

class BinaryReadStream : public IReadStream
{
public:
    BinaryReadStream( std::ifstream& is) : _is( is ) {}

    void Read( bool& b) IMPL(b)
    void Read( char& ch) IMPL(ch)
    void Read( short& s) IMPL(s)
    void Read( int& i) IMPL(i)
    void Read( long& l) IMPL(l)
    void Read( size_t& size) IMPL(size)
    void Read( float& f) IMPL(f)
    void Read( double& d) IMPL(d)
    void Read( unsigned short&s) IMPL(s)
    void Read( std::string& str) IMPL(str)
    void Read( GPSLocation &g ) IMPL(g)
    void Read( Vector3F& v) IMPL(v)
    void Read( Vector4F& v) IMPL(v)
    void Read( Vector3D& v) IMPL(v)
    void Read( Vector2D& v) IMPL(v)
    void Read( Vector2F& v) IMPL(v)
    void Read( Matrix3x3F& m) IMPL(m)
    void Read( Matrix3x3D& m) IMPL(m)
    void Read( Matrix4x4F& m) IMPL(m)
    void Read( Matrix4x4D& m) IMPL(m)
    void Read( QuarternionF& q) IMPL(q)
    void Read( QuarternionD& q) IMPL(q)
    void Read( BoundingBox& b) IMPL(b)
    void Read( MatrixArrayCoords& m ) IMPL(m)
    void Read( PlaneF& p ) IMPL(p)

    protected:

        template<typename T>
                          void Read( std::ifstream& is, T& t )
    {
        is.read( (char*) &t, sizeof(T ) );
    }

    void Read( std::ifstream& is, std::string& str )
    {
        size_t length;
        is.read((char*)&length, sizeof(length));
        if( length )
        {
            str.resize( length );
            is.read((char*)&*str.begin(), length);
        }
    }

private:
    std::ifstream& _is;
};
#undef IMPL


