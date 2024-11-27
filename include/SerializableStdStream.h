#pragma once

#include <jibbs/math/MathSupport.h>
#include <Serializable.h>
#include <fstream>

class BinaryWriteStream : public IWriteStream
{
public:
    BinaryWriteStream( std::ofstream& os) : _os(os ) {}

protected:

    // Implement Write for raw data
    void Write(const void* data, size_t size) override
    {
        _os.write(static_cast<const char*>(data), size);
    }

    // Overload specifically for std::string
    virtual void Write(const std::string& str) override
    {
        size_t length( str.length() );
        _os.write((const char*)&length, sizeof(length));
        _os.write((const char*)str.c_str(), length);
    }

private:
    std::ofstream& _os;
};

class BinaryReadStream : public IReadStream
{
public:
    BinaryReadStream( std::ifstream& is) : _is( is ) {}

protected:
    // Implement raw Read for void* data
    void Read(void* data, size_t size) override
    {
        _is.read(static_cast<char*>(data), size);
    }

    // Implement the std::string overload
    void Read(std::string& str) override
    {
        size_t length;
        _is.read((char*)&length, sizeof(length));
        if( length )
        {
            str.resize( length );
            _is.read((char*)&*str.begin(), length);
        }
    }

private:
    std::ifstream& _is;
};
#undef IMPL


