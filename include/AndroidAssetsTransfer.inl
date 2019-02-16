#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>

#define androidAssetFolder "assets:/"

static void makePathForDestination(const QString& filePath)
{
    QFileInfo fi(filePath);
    QDir dir(QDir::currentPath());
    dir.mkpath(fi.absolutePath());
}

static void ExtractAsset(const QString filePath, const QString& folder=androidAssetFolder, bool bOverwrite=true)
{    
    QString fullFilePath= QString("%1%2").arg(folder).arg(filePath);

    if(bOverwrite)
        QFile::remove(filePath);

    makePathForDestination(filePath);

    QFile::copy(fullFilePath, filePath);
}


class AssetTempExtractor
{
public:
    AssetTempExtractor( const QStringList& fileList, const QString& folder=androidAssetFolder)
        :_fileList(fileList), _folder(folder)
    {
#ifdef Q_OS_WIN
    if( folder == androidAssetFolder )
        return;
#endif
        for(const QString& fileName:_fileList)
            ExtractAsset(fileName, androidAssetFolder);
    }

    static void RemoveAll(const QStringList& fileList)
    {
#ifndef Q_OS_WIN
        for(const QString& fileName:fileList)
            QFile::remove(fileName);
#endif
    }

    ~AssetTempExtractor()
    {
        if( !_autoRemove)
            return;

#ifdef Q_OS_WIN
    if( _folder == androidAssetFolder )
        return;
#endif
        RemoveAll(_fileList);
    }

    void setAutoRemove(bool autoRemove)
    {
        _autoRemove = autoRemove;
    }

private:
    const QStringList& _fileList;
    QString _folder;
    bool _autoRemove = true;
};
