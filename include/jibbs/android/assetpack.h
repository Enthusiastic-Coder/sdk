#pragma once

#include <QString>

#ifdef Q_OS_ANDROID
struct AssetPackLocation {
    QString assetPath;
    int storage;
    QString path;
};


struct AssetLocation {
    QString path;
    long offset =0l;
    long size =0l;
};

#endif

QByteArray getDataFromAsset(const QString &assetPackName, const QString &fileName);
