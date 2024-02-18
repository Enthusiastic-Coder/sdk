#ifndef SKYDOME_H
#define SKYDOME_H

#include <QOpenGLBuffer>

class Renderer;

class SkyDome
{
public:
    struct properties
    {
        float radius;
        int sides;
        int slices;
        float hemisphereCount;
    } ;

    SkyDome(float radius, int sides, int slices, bool bFullSphere=false);

    void Render(Renderer* r);

private:
    QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    properties _prop;
};

#endif // SKYDOME_H
