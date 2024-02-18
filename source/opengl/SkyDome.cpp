#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/mesh/MeshData.h>
#include <jibbs/opengl/SkyDome.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

SkyDome::SkyDome(float radius, int sides, int slices, bool bFullSphere)
{
    _prop.radius = radius;
    _prop.sides = sides;
    _prop.slices = slices;
    _prop.hemisphereCount = bFullSphere ? 2 : 1;

    float fPolyAng = 2.0f * M_PI / sides;
    Vector3F v;

    meshData data;

    for( int y=0; y <= _prop.hemisphereCount*slices; ++y)
    {
        float fAng = (y-slices) * M_PI / 2 / slices;

        for( int x =0; x <= sides; ++x)
        {
            v.x = cos(x*fPolyAng) * cos(fAng);
            v.y = sin(fAng);
            v.z = sin(x* fPolyAng) * cos(fAng);

            data.addVertex(v * radius);
            data.addTexture(float(x)/sides, 1.0f-float(y)/slices/_prop.hemisphereCount);
            data.addIndex(y * (sides+1) + x);
            data.addIndex((y+1) * (sides+1) + x);
        }
    }

    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    _indexBuffer.create();
    _indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    data.allocateVertex(_vertexBuffer);
    data.allocateTexture(_textureBuffer);
    data.allocateIndex(_indexBuffer);
}

void SkyDome::Render(Renderer *r)
{
    _vertexBuffer.bind();
    r->bindVertex(Renderer::Vertex, 3);

    _textureBuffer.bind();
    r->bindVertex(Renderer::TexCoord, 2);

    _indexBuffer.bind();

    for( int x=0; x < _prop.hemisphereCount*_prop.slices; ++x)
        r->drawElements(GL_TRIANGLE_STRIP, (_prop.sides +1)* 2, x * (_prop.sides+1)*2);

    r->unBindBuffers();
}
