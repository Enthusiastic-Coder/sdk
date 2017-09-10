#include <QDir>
#include <QString>
#include <QStringList>
#include <QDirIterator>

static void copyFileAsset(bool bOverWrite, const QFileInfo& fileInfo)
{
    QString assets("assets:/");
    QString assetFolder = fileInfo.absolutePath();
    assetFolder.remove(0, 8);
    QDir curDir(QDir::current());

    bool b3 = curDir.mkpath(assetFolder);

    QString assetFullName = assetFolder + "/" + fileInfo.fileName();
    if( bOverWrite && QFile::exists(assetFullName))
    {
        qDebug() << "remove :" << assetFullName;
        QFile::remove(assetFullName);
    }

    bool bCopyOk = QFile::copy(assets+ assetFullName, assetFullName);
    qDebug() << "copy : " << (assets+assetFullName) << " -to- " << assetFullName << " : " << bCopyOk;
}

static QString rootFolder = "assets:/";

static void copyAssets(bool bOverWrite, QStringList ignoreFolderList, QString rootdirName=rootFolder)
{
    if( rootdirName == rootFolder)
    {
        qDebug() << "begin: copyAssets ";
        qDebug() << rootdirName;
        qDebug() << bOverWrite;
        qDebug() << ignoreFolderList;
    }
    else
    {
        qDebug() << "Folder:" << rootdirName;
    }

    QDirIterator it(rootdirName, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString dirFilename = it.next();

        if( it.fileInfo().isDir())
        {
            if( dirFilename.contains("androiddeployqt"))
                continue;

            bool bIgnore(false);
            for(const QString& ignoreFolder:ignoreFolderList)
                if( dirFilename == ignoreFolder)
                {
                    bIgnore = true;
                    break;
                }

            if( bIgnore)
            {
                qDebug() << "Ignored : " << dirFilename;
                continue;
            }

            copyAssets(bOverWrite, ignoreFolderList, dirFilename);
        }
        else
        {
            copyFileAsset(bOverWrite, it.fileInfo());
        }
    }

    if(rootdirName == rootFolder)
    {
        qDebug() << "end: copyAssets ";
    }
}

static void ExtractAsset(const QString filePath, bool bOverwrite=true)
{
#ifndef ANDROID
    return;
#endif
    QString fullFilePath= QString("assets:/%0").arg(filePath);
    if(bOverwrite)  QFile::remove(filePath);
    QFileInfo fi(filePath);
    QDir dir(QDir::currentPath());
    dir.mkpath(fi.absolutePath());
    QFile::copy( fullFilePath, filePath);
}
