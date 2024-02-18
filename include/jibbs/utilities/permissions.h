#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#include <QJniEnvironment>
#endif

class Permissions : public QObject
{
    Q_OBJECT
public:
    explicit Permissions(QObject *parent = nullptr);

    // Method to request permissions
    void requestExternalStoragePermission();

public slots:

private:
#ifdef Q_OS_ANDROID
    // Object used to obtain permissions on Android Marshmallow
    QJniObject ShowPermissionRationale;
#endif
};

#endif // PERMISSIONS_H
