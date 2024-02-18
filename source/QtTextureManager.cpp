#include "QtTextureManager.h"
#include <QImage>
#include <QDebug>

QtTextureManager::QtTextureManager(QString name) :_name(name)
{
    qDebug() << Q_FUNC_INFO << ":" << _name;
}

QtTextureManager::~QtTextureManager()
{
    qDebug() << Q_FUNC_INFO << ":" << _name;
}

void QtTextureManager::init()
{
    QOpenGLFunctions::initializeOpenGLFunctions();
}

std::shared_ptr<QOpenGLTexture> QtTextureManager::peekTexture(const QString &filename)
{
    auto it = _textures.find(filename);
    if( it != _textures.end())
        return it.value().lock();

    return nullptr;
}

std::shared_ptr<QOpenGLTexture> QtTextureManager::loadTexture(const QString &filename, QOpenGLTexture::Filter minFilter, QOpenGLTexture::Filter maxFilter)
{
    auto texture = peekTexture(filename);

    if( texture)
        return texture;

    QImage img = QImage(filename);

    if( img.isNull())
        return nullptr;

    texture = std::make_shared<QOpenGLTexture>(img);
    texture->setMinMagFilters(minFilter, maxFilter);
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

    _textures[filename] = texture;

    return texture;
}
