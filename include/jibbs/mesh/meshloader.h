#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <jibbs/boundary/BoundingBox.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/mesh/SceneLoader.h>
#include <jibbs/opengl/QtTextureManager.h>

#include <vector>
#include <QString>
#include <vector>

#include <functional>
#include "stdafx.h"

class QtTextureManager;
class AssimpMeshManager;

class subMesh
{
public:
    subMesh(std::shared_ptr<SubScene> p):_p(p) {}
    SubScene* operator->() { return _p.get();}
    const SubScene* operator->()const { return _p.get();}
    const SubScene* get() const { return _p.get();}
    void draw(OpenGLShaderProgram &program) const
    {
        _p->draw(program, _idx);
    }

    void setTex(GLuint idx) { _idx = idx; }
private:
    std::shared_ptr<SubScene> _p;
    GLuint _idx = -1;
};

class meshLoader : public QObject
{
    Q_OBJECT
public:
    meshLoader(std::shared_ptr<QtTextureManager> t, std::shared_ptr<AssimpMeshManager> meshManager);
    ~meshLoader();

    void setMinMaxFilters(QOpenGLTexture::Filter minFilter, QOpenGLTexture::Filter maxFilter);
    void setExtractorHooks(std::function<void()> extractLambda, std::function<void()> unextractorLambda);
    void load(const QString &filename, const QString& skin="");
    void draw(OpenGLShaderProgram& programId) const;
    const std::vector<subMesh> &getMeshes() const;
    void addPivot(const QString& name, std::function<void(OpenGLMatrixStack &, pivotData&)> handler);

    bool InFrustum(const Frustum& f) const;
    float getCG() const;
    const BoundingBox& getBoundingBox() const;

private slots:
    void prepareSkin();

signals:
    void onAttachPivots();
    void onError();

private:
    void realiseOpenglTextures();

private:
    const QString _globalFolder = ResourcePath("models/globalTextures");
    QString _baseFolder;
    QString _filename;
    QString _skin;

    float _CG = 0.0f;
    BoundingBox _BoundingBox;

    QOpenGLTexture::Filter _minFilter = QOpenGLTexture::LinearMipMapNearest;
    QOpenGLTexture::Filter _maxFilter = QOpenGLTexture::Linear;

    std::function<void()> _openglRealiser = nullptr;
    std::function<void()> _extractor = nullptr;
    std::function<void()> _unextractor = nullptr;

    std::shared_ptr<sceneLoader> _scene;
    std::shared_ptr<QtTextureManager> _texManager;
    std::shared_ptr<AssimpMeshManager> _meshManager;
    std::unordered_map<QString,std::shared_ptr<QOpenGLTexture>> _texturesUsed;

    std::vector<subMesh> _meshes;
};

#endif // MESHLOADER_H
