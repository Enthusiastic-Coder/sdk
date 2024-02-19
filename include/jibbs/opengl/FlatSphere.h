#ifndef FLATSPHERE_H
#define FLATSPHERE_H

#include <jibbs/gps/GPSLocation.h>
#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/mesh/MeshData.h>

#include <QOpenGLTexture>
#include <memory>
#include "color3.h"

#include <QOpenGLBuffer>

class FlatSphere
{
public:
    void init();

    void clear();
    void addGPSPoint(const GPSLocation& gps, float x, float y, float cx, float cy);
    void updateIndices( int dimX, int dimY);

    void setTextureMinMag(QOpenGLTexture::Filter minification, QOpenGLTexture::Filter magnification);
    void setMod(float r, float g, float b, float a);
    void render();
    void setAspectRatio(float f);
    void setSea(float r, float g, float b, float a);
    void setGround(float r, float g, float b, float a);

protected:
    std::pair<float,float> toUV(const GPSLocation& loc);
    void addVertex(float x, float y, float cx, float cy);
    void addTexture(const GPSLocation& loc);
    void updateTexture();

private:
    OpenGLShaderProgram _shader;
    std::unique_ptr<QOpenGLTexture> _texID; //New Qt to load texture.
    meshData _vertices;
    std::unique_ptr<Renderer> _renderer;
    QOpenGLTexture::Filter _minification = QOpenGLTexture::Nearest;
    QOpenGLTexture::Filter _magnification = QOpenGLTexture::Nearest;
    float _aspect = 1.333f;
    color3 _mod;
    color3 _sea;
    color3 _ground;

    QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
};

#endif // FLATSPHERE_H
