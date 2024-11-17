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
        if( isNotExtractable(folder))
            return;

        for(const QString& fileName:_fileList)
            ExtractAsset(fileName, folder);
    }

    static void RemoveAll(const QStringList& fileList, const QString& folder=androidAssetFolder)
    {
        if( isNotExtractable(folder))
            return;

        for(const QString& fileName:fileList)
        {
#ifdef Q_OS_WIN
        QFile(fileName).setPermissions(QFile::Permission::WriteOwner);
#endif
            QFile::remove(fileName);
        }
    }

    ~AssetTempExtractor()
    {
        if( !_autoRemove)
            return;

        RemoveAll(_fileList, _folder);
    }

    void setAutoRemove(bool autoRemove)
    {
        _autoRemove = autoRemove;
    }

    static bool isNotExtractable(const QString& folder)
    {
    #ifdef Q_OS_WIN
        if( folder == androidAssetFolder )
            return true;
    #endif

        return false;
    }

private:
    const QStringList& _fileList;
    QString _folder;
    bool _autoRemove = true;
};
