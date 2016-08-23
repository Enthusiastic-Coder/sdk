#pragma once
#include <utility>

class MatrixArrayCoords
{
public:

    MatrixArrayCoords()
    {
        setDims(0);
    }

    MatrixArrayCoords( int dims )
    {
        setDims(dims);
    }

    MatrixArrayCoords( int dimsX, int dimsY )
    {
        setDims(dimsX, dimsY);
    }

    void setDims( int dims)
    {
        setDims(dims ,dims);
    }

    void setDims( int dimsX, int dimsY )
    {
        _dims = std::make_pair(dimsX, dimsY);
    }

    int toArrayPos( int x, int y ) const
    {
        return y * _dims.first + x;
    }

    std::pair<int,int> toMatrixPos( int pos ) const
    {
        return std::make_pair( pos % _dims.first, pos / _dims.second );
    }

    std::pair<int,int> getDims() const
    {
        return _dims;
    }

    int product() const
    {
        return _dims.first * _dims.second;
    }

    int getX() const
    {
        return _dims.first;
    }

    int getY() const
    {
        return _dims.second;
    }

protected:
    std::pair<int,int> _dims;
};
