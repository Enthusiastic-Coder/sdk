#include <jibbs/utilities/stdafx.h>
#include <jibbs/opengl/OpenGLPipeline.h>
#include <jibbs/maptiles/FlatSphere.h>

#include <QString>
#include <QDebug>


void FlatSphere::init()
{
    _renderer.reset(new Renderer);

    if( !_shader.loadSrc(readAllRP("shaders/texFlatShader.vert"), readAllRP("shaders/texFlatShader.frag")))
    {
        qDebug() << QString::fromStdString(_shader.getError());
    }

    _texID.reset( new QOpenGLTexture(QImage(ResourcePath("images/earthmap.jpg")).mirrored()));
    updateTexture();

    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    _indexBuffer.create();
    _indexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

void FlatSphere::clear()
{
    _vertices.clear(meshData::vertices|meshData::texture);
}

void FlatSphere::addGPSPoint(const GPSLocation& gps, float x, float y, float cx, float cy)
{
    addVertex(x, y, cx, cy);
    addTexture(gps);
}

void FlatSphere::updateIndices(int dimX, int dimY)
{
    _vertexBuffer.bind();

    if( _vertexBuffer.size() < _vertices.vertexBufferSize())
    {
        _vertices.allocateVertex(_vertexBuffer);
        _vertices.allocateTexture(_textureBuffer);
    }
    else
    {
        _vertices.bufferVertex(_vertexBuffer);
        _vertices.bufferTexture(_textureBuffer);
    }

    if( _vertices.indexSize() != 0)
        return;

#ifdef Q_OS_WIN
    qDebug() << "flatSphere - index built";
#endif

    for( int y =0; y < dimY-1; ++y )
    {
        for(int x=0; x < dimX-1; ++x)
        {
            _vertices.addIndex(x + dimX*y);
            _vertices.addIndex(x+1 + dimX*y);
            _vertices.addIndex(x + dimX*(y+1));

            _vertices.addIndex(x+1 + dimX*y);
            _vertices.addIndex(x+1 + dimX*(y+1));
            _vertices.addIndex(x + dimX*(y+1));
        }
    }

    _vertices.allocateIndex(_indexBuffer);
}

void FlatSphere::setTextureMinMag(QOpenGLTexture::Filter minification, QOpenGLTexture::Filter magnification)
{
    _minification = minification;
    _magnification = magnification;
    updateTexture();
}

std::pair<float, float> FlatSphere::toUV(const GPSLocation &loc)
{
    return std::make_pair( (loc._lng + 180)/360.0f, (loc._lat+90)/180.0f);
}

void FlatSphere::addVertex(float x, float y, float cx, float cy)
{
    _vertices.addVertex(x/cx * _aspect, y/cy, 0);
}

void FlatSphere::addTexture(const GPSLocation &loc)
{
    const auto& uv = toUV(loc);
    _vertices.addTexture(uv.first, uv.second);
}

void FlatSphere::updateTexture()
{
    _texID->setMinMagFilters(_minification, _magnification);
}

void FlatSphere::render()
{
    if( _vertices.indexSize() == 0)
        return;

    _renderer->camID = 1;
    _renderer->useProgram( _shader);

    OpenGLPipeline& pipeline = OpenGLPipeline::Get(_renderer->camID);
    pipeline.Push();
    pipeline.GetModel().LoadIdentity();
    pipeline.GetView().LoadIdentity();
    pipeline.GetProjection().LoadIdentity();
    pipeline.GetProjection().SetOrthographic(0, _aspect, 0, 1, -1, 1);

    pipeline.bindMatrices(_renderer->progId());
    _renderer->progId().sendUniform("Color", _mod.r, _mod.g, _mod.b, _mod.a);
    _renderer->progId().sendUniform("texture0", 0);

    _renderer->progId().sendUniform("sea", _sea.r, _sea.g, _sea.b, _sea.a);
    _renderer->progId().sendUniform("ground", _ground.r, _ground.g, _ground.b, _ground.a);

    _vertexBuffer.bind();
    _renderer->bindVertex(Renderer::Vertex, 3);

    _textureBuffer.bind();
    _renderer->bindVertex(Renderer::TexCoord, 2);

    _renderer->bindTexture(_texID->textureId());
#ifdef WIN32___
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    _indexBuffer.bind();
    _renderer->drawElements(GL_TRIANGLES, _vertices.indexSize());
#ifdef WIN32___
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    _renderer->unBindBuffers();
    _texID->release();

    pipeline.Pop();
}

void FlatSphere::setAspectRatio(float f)
{
    _aspect = f;
}

void FlatSphere::setMod(float r, float g, float b, float a)
{
    _mod.r = r;
    _mod.g = g;
    _mod.b = b;
    _mod.a = a;
}

void FlatSphere::setSea(float r, float g, float b, float a)
{
    _sea.r = r;
    _sea.g = g;
    _sea.b = b;
    _sea.a = a;
}

void FlatSphere::setGround(float r, float g, float b, float a)
{
    _ground.r = r;
    _ground.g = g;
    _ground.b = b;
    _ground.a = a;
}
