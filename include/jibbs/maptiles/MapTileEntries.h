#ifndef MAPTILEENTRIES_H
#define MAPTILEENTRIES_H

#include "IPersist.h"

#include <QObject>
#include <QSettings>
#include <QVariantMap>

class MapTileEntry;

class MapTileEntries : public QObject, public IQSettingPersist
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap Tiles MEMBER _entries NOTIFY tilesChanged)
    Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool showZoom READ getShowZoom WRITE setShowZoom NOTIFY showZoomChanged)
    Q_PROPERTY(QString activeProfile READ getActiveName WRITE setActiveName NOTIFY activeNameChanged)

public:
    explicit MapTileEntries(QObject* parent, QString settingsPath);

    virtual void saveSettings() override;
    virtual void loadSettings() override;

    Q_INVOKABLE bool add(QString name);
    Q_INVOKABLE bool remove(QString name);
    Q_INVOKABLE void doUpdate();

    QVariantMap getEntries() const;

    bool getActive() const;
    QString getActiveName() const;
    bool getShowZoom() const;

    const MapTileEntry* getActiveEntry() const;

private:
    void setActive(bool active);
    void setActiveName(QString name);
    void setShowZoom(bool show);

private:

    bool _active = false;
    bool _showZoom = false;
    QString _settingsPath;
    QString _activeName;
    QVariantMap _entries;

signals:
    void tilesChanged();
    void activeNameChanged();
    void activeChanged();
    void showZoomChanged();
    void tileUpdate();
};

#endif // MAPTILEENTRIES_H
