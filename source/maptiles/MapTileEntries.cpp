#include <jibbs/maptiles/MapTileEntries.h>
#include <jibbs/maptiles/MapTileEntry.h>

#include <QSettings>

MapTileEntries::MapTileEntries(QObject *parent, QString settingsPath) : QObject{parent}, _settingsPath(settingsPath)
{}

void MapTileEntries::saveSettings()
{
    QSettings s;

    s.beginGroup(_settingsPath);

    s.setValue("active", _active);
    s.setValue("activeName", _activeName);
    s.setValue("showZoom", _showZoom);

    for(auto it = _entries.constBegin(); it != _entries.constEnd(); ++it)
    {
        const MapTileEntry* entry = it->value<MapTileEntry*>();

        s.beginGroup(it.key());
        entry->save(s);
        s.endGroup();
    }
    s.endGroup();
}

void MapTileEntries::loadSettings()
{
    QSettings s;

    _entries.clear();

    s.beginGroup(_settingsPath);

    const QStringList groups = s.childGroups();

    for(const auto& group : groups)
    {
        MapTileEntry* entry = new MapTileEntry;
        s.beginGroup(group);
        entry->load(s);
        s.endGroup();
        _entries[group] = QVariant::fromValue(entry);
    }

    setActive(s.value("active").toBool());
    setActiveName(s.value("activeName").toString());
    setShowZoom(s.value("showZoom", false).toBool());

    s.endGroup();

    emit tileUpdate();
}

bool MapTileEntries::add(QString name)
{
    auto it = _entries.find(name);
    if( it != _entries.end())
        return false;

    MapTileEntry* entry = new MapTileEntry;
    _entries[name] = QVariant::fromValue(entry);

    emit tilesChanged();
    return true;
}

bool MapTileEntries::remove(QString name)
{
    auto it = _entries.find(name);
    if( it == _entries.end())
        return false;

    QSettings s;
    s.beginGroup(_settingsPath);
    s.remove(name);
    s.endGroup();

    _entries.remove(name);
    emit tilesChanged();
    return true;
}

void MapTileEntries::doUpdate()
{
    emit tileUpdate();
}

QVariantMap MapTileEntries::getEntries() const
{
    return _entries;
}

bool MapTileEntries::getActive() const
{
    return _active;
}

QString MapTileEntries::getActiveName() const
{
    return _activeName;
}

bool MapTileEntries::getShowZoom() const
{
    return _showZoom;
}

const MapTileEntry *MapTileEntries::getActiveEntry() const
{
    auto it = _entries.find(_activeName);
    if(  it == _entries.end())
        return nullptr;

    return it->value<MapTileEntry*>();
}

void MapTileEntries::setActive(bool active)
{
    if( _active == active)
        return;

    _active = active;
    emit activeChanged();
}

void MapTileEntries::setActiveName(QString name)
{
    if( _activeName == name)
        return;

    _activeName = name;
    emit activeNameChanged();
}

void MapTileEntries::setShowZoom(bool show)
{
    if( _showZoom == show)
        return;

    _showZoom = show;
    emit showZoomChanged();
}

