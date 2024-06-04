
#include <jibbs/android/assetpack.h>
#include <QFile>

#ifdef Q_OS_ANDROID

#include <QJniObject>
#include <QJniEnvironment>
#include <QCoreApplication>

AssetPackLocation getAssetPackLocation(const QString &assetPackName)
{
    QJniObject context = QNativeInterface::QAndroidApplication::context();
    QJniObject assetHelper("com/enthusiasticcoder/aihorizon/AssetPackHelper",
                           "(Landroid/content/Context;)V",
                           context.object<jobject>());

    QJniObject jAssetPackName = QJniObject::fromString(assetPackName);

    QJniObject assetLocation = assetHelper.callObjectMethod("getPackLocation",
                                                            "(Ljava/lang/String;)Lcom/google/android/play/core/assetpacks/AssetPackLocation;",
                                                            jAssetPackName.object<jstring>());

    AssetPackLocation location;
    if (assetLocation.isValid())
    {
        location.assetPath = assetLocation.callObjectMethod("assetsPath", "()Ljava/lang/String;").toString();
        location.path = assetLocation.callObjectMethod("path", "()Ljava/lang/String;").toString();
        location.storage = assetLocation.callMethod<jint>("packStorageMethod", "()I");
    }
    else
    {
        location.path = "Failed " + assetPackName;
    }

    return location;
}

AssetLocation getAssetLocation(const QString &assetPackName, const QString &fileName) {
    QJniObject context = QNativeInterface::QAndroidApplication::context();
    QJniObject assetHelper("com/enthusiasticcoder/aihorizon/AssetPackHelper",
                           "(Landroid/content/Context;)V",
                           context.object<jobject>());

    QJniObject jAssetPackName = QJniObject::fromString(assetPackName);
    QJniObject jFileName = QJniObject::fromString(fileName);

    QJniObject assetLocation = assetHelper.callObjectMethod("getAssetLocation",
                                                            "(Ljava/lang/String;Ljava/lang/String;)Lcom/google/android/play/core/assetpacks/AssetLocation;",
                                                            jAssetPackName.object<jstring>(), jFileName.object<jstring>());

    AssetLocation location;
    if (assetLocation.isValid()) {
        location.path = assetLocation.callObjectMethod("path", "()Ljava/lang/String;").toString();
        location.offset = assetLocation.callMethod<jlong>("offset", "()J");
        location.size = assetLocation.callMethod<jlong>("size", "()J");
    }
    else
    {
        location.path = "Failed to get asset location for" + assetPackName + "/" + fileName;
    }

    return location;
}
#endif

QByteArray getDataFromAsset(const QString &assetPackName, const QString &fileName)
{
#ifdef Q_OS_ANDROID
    AssetLocation loc = getAssetLocation(assetPackName, fileName);

    QByteArray data;

    QFile file(loc.path);

    if( file.open(QIODevice::ReadOnly))
    {
        file.seek(loc.offset);
        data = file.read(loc.size);
    }

    return data;
#else
    QFile file(assetPackName + "/" + fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        return file.readAll();
    }

    return QByteArray{};
#endif

}
