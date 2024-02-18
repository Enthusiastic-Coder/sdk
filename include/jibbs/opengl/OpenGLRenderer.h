#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <jibbs/vector/vector3.h>

class OpenGLTexture2D;
class OpenGLVertexBuffer;
class OpenGLShaderProgram;

#include <QOpenGLFunctions>

class Renderer : private QOpenGLFunctions
{
public:
    Renderer();
    float dt = 1/30.0f;

    unsigned int camID  = 0;
    float fLightingFraction = 1.0f;

    static OpenGLShaderProgram nullShader;
    //Shader Variables
    void useProgram(OpenGLShaderProgram &progId );
    OpenGLShaderProgram& progId();

    //Mesh
    void bindMaterial(Vector3F &diffuseColor, Vector3F &specularColor, float shininess);
    void bindTexture(int id, int activeUnit = 0);
    void unBindBuffers();

    enum VertexType
    {
        Vertex = 0x1,
        Color = 0x2,
        Normal = 0x4,
        TexCoord = 0x8,
        TexCoord2 = 0x10
    };

    void bindVertex(VertexType attribType, int attribSize, GLsizei stride=0, int offset=0);

    void drawElements(int primitiveType, unsigned int count, unsigned int offset=0);
    void drawArrays(int primitiveType, unsigned int count, unsigned int offset=0);

private:
    void bindGenericBuffer(int attribType, int attribSize, GLsizei stride, int offset);

    OpenGLShaderProgram*_progId = &nullShader;

    int _vertexAttribId = -1;
    int _colorAttribId = -1;
    int _normalAttribId = -1;
    int _texCoordAttribId = -1;
    int _texCoordAttrib2Id = -1;
};

class OpenGLRenderer : public Renderer
{
};


#endif // OPENGLRENDERER_H
