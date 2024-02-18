#ifndef SKYBOX_H
#define SKYBOX_H

#include <jibbs/opengl/QtTextureManager.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/opengl/OpenGLPipeline.h>
#include <jibbs/opengl/SkyDome.h>

#include <QHash>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include <memory>

class Renderer;

class SkyBox : private QOpenGLFunctions
{
private:
    struct TexturesInfo
    {
        bool isDay = true;
        bool isCube = true;
        QRgb brightness;
        QString id;
        QString filenames[6];
        std::shared_ptr<QOpenGLTexture> ids[6];
    };

public:
    SkyBox();

    void init();
    void addCube(const QString &folder, bool isDay, QRgb brightness);
    void addDome(const QString &name, const QString& filename, bool isDay, QRgb brightness);
    bool selectId(const QString& folder);

    int rBrightness() const;
    int gBrightness() const;
    int bBrightness() const;
    bool isDayTime() const;
    void Render(Renderer *r, OpenGLPipeline &pipe);

    const QStringList& getIds() const;

protected:
    void initCube();
    void renderCube(Renderer *r, const TexturesInfo &t);

private:
    std::shared_ptr<QOpenGLTexture> loadTexture( const QString& file );

private:
    QStringList _Ids;
    std::unique_ptr<SkyDome> _skyDome;
    OpenGLShaderProgram _skyLineProgram;

    QtTextureManager _texManager;
    QOpenGLBuffer _vertexBuffer  = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QHash<QString,TexturesInfo> _cubes;
    TexturesInfo _current;

private:
    void loadCubeTextures(TexturesInfo& t);
    void unLoadCubeTextures(TexturesInfo& t);
};

#endif // SKYBOX_H
