#ifndef GPSTILECONTAINER_H
#define GPSTILECONTAINER_H

#include <QString>
#include <vector>
#include <map>

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

        initializeTileBoundaries();
    }

    const std::vector<T>& getTile(const GPSLocation& pt) const
    {
        return _tiles[GPSToTileIndex(pt)];
    }

    void add(const T& item, const GPSLocation& pos)
    {
        int index = GPSToTileIndex(pos);
        if( index < 0 || index >= _divisions*_divisions)
            return;

        _tiles[index].push_back(item);
    }

    void setViewBoundary(const GPSLocation& tL, const GPSLocation& bR)
    {
        _viewableTiles.clear();
        GPSBoundary b(tL, bR);

        // Compute the range of tile indices based on GPS boundaries
        int minX = std::floor((tL._lng - _boundary.topLeft()._lng) / _cellWidth);
        int maxX = std::floor((bR._lng - _boundary.topLeft()._lng) / _cellWidth);
        int minY = std::floor((tL._lat - _boundary.topLeft()._lat) / _cellHeight);
        int maxY = std::floor((bR._lat - _boundary.topLeft()._lat) / _cellHeight);

        // Clamp the range to valid indices
        minX = std::max(0, minX);
        maxX = std::min(_divisions - 1, maxX);
        minY = std::max(0, minY);
        maxY = std::min(_divisions - 1, maxY);

        // Iterate over the relevant range and check if the tile exists in the map
        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                int tileIndex = y * _divisions + x; // Convert (x, y) to tile index
                auto it = _tiles.find(tileIndex);  // Check if the tile exists

                if (_tiles.find(tileIndex) != _tiles.end() && b.intersects(_tileBoundaries[tileIndex]))
                {
                    _viewableTiles.push_back(tileIndex);
                }
            }
        }
    }

    const std::vector<int>& getViewableTileIds() const
    {
        return _viewableTiles;
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

protected:
    int GPSToTileIndex(const GPSLocation& pos) const
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

    GPSBoundary boundaryFromIndex(int i) const
    {
        int x = i % _divisions;
        int y = i/ _divisions;

        int x2 = (i+1) % _divisions;
        int y2 = (i+ _divisions +1) / _divisions;

        GPSLocation tL;
        tL._lng = _boundary.topLeft()._lng + _cellWidth * x;
        tL._lat = _boundary.topLeft()._lat + _cellHeight * y;

        GPSLocation bR;
        bR._lng = _boundary.topLeft()._lng + _cellWidth * x2;
        bR._lat = _boundary.topLeft()._lat + _cellHeight * y2;

        return {tL, bR};
    }

    void initializeTileBoundaries()
    {
        _tileBoundaries.resize(_divisions * _divisions);
        for (int i = 0; i < _divisions * _divisions; ++i)
        {
            _tileBoundaries[i] = boundaryFromIndex(i); // Use the simplified function here
        }
    }

private:
    std::vector<GPSBoundary> _tileBoundaries;
    GPSBoundary _boundary;
    double _cellWidth;
    double _cellHeight;
    int _divisions;

    std::map<int,std::vector<T>> _tiles;
    std::vector<int> _viewableTiles;
};

#endif // GPSTILECONTAINER_H
