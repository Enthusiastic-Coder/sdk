#include <jibbs/utilities/TextureCacheManager.h>

#include <QImage>
#include <QCryptographicHash>
#include <QDebug>

namespace {
    QString calculateImageHash(const QImage& image) {
    // Convert QImage to QByteArray
    QByteArray imageData(reinterpret_cast<const char*>(image.bits()), image.sizeInBytes());

    // Calculate SHA-256 hash
    QByteArray hash = QCryptographicHash::hash(imageData, QCryptographicHash::Sha256);

    // Convert hash to a hexadecimal string
    return hash.toHex();
}
}

TextureCacheManager::TextureCacheManager() {}

std::shared_ptr<QOpenGLTexture> TextureCacheManager::getTexture(const QImage& img)
{
    QString imageHash = calculateImageHash(img);
    auto it = _textureMap.find(imageHash);

    auto genTexture = [this,imageHash,img] {
        std::shared_ptr<QOpenGLTexture> tex = std::make_shared<QOpenGLTexture>(img);
        _textureMap[imageHash] = tex;
        return tex;
    };

    if( it == _textureMap.end())
    {
        return genTexture();
    }
    else
    {
        std::shared_ptr<QOpenGLTexture> tex = it->second.lock();

        if( tex)
            return tex;

        return genTexture();
    }
}
void TextureCacheManager::cleanupExpiredTextures()
{
    for (auto it = _textureMap.begin(); it != _textureMap.end();)
    {
        if (it->second.expired())
        {
            // Remove the entry from the map if the weak pointer is expired
            it = _textureMap.erase(it);
        } else
        {
            ++it;
        }
    }
#ifdef Q_OS_WINDOWS
    qDebug() << Q_FUNC_INFO << ":" << _textureMap.size();
#endif
}
