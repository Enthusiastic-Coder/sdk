#ifndef QTTEXTUREMANAGER_H
#define QTTEXTUREMANAGER_H

#include <QHash>
#include <QOpenGLFunctions>
#include <memory>
#include <QOpenGLTexture>

class QtTextureManager : private QOpenGLFunctions
{
public:
    explicit QtTextureManager(QString name);
    ~QtTextureManager();

    void init();
    std::shared_ptr<QOpenGLTexture> peekTexture(const QString& filename);
    std::shared_ptr<QOpenGLTexture> loadTexture(const QString& filename, QOpenGLTexture::Filter minFilter, QOpenGLTexture::Filter maxFilter);

private:
    QString _name;
    QHash<QString,std::weak_ptr<QOpenGLTexture>> _textures;
};

#endif // QTTEXTUREMANAGER_H
