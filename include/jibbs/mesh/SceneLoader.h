#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <jibbs/boundary/BoundingBox.h>
#include <vector>
#include <functional>

#include <QString>
#include <QFuture>
#include <QFutureWatcher>
#include <OpenGLShaderProgram.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SubScene.h"

class AssimpMeshManager;

class sceneLoader : public QObject
{
    Q_OBJECT
public:
    sceneLoader(AssimpMeshManager* manager);
    ~sceneLoader();

    void setExtractorHooks(std::function<void()> extractLambda, std::function<void()> unextractorLambda);
    void load(const QString &filename);
    const std::vector<std::shared_ptr<SubScene> > &getMeshes() const;
    void addPivot(const QString& name, std::function<void(OpenGLMatrixStack &, pivotData&)> handler);

    void asyncIsLoaded(std::function<void (bool)> callback);
    bool InFrustum(const Frustum& f) const;
    float getCG() const;
    const BoundingBox& getBoundingBox() const;

signals:
    void onLoaded();
    void onError();

protected:

private:
    QString _filename;
    std::function<void()> _extractor = nullptr;
    std::function<void()> _unextractor = nullptr;
    bool _isLoaded = false;
    std::vector<std::shared_ptr<SubScene>> _meshes;
    void recursiveProcess(aiNode* node, const aiScene* _scene, Vector3F &minV, Vector3F &maxV);
    BoundingBox processMesh(aiString name, aiMesh* mesh, const aiScene* _scene);
    BoundingBox _BoundingBox;
    float _CG = 0.0f;

    QMutex _isLoadedMutex;
    QFuture<void> _loadFuture;
    QFutureWatcher<void>* _watcher = new QFutureWatcher<void>(this);
    AssimpMeshManager* _manager;
};

#endif // MESHLOADER_H
