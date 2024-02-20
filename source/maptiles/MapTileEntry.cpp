#include <jibbs/maptiles/MapTileEntry.h>

#include <QSettings>

MapTileEntry::MapTileEntry(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<MapTileEntry*>();
}

void MapTileEntry::save(QSettings &s) const
{
    s.setValue("url", _url);
    s.setValue("url2", _url2);

    s.setValue("showRunway", _showRunway);
    s.setValue("smoothedTexture", _smoothedTexture);

    s.setValue("showZoom12", _showZoom12);
    s.setValue("showZoom13", _showZoom13);
    s.setValue("showZoom14", _showZoom14);

    s.setValue("user", _user);
    s.setValue("password", _password);
}

void MapTileEntry::load(QSettings &s)
{
    _url = s.value("url").toString();
    _url2 = s.value("url2").toString();

    _showRunway = s.value("showRunway", true).toBool();
    _smoothedTexture = s.value("smoothedTexture", true).toBool();

    _showZoom12 = s.value("showZoom12", true).toBool();
    _showZoom13 = s.value("showZoom13", true).toBool();
    _showZoom14 = s.value("showZoom14", true).toBool();

    _user = s.value("user").toString();
    _password = s.value("password").toString();
}

QString MapTileEntry::url() const
{
    return _url;
}

QString MapTileEntry::url2() const
{
    return _url2;
}

bool MapTileEntry::showRunway() const
{
    return _showRunway;
}

bool MapTileEntry::smoothedTexture() const
{
    return _smoothedTexture;
}

bool MapTileEntry::showZoom12() const
{
    return _showZoom12;
}

bool MapTileEntry::showZoom13() const
{
    return _showZoom13;
}

bool MapTileEntry::showZoom14() const
{
    return _showZoom14;
}

QString MapTileEntry::user() const
{
    return _user;
}

QString MapTileEntry::password() const
{
    return _password;
}

