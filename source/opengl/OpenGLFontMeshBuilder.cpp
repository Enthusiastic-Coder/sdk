#include <jibbs/utilities/stdafx.h>
#include <jibbs/opengl/OpenGLFontMeshBuilder.h>
#include <jibbs/opengl/OpenGLFontTexture.h>
#include <jibbs/opengl/OpenGLRenderer.h>


OpenGLFontMeshBuilder::OpenGLFontMeshBuilder() :
    _fontTexture(nullptr)
{
    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

void OpenGLFontMeshBuilder::clear()
{
    _meshData.clear(meshData::vertices|meshData::texture);
    _curIndex = 0;
}

void OpenGLFontMeshBuilder::resetIndex()
{
    _curIndex = 0;
}

void OpenGLFontMeshBuilder::add(int x, int y, const QString& str, bool bHorizontal)
{
    const int len = str.length();
    for( int i =0; i < len; ++i)
    {
        MathSupport<int>::size sz = _fontTexture->getFontSize((unsigned char)str.at(i).toLatin1());
        add( x, y, sz.width, sz.height, (unsigned char)str.at(i).toLatin1());

        if( bHorizontal)
            x += sz.width;
        else
            y += sz.height;
    }

    updateBuffers();
}

void OpenGLFontMeshBuilder::add(int x, int y, const std::vector<QString> &lines)
{
    const int x0 = x;
    for( auto it = lines.rbegin(); it != lines.rend(); ++it)
    {
        const auto& str = *it;
        const int len = str.length();
        int height = 0;
        for( int i =0; i < len; ++i)
        {
            MathSupport<int>::size sz = _fontTexture->getFontSize((unsigned char)str.at(i).toLatin1());
            add( x, y, sz.width, sz.height, (unsigned char)str.at(i).toLatin1());

            x += sz.width;
            height = sz.height;
        }
        x = x0;
        y -= height;
    }

    updateBuffers();
}

void OpenGLFontMeshBuilder::updateBuffers()
{
    _vertexBuffer.bind();
    if( _vertexBuffer.size() < _meshData.vertexBufferSize())
    {
        _meshData.allocateVertex(_vertexBuffer);
        _meshData.allocateTexture(_textureBuffer);
    }
    else
    {
        _meshData.bufferVertex(_vertexBuffer);
        _meshData.bufferTexture(_textureBuffer);
    }
}


void OpenGLFontMeshBuilder::add(int x, int y, int dx, int dy, unsigned char ch)
{
    std::pair<float,float> uvs[4];
    _fontTexture->getUV( ch, uvs);

    //add **TWO** triangles to make QUAD


    if( _meshData.vertexSize() <= _curIndex )
    {
        //Triangle A
        _meshData.addVertex(Vector3F(x,y,0));
        _meshData.addTexture(uvs[0].first, uvs[0].second);

        _meshData.addVertex(Vector3F(x+dx,y,0));
        _meshData.addTexture(uvs[1].first, uvs[1].second);

        _meshData.addVertex(Vector3F(x,y-dy,0));
        _meshData.addTexture(uvs[3].first, uvs[3].second);

        //Triangle B

        _meshData.addVertex(Vector3F(x+dx,y,0));
        _meshData.addTexture(uvs[1].first, uvs[1].second);

        _meshData.addVertex(Vector3F(x+dx,y-dy,0));
        _meshData.addTexture(uvs[2].first, uvs[2].second);

        _meshData.addVertex(Vector3F(x,y-dy,0));
        _meshData.addTexture(uvs[3].first, uvs[3].second);

        _curIndex += 6;
    }
    else
    {
        //Triangle A
        _meshData.setVertex(_curIndex, Vector3F(x,y,0));
        _meshData.setTexture(_curIndex, uvs[0].first, uvs[0].second);

        _curIndex++;

        _meshData.setVertex(_curIndex, Vector3F(x+dx,y,0));
        _meshData.setTexture(_curIndex, uvs[1].first, uvs[1].second);

        _curIndex++;

        _meshData.setVertex(_curIndex, Vector3F(x,y-dy,0));
        _meshData.setTexture(_curIndex, uvs[3].first, uvs[3].second);

        _curIndex++;
        //Triangle B

        _meshData.setVertex(_curIndex, Vector3F(x+dx,y,0));
        _meshData.setTexture(_curIndex, uvs[1].first, uvs[1].second);

        _curIndex++;

        _meshData.setVertex(_curIndex, Vector3F(x+dx,y-dy,0));
        _meshData.setTexture(_curIndex, uvs[2].first, uvs[2].second);

        _curIndex++;
        _meshData.setVertex(_curIndex, Vector3F(x,y-dy,0));
        _meshData.setTexture(_curIndex, uvs[3].first, uvs[3].second);

        _curIndex++;
    }
}

void OpenGLFontMeshBuilder::setFont(OpenGLFontTexture *texture)
{
    _fontTexture = texture;
}

void OpenGLFontMeshBuilder::beginRender(Renderer *r)
{
    if( _fontTexture != nullptr)
        r->bindTexture(_fontTexture->texture()->textureId());
}

void OpenGLFontMeshBuilder::render(Renderer *r)
{
    _vertexBuffer.bind();
    r->bindVertex(Renderer::Vertex, 3);

    _textureBuffer.bind();
    r->bindVertex(Renderer::TexCoord, 2);

    r->drawArrays(GL_TRIANGLES, _curIndex);
    r->unBindBuffers();
}

void OpenGLFontMeshBuilder::endRender(Renderer *r)
{
    _fontTexture->texture()->release();
}

OpenGLFontTexture *OpenGLFontMeshBuilder::getFontTexture()
{
    return _fontTexture;
}

int OpenGLFontMeshBuilder::getWidth(const QString &str) const
{
    return _fontTexture->getWidth(str);
}

