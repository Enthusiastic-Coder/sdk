#include <QDir>
#include <QString>
#include <QStringList>

static void copyAssets(bool bOverWrite, QStringList ignoreFolderList)
{
    QDir curDir(QDir::current());
    QString assets("assets:/");

    QStringList assetFolderList = QDir(assets).entryList(QDir::NoDotAndDotDot|QDir::Dirs);

    foreach(QString assetFolder, assetFolderList)
    {
        if( assetFolder.startsWith("--"))
            continue;

        bool bIgnoreFile(false);

        for(const QString& folder:ignoreFolderList)
        {
            if( assetFolder.startsWith(folder))
            {
                bIgnoreFile = true;
                break;
            }
        }

        bool b3 = curDir.mkdir(assetFolder);

        if(bIgnoreFile)
            continue;

        QStringList listofFiles = QDir(assets + assetFolder).entryList(QDir::NoDotAndDotDot|QDir::Files);

        foreach(QString filename,listofFiles)
        {
            QString assetFullName = assetFolder + "/" + filename;
            if( bOverWrite && QFile::exists(assetFullName))
                QFile::remove(assetFullName);

            QFile::copy(assets+ assetFullName, assetFullName);
        }
    }
}

static void ExtractAsset(const QString filePath, bool bOverwrite=true)
{
    QString fullFilePath= QString("assets:/%0").arg(filePath);
    if(bOverwrite)  QFile::remove(filePath);
    QFile::copy( fullFilePath, filePath);
}
