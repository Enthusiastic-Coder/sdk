#ifndef MAPTILEENTRY_H
#define MAPTILEENTRY_H

#include <QObject>
#include <QSettings>
#include <QStringList>

class MapTileEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url MEMBER _url NOTIFY urlChanged)
    Q_PROPERTY(QString url2 MEMBER _url2 NOTIFY url2Changed)

    Q_PROPERTY(bool showRunway MEMBER _showRunway NOTIFY showRunwayChanged)
    Q_PROPERTY(bool smoothedTexture MEMBER _smoothedTexture NOTIFY smoothTextureChanged)

    Q_PROPERTY(bool showZoom12 MEMBER _showZoom12 NOTIFY zoom12Changed)
    Q_PROPERTY(bool showZoom13 MEMBER _showZoom13 NOTIFY zoom13Changed)
    Q_PROPERTY(bool showZoom14 MEMBER _showZoom14 NOTIFY zoom14Changed)

    Q_PROPERTY(QString userName MEMBER _user NOTIFY userNameChanged)
    Q_PROPERTY(QString password MEMBER _password NOTIFY passwordChanged)

public:
    MapTileEntry(QObject *parent=nullptr);

    void save(QSettings& s) const;
    void load(QSettings& s);

    QString url() const;
    QString url2() const;

    bool showRunway() const;
    bool smoothedTexture() const;
    bool showZoom12() const;
    bool showZoom13() const;
    bool showZoom14() const;

    QString user() const;
    QString password() const;

private:

    QString _url;
    QString _url2;

    bool _showRunway;
    bool _smoothedTexture;
    bool _showZoom12;
    bool _showZoom13;
    bool _showZoom14;

    QString _user;
    QString _password;

signals:
    void urlChanged();
    void url2Changed();

    void showRunwayChanged();
    void smoothTextureChanged();

    void zoom12Changed();
    void zoom13Changed();
    void zoom14Changed();

    void userNameChanged();
    void passwordChanged();
};

Q_DECLARE_METATYPE(MapTileEntry*)

#endif // MAPTILEENTRY_H
