#ifndef GPSBOUNDARY_H
#define GPSBOUNDARY_H

#include <GPSLocation.h>
#include <QPair>

class GPSBoundary
{
public:
    GPSBoundary() {}

    GPSBoundary(const GPSLocation &topLeft, const GPSLocation &bottomRight)
        :_topLeft(topLeft), _bottomRight(bottomRight)
    {

    }

    GPSBoundary(const QPair<GPSLocation, GPSLocation> &box)
       : GPSBoundary( box.first, box.second)
    {

    }

    const GPSLocation& topLeft() const
    {
        return _topLeft;
    }

    const GPSLocation& bottomRight() const
    {
        return _bottomRight;
    }

    bool overlaps(const GPSBoundary &other) const
    {
        if( topLeft()._lng > other.bottomRight()._lng ||
                bottomRight()._lng < other.topLeft()._lng)
            return false;

        if( topLeft()._lat < other.bottomRight()._lat ||
                bottomRight()._lat > other.topLeft()._lat)
            return false;

        return true;
    }

private:
    GPSLocation _topLeft;
    GPSLocation _bottomRight;
};

#endif // GPSBOUNDARY_H
