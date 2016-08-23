#include <QDir>
#include <QString>
#include <QStringList>

static void copyAssets(bool bOverWrite)
{
#ifdef IS_QT_ANDROID
    QDir curDir(QDir::current());
    QString assets("assets:/");

    QStringList assetFolderList = QDir(assets).entryList(QDir::NoDotAndDotDot|QDir::Dirs);

    foreach(QString assetFolder, assetFolderList)
    {
        if( assetFolder.startsWith("--"))
            continue;

        bool b3 = curDir.mkdir(assetFolder);

        QStringList listofFiles = QDir(assets + assetFolder).entryList(QDir::NoDotAndDotDot|QDir::Files);

        foreach(QString filename,listofFiles)
        {
            QString assetFullName = assetFolder + "/" + filename;
            if( bOverWrite && QFile::exists(assetFullName))
                QFile::remove(assetFullName);

            QFile::copy(assets+ assetFullName, assetFullName);
        }
    }
#endif
}

