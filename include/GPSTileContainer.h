#ifndef GPSTILECONTAINER_H
#define GPSTILECONTAINER_H

#include <QString>
#include <GPSLocation.h>
#include <vector>
#include <map>

#include <memory>

template<class T>
class GPSTileContainer
{
public:
    void setBounds(const GPSLocation& topLeft, const GPSLocation& bottomRight, int divisions)
    {
        _topLeft = topLeft;
        _divisions = divisions;

        GPSLocation topRight(_topLeft._lat, bottomRight._lng);
        _cellWidth = (topRight._lng - topLeft._lng)/divisions;

        GPSLocation bottomLeft(bottomRight._lat, _topLeft._lng);
        _cellHeight = (bottomLeft._lat - topLeft._lat)/divisions;
    }

    const std::map<QString,T*>& getTile(const GPSLocation& pt) const
    {
        return _tiles[GPSToTileIndex(pt)];
    }

    const std::map<int,std::map<QString,std::unique_ptr<T>>>& getTiles() const
    {
        return _tiles;
    }

    void add(const QString& key, T* item, const GPSLocation& pos)
    {
        _tiles[GPSToTileIndex(pos)][key] = std::unique_ptr<T>(item);
    }

protected:
    int GPSToTileIndex(const GPSLocation& pos)
    {
        float dW = pos._lng - _topLeft._lng;
        float dH = pos._lat - _topLeft._lat;

        int idxH = dH/ _cellHeight;
        int idxW = dW / _cellWidth;

        return idxH * _divisions + idxW;
    }

private:
    GPSLocation _topLeft;
    float _cellWidth;
    float _cellHeight;
    int _divisions;

    std::map<int,std::map<QString,std::unique_ptr<T>>> _tiles;
};

#endif // GPSTILECONTAINER_H
