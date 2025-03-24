#ifndef WORLDTILERENDERER_H
#define WORLDTILERENDERER_H

#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/opengl/QtTextureManager.h>

#include <QColor>
#include <QPoint>
#include <QObject>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include <unordered_map>
#include <memory>

class RadarView;

class WorldTileRenderer : public QObject, private QOpenGLFunctions
{
    Q_OBJECT
public:
    struct Tile
    {
        Tile() {
            _vertexBuffer.create();
            _vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            _vertexBuffer.bind();
            _vertexBuffer.allocate(vertices, sizeof(vertices));
            _vertexBuffer.release();
        }
        bool bRequired=false;
        std::shared_ptr<QOpenGLTexture> tex;
        QPointF tL;
        QPointF bR;
        float vertices[12] = {};

        void updateBuffer() {
            _vertexBuffer.bind();
            _vertexBuffer.write(0, vertices, sizeof(vertices));
            _vertexBuffer.release();
        }

        QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    };

    explicit WorldTileRenderer(QObject* parent=nullptr);

    void init(std::shared_ptr<QtTextureManager> texManager);

    void setDimensions(float width, float height);
    void setGPSOrigin(GPSLocation location);
    void setGPSToScreenCallBack(std::function<QPointF(GPSLocation)> callBack);
    void setEnabled(bool enabled);

    void releaseAllTextures();
    void render(float compassValue, QPointF mid);
    void render(float compassValue);
    void setSea(QColor c);
    void setGround(QColor c);
    void BuildGroundTile(int tileWidth, int tileHeight);

private:
    bool _enabled = true;
    GPSLocation  _getGPSOrigin;
    float _width = 0;
    float _height = 0;

    std::function<QPointF(GPSLocation)> _toScreenCallBack = [](GPSLocation){return QPointF();};
    std::shared_ptr<QtTextureManager> _texManager;

    OpenGLShaderProgram _shader;
    QTimer* _timer =nullptr;

    std::unique_ptr<Renderer> _renderer;
    QOpenGLTexture::Filter _minification = QOpenGLTexture::LinearMipMapNearest;
    QOpenGLTexture::Filter _magnification = QOpenGLTexture::Nearest;
    QColor _sea = Qt::blue;
    QColor _ground = Qt::green;

    std::unordered_map<QString, Tile> _textures;

    QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
};

#endif // WORLDTILERENDERER_H
