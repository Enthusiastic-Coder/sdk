#ifndef ASSIMPMESHMANAGER_H
#define ASSIMPMESHMANAGER_H

#include <jibbs/mesh/SceneLoader.h>

#include <unordered_map>
#include <QString>

#include <QMutex>
#include <QFuture>
#include <QFutureWatcher>

class AssimpMeshManager
{
public:
    explicit AssimpMeshManager();
    ~AssimpMeshManager();
    QMutex* mutex();

    std::shared_ptr<sceneLoader> getSceneIfExist(const QString& filename);
    std::shared_ptr<sceneLoader> getScene(const QString& filename);

private:
    std::unordered_map<QString,std::weak_ptr<sceneLoader>> _scenes;
    QMutex _mutex;
};

#endif // ASSIMPMESHMANAGER_H
