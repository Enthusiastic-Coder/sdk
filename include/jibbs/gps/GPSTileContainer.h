#ifndef GPSTILECONTAINER_H
#define GPSTILECONTAINER_H

#include <QString>
#include <vector>
#include <unordered_map>

#include "GPSBoundary.h"
#include "GPSLocation.h"

template<class T>
class GPSTileContainer
{
public:
    void setBounds(const GPSLocation& topLeft, const GPSLocation& bottomRight, int divisions)
    {
        _boundary = GPSBoundary(topLeft, bottomRight);
        _divisions = divisions;

        _cellWidth = (bottomRight._lng - topLeft._lng)/divisions;
        _cellHeight = (bottomRight._lat - topLeft._lat)/divisions;

        _tileBoundaries.resize(_divisions * _divisions);

        for (int i = 0; i < _divisions * _divisions; ++i)
        {
            _tileBoundaries[i] = getBoundaryFromIndex(i); // Use the simplified function here
        }
    }

    void add(const T& item, const GPSLocation& pos)
    {
        int index = getIndexFromGPS(pos);
        if( index < 0 || index >= _divisions*_divisions)
            return;

        _tiles[index].push_back(item);
    }

    std::vector<int> getViewableTiles(const GPSLocation& tL, const GPSLocation& bR) const
    {
        std::vector<int> viewables;
        // Compute the range of tile indices based on GPS boundaries
        int minX, maxX, minY, maxY;

        GPSBoundary b(tL, bR);

        getIndexBoundsFromGPSBounds(b, minX, maxX, minY, maxY);

        // Iterate over the relevant range and check if the tile exists in the map
        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                int tileIndex = y * _divisions + x; // Convert (x, y) to tile index
                auto it = _tiles.find(tileIndex);  // Check if the tile exists

                if (it != _tiles.end() && b.intersects(_tileBoundaries[tileIndex]))
                {
                    viewables.push_back(tileIndex);
                }
            }
        }

        return viewables;
    }

    void setViewBoundary(const GPSLocation& tL, const GPSLocation& bR)
    {
        _viewableTiles = getViewableTiles(tL, bR);
    }

    const std::vector<int>& getViewableTileIds() const
    {
        return _viewableTiles;
    }

    const std::vector<T>& getTile(const GPSLocation& pt) const
    {
        return getTile(getIndexFromGPS(pt));
    }

    const std::vector<T>& getTile(int idx) const
    {
        auto it = _tiles.find(idx);

        if( it == _tiles.end())
        {
            static std::vector<T> empty;
            return empty;
        }

        return it->second;
    }

    void getIndexBoundsFromGPSBounds(const GPSBoundary& b, int& minX, int &maxX, int &minY, int &maxY) const
    {
        // Compute the range of tile indices based on GPS boundaries
        minX = std::floor((b.topLeft()._lng - _boundary.topLeft()._lng) / _cellWidth);
        maxX = std::floor((b.bottomRight()._lng - _boundary.topLeft()._lng) / _cellWidth);

        minY = std::floor((b.topLeft()._lat - _boundary.topLeft()._lat) / _cellHeight);
        maxY = std::floor((b.bottomRight()._lat - _boundary.topLeft()._lat) / _cellHeight);

        // Clamp the range to valid indices
        minX = std::max(0, minX);
        maxX = std::min(_divisions - 1, maxX);
        minY = std::max(0, minY);
        maxY = std::min(_divisions - 1, maxY);
    }

    GPSBoundary getBoundaryFromIndex(int index) const
    {
        int x = index % _divisions;
        int y = index/ _divisions;

        int x2 = (index+1) % _divisions;
        int y2 = (index+ _divisions +1) / _divisions;

        GPSLocation tL;
        tL._lng = _boundary.topLeft()._lng + _cellWidth * x;
        tL._lat = _boundary.topLeft()._lat + _cellHeight * y;

        GPSLocation bR;
        bR._lng = _boundary.topLeft()._lng + _cellWidth * x2;
        bR._lat = _boundary.topLeft()._lat + _cellHeight * y2;

        return {tL, bR};
    }

    int getIndexFromGPS(const GPSLocation& pos) const
    {
        double dW = pos._lng - _boundary.topLeft()._lng;
        double dH = pos._lat - _boundary.topLeft()._lat;

        int cellX = std::floor(dW / _cellWidth);
        int cellY = std::floor(dH/ _cellHeight);

        if( cellX <0 || cellX >= _divisions)
            return -1;

        if( cellY < 0 || cellY >= _divisions)
            return -1;

        return cellY * _divisions + cellX;
    }

private:
    std::vector<GPSBoundary> _tileBoundaries;
    GPSBoundary _boundary;
    double _cellWidth;
    double _cellHeight;
    int _divisions;

    std::unordered_map<int,std::vector<T>> _tiles;
    std::vector<int> _viewableTiles;
};

#endif // GPSTILECONTAINER_H
