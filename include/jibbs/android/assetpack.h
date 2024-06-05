#pragma once

#include <QString>

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


class AssetPack
{
public:
    AssetPack(QString javaNamespace);

    QByteArray getDataFromAsset(const QString &assetPackName, const QString &fileName);

    AssetPackLocation getAssetPackLocation(const QString &assetPackName);
    AssetLocation getAssetLocation(const QString &assetPackName, const QString &fileName);

private:
    QString _javaNamespace;
};
