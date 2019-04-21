#ifndef GPSTILECONTAINER_H
#define GPSTILECONTAINER_H

#include <QString>
#include <GPSLocation.h>
#include <vector>
#include <map>

#include <memory>

#include <GPSBoundary.h>

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
    }

    const std::map<QString,T>& getTile(const GPSLocation& pt) const
    {
        return _tiles[GPSToTileIndex(pt)];
    }

    const std::map<int,std::map<QString,T>>& getTiles() const
    {
        return _tiles;
    }

    void add(const QString& key, T& item, const GPSLocation& pos)
    {
        int index = GPSToTileIndex(pos);
        if( index < 0 || index >= _divisions*_divisions)
            return;

        _tiles[index][key] = item;
    }

    void setViewBoundary(const GPSLocation& tL, const GPSLocation& bR) const
    {
        _viewableTiles.clear();
        GPSBoundary b(tL, bR);

        for( const auto& item : _tiles )
        {
            if(b.intersects(boundaryFromIndex(item.first)))
                _viewableTiles.push_back(item.first);
        }
    }

    const std::vector<int>& getViewableTileIds() const
    {
        return _viewableTiles;
    }

    const std::map<QString,T>& getTile(int idx) const
    {
        auto it = _tiles.find(idx);

        if( it == _tiles.end())
        {
            static std::map<QString,T> empty;
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

private:
    GPSBoundary _boundary;
    double _cellWidth;
    double _cellHeight;
    int _divisions;

    std::map<int,std::map<QString,T>> _tiles;
    mutable std::vector<int> _viewableTiles;
};

#endif // GPSTILECONTAINER_H
