#include <jibbs/utilities/stdafx.h>
#include <jibbs/math/MeshHelper.h>
#include <jibbs/mesh/meshloader.h>
#include <jibbs/opengl/QtTextureManager.h>
#include <jibbs/mesh/AssimpMeshManager.h>
#include <jibbs/mesh/vertexData.h>

#include <stdlib.h>

#include <QImage>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QOpenGLFunctions>
#include <QObject>
#include <QMutexLocker>


meshLoader::meshLoader(std::shared_ptr<QtTextureManager> t, std::shared_ptr<AssimpMeshManager> meshManager) :
    _texManager(t), _meshManager(meshManager)
{

}

meshLoader::~meshLoader()
{
#ifdef Q_OS_WIN
    qDebug() << "Mesh Unloaded: " << _filename << ":" << _skin;
#endif
}

void meshLoader::setMinMaxFilters(QOpenGLTexture::Filter minFilter, QOpenGLTexture::Filter maxFilter)
{
    _minFilter = minFilter;
    _maxFilter = maxFilter;
}

void meshLoader::setExtractorHooks(std::function<void ()> extractLambda, std::function<void()> unextractorLambda)
{
    _extractor = extractLambda;
    _unextractor = unextractorLambda;
}

void meshLoader::load(const QString &filename, const QString &skin)
{
    _filename= filename;
    _skin = skin;

    int idx = filename.lastIndexOf("/");

    if( idx == -1)
        idx = filename.lastIndexOf("\\");

    if( idx != -1)
        _baseFolder = filename.mid(0, idx );

    _scene = _meshManager->getSceneIfExist(filename);

    if( _scene)
    {
        _scene->asyncIsLoaded([this](bool loaded) {
            if( loaded)
                prepareSkin();
            else
                QObject::connect(_scene.get(), &sceneLoader::onLoaded, this, &meshLoader::prepareSkin);
        });
    }
    else
    {
        _scene = _meshManager->getScene(filename);

        QObject::connect(_scene.get(), &sceneLoader::onLoaded, this, &meshLoader::prepareSkin);
        QObject::connect(_scene.get(), &sceneLoader::onLoaded, this, &meshLoader::onAttachPivots);

        _scene->setExtractorHooks(_extractor, _unextractor);
        _scene->load(filename);
    }
}

void meshLoader::prepareSkin()
{
    if (_scene == nullptr)
    {
#ifdef Q_OS_WIN
        qDebug() << "The file wasn't successfuly opened " << _filename;
        qDebug() << "Error : " << aiGetErrorString();
#endif
        emit onError();
        return;
    }

    _openglRealiser = [this] {realiseOpenglTextures();};
}

void meshLoader::realiseOpenglTextures()
{
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    if( ctx == nullptr)
        return;

    _openglRealiser = nullptr;

    auto& meshes = _scene->getMeshes();

    _meshes.insert(_meshes.end(), meshes.begin(), meshes.end());

    _CG = _scene->getCG();
    _BoundingBox = _scene->getBoundingBox();

    for( size_t i =0;i < meshes.size(); ++i)
    {
        meshes[i]->finalise();

        if( !_texManager )
            continue;

        const QString texFilename = meshes[i]->texFilename();

        if( texFilename.isEmpty())
            continue;

        QString actualTexFilename;

        if( _skin.length())
        {
            QString filename = QString("%1/%2/%3").arg(_baseFolder, _skin, texFilename);

            if( QFile::exists(filename))
                actualTexFilename = filename;
        }

        if( actualTexFilename.isEmpty())
        {
            QString filename = QString("%1/%2").arg(_baseFolder, texFilename);

            if( QFile::exists(filename))
                actualTexFilename = filename;
        }

        if( actualTexFilename.isEmpty())
        {
            QString filename = QString("%1/%2").arg(_globalFolder, texFilename);

            if( QFile::exists(filename))
                actualTexFilename = filename;
        }

        if( actualTexFilename.isEmpty())
        {
#ifdef Q_OS_WIN
            qDebug() << "ERROR Failed to load folder : " << _skin << "->" << texFilename;
#endif
            continue;
        }

        std::shared_ptr<QOpenGLTexture> tex;

        auto it = _texturesUsed.find(texFilename);

        if(it != _texturesUsed.end())
        {
            tex = it->second;
        }
        else
        {
            tex = _texManager->loadTexture(actualTexFilename, _minFilter, _maxFilter);
            _texturesUsed[texFilename] = tex;
        }

        if( tex )
        {
            _meshes[i].setTex(tex->textureId());
        }
    }
}

void meshLoader:: draw(OpenGLShaderProgram &program) const
{
    if( _openglRealiser )
        _openglRealiser();

    for( auto& mesh : _meshes)
        mesh.draw(program);
}

const std::vector<subMesh> &meshLoader::getMeshes() const
{
    if( _openglRealiser )
        _openglRealiser();

    return _meshes;
}

void meshLoader::addPivot(const QString &name, std::function<void (OpenGLMatrixStack &, pivotData&)> handler)
{
    _scene->addPivot(name, handler);
}

bool meshLoader::InFrustum(const Frustum &f) const
{
    if( _openglRealiser )
        _openglRealiser();

    return _BoundingBox.InFrustum(f);
}

float meshLoader::getCG() const
{
    if( _openglRealiser )
        _openglRealiser();

    return _CG;
}

const BoundingBox &meshLoader::getBoundingBox() const
{
    if( _openglRealiser )
        _openglRealiser();

    return _BoundingBox;
}
