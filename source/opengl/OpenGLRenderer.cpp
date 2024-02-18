#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/opengl/OpenGLPipeline.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

OpenGLShaderProgram Renderer::nullShader;

Renderer::Renderer() :  _progId(&nullShader)
{
    QOpenGLFunctions::initializeOpenGLFunctions();
}

void Renderer::useProgram(OpenGLShaderProgram& progId)
{
	progId.use();
    _progId = &progId;
}

OpenGLShaderProgram& Renderer::progId()
{
    return *_progId;
}

void Renderer::drawElements(int primitiveType, unsigned int count, unsigned int offset)
{
    glDrawElements(primitiveType, count, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(unsigned short) * offset));
}

void Renderer::drawArrays(int primitiveType, unsigned int count, unsigned int offset)
{
    glDrawArrays(primitiveType, offset, count);
}

void Renderer::bindMaterial(Vector3F& diffuseColor, Vector3F& specularColor, float shininess)
{
    Vector4F materialDiffuse = diffuseColor * fLightingFraction;
    Vector4F materialSpecular = specularColor * fLightingFraction;

    materialDiffuse.w = 1.0f;
    materialSpecular.w = 1.0f;

    OpenGLPipeline::ApplyMaterialToShader(progId(), materialDiffuse, materialDiffuse, materialSpecular, shininess);

    Vector4F lightDiffuse( fLightingFraction, fLightingFraction, fLightingFraction, 1.0f);

    OpenGLPipeline::ApplyLightToShader(progId(), lightDiffuse, lightDiffuse, lightDiffuse, shininess);
}

void Renderer::bindTexture(int id, int activeUnit)
{
    if( activeUnit != 0)
        glActiveTexture(GL_TEXTURE0 + activeUnit);

    glBindTexture(GL_TEXTURE_2D, id);

    if( activeUnit != 0)
        glActiveTexture(GL_TEXTURE0);
}

void Renderer::bindGenericBuffer(int attribType, int attribSize, GLsizei stride, int offset)
{
    int attribId = -1;

    if( attribType == Vertex)
        attribId = _vertexAttribId = OpenGLPipeline::GetVertexAttribId(progId());

    else if( attribType == Color)
        attribId = _colorAttribId = OpenGLPipeline::GetColorAttribId(progId());

    else if( attribType == Normal)
        attribId = _normalAttribId = OpenGLPipeline::GetNormalAttribId(progId());

    else if( attribType == TexCoord)
        attribId = _texCoordAttribId = OpenGLPipeline::GetTextureAttribId(progId());

    else if( attribType == TexCoord2)
        attribId = _texCoordAttrib2Id = OpenGLPipeline::GetTexture2AttribId(progId());

    if( attribId == -1)
        return;

    glEnableVertexAttribArray(attribId);
    glVertexAttribPointer(attribId, attribSize, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
}

void Renderer::unBindBuffers()
{  
    if( _vertexAttribId != -1 )
    {
        glDisableVertexAttribArray(_vertexAttribId);
        _vertexAttribId = -1;
    }

    if( _colorAttribId != -1 )
    {
        glDisableVertexAttribArray(_colorAttribId);
        _colorAttribId = -1;
    }

    if( _normalAttribId != -1 )
    {
        glDisableVertexAttribArray(_normalAttribId);
        _normalAttribId = -1;
    }

    if( _texCoordAttribId != -1 )
    {
        glDisableVertexAttribArray(_texCoordAttribId);
        _texCoordAttribId = -1;
    }

    if( _texCoordAttrib2Id != -1 )
    {
        glDisableVertexAttribArray(_texCoordAttrib2Id);
        _texCoordAttrib2Id = -1;
    }

    QOpenGLBuffer::release(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer::release(QOpenGLBuffer::IndexBuffer);
}

void Renderer::bindVertex(VertexType attribType, int attribSize, GLsizei stride, int offset)
{
    bindGenericBuffer(attribType, attribSize, stride, offset);
}


