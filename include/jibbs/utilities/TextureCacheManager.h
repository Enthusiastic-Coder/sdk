#ifndef TEXTURECACHEMANAGER_H
#define TEXTURECACHEMANAGER_H

#include <unordered_map>
#include <memory>

#include <QString>
#include <QOpenGLTexture>

class TextureCacheManager
{
public:
    TextureCacheManager();

    std::shared_ptr<QOpenGLTexture> getTexture(const QImage &img);

    void cleanupExpiredTextures();

private:
    std::unordered_map<QString, std::weak_ptr<QOpenGLTexture>> _textureMap;
};

#endif // TEXTURECACHEMANAGER_H
