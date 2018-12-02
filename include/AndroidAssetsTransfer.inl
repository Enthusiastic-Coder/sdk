#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>

#define rootFolder "assets:/"

static void ExtractAsset(const QString filePath, bool bOverwrite=true)
{
    QString fullFilePath= QString("assets:/%0").arg(filePath);
    if(bOverwrite)  QFile::remove(filePath);
    QFileInfo fi(filePath);
    QDir dir(QDir::currentPath());
    dir.mkpath(fi.absolutePath());
    QFile::copy(fullFilePath, filePath);
}


class AssetTempExtractor
{
public:
    AssetTempExtractor( const QStringList& fileList)
        :_fileList(fileList)
    {
#ifdef Q_OS_ANDROID
        for(const QString& fileName:_fileList)
            ExtractAsset(fileName);
#endif
    }

#ifdef Q_OS_ANDROID
    ~AssetTempExtractor()
    {
        for(const QString& fileName:_fileList)
            QFile::remove(fileName);
    }
#endif

private:
    const QStringList& _fileList;
};
