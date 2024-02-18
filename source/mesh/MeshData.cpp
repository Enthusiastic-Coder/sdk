#include <jibbs/mesh/MeshData.h>
#include <QOpenGLBuffer>

void meshData::clear(int t)
{
    if( t == type::ALL)
    {
        _vertexData.clear();
        _normalData.clear();
        _texCoordData.clear();
        _indices.clear();
    }
    if( t & type::vertices )
        _vertexData.clear();

    if( t & type::texture )
        _texCoordData.clear();
}

void meshData::addVertex(const Vector2F &v)
{
	_vertexData.push_back(Vector3F(v.x, v.z, 0));
}

void meshData::addVertex(float x, float y)
{
    addVertex(Vector2F(x,y));
}

void meshData::addVertex(const Vector3F &v)
{
    _vertexData.push_back(v);
}

void meshData::addNormal(const Vector3F &v)
{
    _normalData.push_back(v);
}

void meshData::addVertex(float x, float y, float z)
{
    addVertex(Vector3F(x,y,z));
}

void meshData::addColor(float r, float g, float b, float a)
{
    addColor(Vector4F(r,g,b,a));
}

void meshData::addNormal(float x, float y, float z)
{
    addNormal(Vector3F(x,y,z));
}

void meshData::addColor(const Vector4F &v)
{
    _colorData.push_back(v);
}

void meshData::addTexture(float U, float V)
{
    _texCoordData.push_back(std::make_pair(U,V));
}

void meshData::addTexture2(float U, float V)
{
    _texCoord2Data.push_back(std::make_pair(U,V));
}

void meshData::addIndex(unsigned short v)
{
    _indices.push_back(v);
}

void meshData::setVertexPtr(float arr[], size_t bytes)
{
    _vertexData.resize(bytes/sizeof(_vertexData[0]));
    memcpy(&*_vertexData.begin(), arr, bytes);
}

void meshData::setColorPtr(float arr[], size_t bytes)
{
    _colorData.resize(bytes/sizeof(_colorData[0]));
    memcpy(&*_colorData.begin(), arr, bytes);
}

void meshData::setNormalPtr(float arr[], size_t bytes)
{
    _normalData.resize(bytes/sizeof(_normalData[0]));
    memcpy(&*_normalData.begin(), arr, bytes);
}

void meshData::setTexturePtr(float arr[], size_t bytes)
{
    _texCoordData.resize(bytes/sizeof(_texCoordData[0]));
    memcpy(&*_texCoordData.begin(), arr, bytes);
}

void meshData::setTexture2Ptr(float arr[], size_t bytes)
{
    _texCoord2Data.resize(bytes/sizeof(_texCoord2Data[0]));
    memcpy(&*_texCoord2Data.begin(), arr, bytes);
}

void meshData::setIndexPtr(unsigned short arr[], size_t bytes)
{
    _indices.resize(bytes/sizeof(_indices[0]));
    memcpy(&*_indices.begin(), arr, bytes);
}

const float *meshData::vertexPtr(size_t offset) const
{
    if( offset >= _vertexData.size())
        return 0;

    return &_vertexData[offset].x;
}

const float *meshData::colorPtr(size_t offset) const
{
    if( offset >= _colorData.size())
        return 0;

    return &_colorData[offset].x;
}

const float *meshData::normalPtr(size_t offset) const
{
    if( offset >= _normalData.size() )
        return 0;

    return &_normalData[offset].x;
}

const float *meshData::texturePtr(size_t offset) const
{
    if( offset >= _texCoordData.size() )
        return 0;

    return &_texCoordData[offset].first;
}

const float *meshData::texture2Ptr(size_t offset) const
{
    if( offset >= _texCoord2Data.size() )
        return 0;

    return &_texCoord2Data[offset].first;
}

const unsigned short *meshData::indexPtr(size_t offset) const
{
    if( offset >= _indices.size() )
        return 0;

    const auto it = _indices.begin() + offset;
    const auto* p = &(*it);
    return p;
}

int meshData::vertexBufferSize() const
{
    if( _vertexData.empty())
        return 0;

    return _vertexData.size() * sizeof(_vertexData[0]);
}

size_t meshData::vertexSize() const
{
    return _vertexData.size();
}

size_t meshData::colorSize() const
{
    return _colorData.size();
}

size_t meshData::normalSize() const
{
    return _normalData.size();
}

size_t meshData::textureSize() const
{
    return _texCoordData.size();
}

size_t meshData::indexSize() const
{
    return _indices.size();
}

void meshData::setVertex(size_t idx, const Vector3F& v)
{
    _vertexData[idx] = v;
}

void meshData::setColor(size_t idx, const Vector4F& v)
{
    _colorData[idx] = v;
}

void meshData::setNormal(size_t idx, const Vector3F& v)
{
    _normalData[idx] = v;
}

void meshData::setTexture(size_t idx, float U, float V)
{
    _texCoordData[idx] = std::make_pair(U,V);
}

void meshData::setTexture2(size_t idx, float U, float V)
{
    _texCoord2Data[idx] = std::make_pair(U,V);
}

void meshData::allocateVertex(QOpenGLBuffer &buffer)
{
    allocateT(_vertexData, buffer);
}

void meshData::allocateColor(QOpenGLBuffer &buffer)
{
    allocateT(_colorData, buffer);
}

void meshData::allocateNormal(QOpenGLBuffer &buffer)
{
    allocateT(_normalData, buffer);
}

void meshData::allocateTexture(QOpenGLBuffer &buffer)
{
    allocateT(_texCoordData, buffer);
}

void meshData::allocateIndex(QOpenGLBuffer &buffer)
{
    allocateT(_indices, buffer);
}

void meshData::bufferVertex(QOpenGLBuffer &buffer)
{
    bufferT(_vertexData, buffer);
}

void meshData::bufferTexture(QOpenGLBuffer &buffer)
{
    bufferT(_texCoordData, buffer);
}
