#pragma once

#include <vector>
#include <jibbs/vector/vector2.h>
#include <jibbs/vector/vector3.h>
#include <jibbs/vector/vector4.h>

#include <QOpenGLBuffer>

class meshData
{
public:
    enum type {ALL, vertices=0x1, texture=0x2};

    void clear(int = ALL);

    void addVertex(const Vector2F& v);
    void addVertex(float x, float y);

    void addVertex(const Vector3F& v);
    void addVertex(float x, float y, float z);

    void addNormal(const Vector3F& v);
    void addNormal(float x, float y, float z);

    void addColor(const Vector4F& v);
    void addColor(float r, float g, float b, float a);

    void addTexture(float U, float V);
    void addTexture2(float U, float V);
    void addIndex(unsigned short v);

    void setVertex(size_t idx, const Vector3F &v);
    void setColor(size_t idx, const Vector4F &v);
    void setNormal(size_t idx, const Vector3F &v);
    void setTexture(size_t ix, float U, float V);
    void setTexture2(size_t ix, float U, float V);

    void setVertexPtr(float[], size_t);
    void setColorPtr(float[], size_t);
    void setNormalPtr(float[], size_t);
    void setTexturePtr(float[], size_t);
    void setTexture2Ptr(float[], size_t);
    void setIndexPtr(unsigned short[], size_t);

    const float* vertexPtr(size_t offset=0) const;
    const float* colorPtr(size_t offset=0) const;
    const float* normalPtr(size_t offset=0) const;
    const float* texturePtr(size_t offset=0) const;
    const float* texture2Ptr(size_t offset=0) const;
    const unsigned short *indexPtr(size_t offset=0) const;

    int vertexBufferSize() const;

    size_t vertexSize() const;
    size_t colorSize() const;
    size_t normalSize() const;
    size_t textureSize() const;
    size_t indexSize() const;

    void allocateVertex(QOpenGLBuffer &buffer);
    void allocateColor(QOpenGLBuffer &buffer);
    void allocateNormal(QOpenGLBuffer &buffer);
    void allocateTexture(QOpenGLBuffer &buffer);
    void allocateIndex(QOpenGLBuffer &buffer);

    void bufferVertex(QOpenGLBuffer &buffer);
    void bufferTexture(QOpenGLBuffer &buffer);

private:
    template<class T> void allocateT(T& vec, QOpenGLBuffer &buffer)
    {
        buffer.bind();
        buffer.allocate(vec.data(), sizeof(vec[0]) * vec.size());
        buffer.release();
    }

    template<class T> void bufferT(T& vec, QOpenGLBuffer &buffer)
    {
        buffer.bind();
        buffer.write(0, vec.data(), sizeof(vec[0]) * vec.size());
        buffer.release();
    }

private:
    std::vector<Vector3F> _vertexData;
    std::vector<Vector4F> _colorData;
    std::vector<Vector3F> _normalData;
    std::vector<std::pair<float, float>> _texCoordData;
    std::vector<std::pair<float, float>> _texCoord2Data;
    std::vector<unsigned short> _indices;
};

