#include <jibbs/utilities/permissions.h>

#include <QFuture>

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>

namespace {

bool shouldShowRequestPermissionRationale(const QString &permission)
{
    if (QtAndroidPrivate::androidSdkVersion() < 23)
        return false;

    return QJniObject(QtAndroidPrivate::activity()).callMethod<jboolean>("shouldShowRequestPermissionRationale", "(Ljava/lang/String;)Z",
                                                                         QJniObject::fromString(permission).object());
}

}
#endif


Permissions::Permissions(QObject *parent) : QObject(parent)
{
}

// Method to request permissions
void Permissions::requestExternalStoragePermission()
{
#ifdef Q_OS_ANDROID
    const QString permission{"android.permission.READ_EXTERNAL_STORAGE"};

    QtAndroidPrivate::PermissionResult result = QtAndroidPrivate::checkPermission(permission).result();

    if( result == QtAndroidPrivate::Authorized)
        return;

    result = QtAndroidPrivate::requestPermission(permission).result();

    if( result == QtAndroidPrivate::Denied)
    {
        if (shouldShowRequestPermissionRationale(permission))
        {
            QJniObject activity = QNativeInterface::QAndroidApplication::context();

            ShowPermissionRationale = QJniObject("com/enthusiasticcoder/adsbflightmonitor/ShowPermissionRationale",
                                               "(Landroid/app/Activity;)V",
                                               activity.object<jobject>());

            // Checking for errors in the JNI
            QJniEnvironment env;
            if (env->ExceptionCheck()) {
                // Handle exception here.
                env->ExceptionClear();
            }
        }
    }
#endif
}
