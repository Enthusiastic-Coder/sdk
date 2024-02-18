#pragma once

#include <jibbs/mesh/MeshData.h>
#include <QString>
#include <QOpenGLBuffer>

class OpenGLFontTexture;
class Renderer;

class OpenGLFontMeshBuilder
{
public:
    OpenGLFontMeshBuilder();

    void setFont(OpenGLFontTexture* texture);
    OpenGLFontTexture* getFontTexture();

    int getWidth(const QString& str) const;

    void resetIndex();
    void clear();
    void add(int x, int y, const QString &str, bool bHorizontal=true);
    void add( int x, int y, const std::vector<QString>& lines);
    void add(int x, int y, int dx, int dy, unsigned char ch);

    void beginRender(Renderer* r);
    void render(Renderer* r);
    void endRender(Renderer* r);

private:
    void updateBuffers();

private:
    OpenGLFontTexture* _fontTexture = nullptr;
    meshData _meshData;
    QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    size_t _curIndex = 0;
};
