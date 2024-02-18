#include <jibbs/mesh/AssimpMeshManager.h>
#include <QDebug>
#include <QMutexLocker>

AssimpMeshManager::AssimpMeshManager()
{
#ifdef Q_OS_WIN
      qDebug() << Q_FUNC_INFO;
#endif
}

AssimpMeshManager::~AssimpMeshManager()
{
#ifdef Q_OS_WIN
      qDebug() << Q_FUNC_INFO;
#endif
}

QMutex *AssimpMeshManager::mutex()
{
    return &_mutex;
}

std::shared_ptr<sceneLoader> AssimpMeshManager::getSceneIfExist(const QString &filename)
{
    return _scenes[filename].lock();
}

std::shared_ptr<sceneLoader> AssimpMeshManager::getScene(const QString &filename)
{
    auto scene = getSceneIfExist(filename);

    if( scene != nullptr)
        return scene;

    scene.reset(new sceneLoader(this));

#ifdef Q_OS_WIN
    qDebug() << "MeshManager Loaded :" <<filename;
#endif

    _scenes[filename] = scene;
    return scene;
}
