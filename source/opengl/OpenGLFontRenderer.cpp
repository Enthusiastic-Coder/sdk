#include "stdafx.h"
#include "OpenGLFontRenderer.h"
#include "OpenGLFontTexture.h"
#include "OpenGLPipeline.h"
#include "OpenGLRenderer.h"
#include "OpenGLShaderProgram.h"

void OpenGLFontRenderer::selectRenderer(Renderer *r)
{
    _r = r;
}

void OpenGLFontRenderer::selectShader(OpenGLShaderProgram *p)
{
    _shader = p;
}

void OpenGLFontRenderer::selectFont(OpenGLFontTexture *texture)
{
    _fontMesh.setFont(texture);
}

void OpenGLFontRenderer::beginRender()
{
    _r->useProgram(*_shader);
    _shader->sendUniform("textColor", _color);
    _shader->sendUniform("backColor", _backColor);
    _fontMesh.beginRender(_r);
}

void OpenGLFontRenderer::setColor( const Vector4F &color)
{
    _color = color;
}

void OpenGLFontRenderer::setBkColor(const Vector4F &color)
{
    _backColor = color;
}

void OpenGLFontRenderer::bindMatrices()
{
    OpenGLPipeline::Get(_r->camID).bindMatrices(_r->progId());
}

void OpenGLFontRenderer::renderText(int x, int y, const QString& str)
{
    _fontMesh.resetIndex();
    _fontMesh.add( x, y, str);
    _fontMesh.render(_r);
}

void OpenGLFontRenderer::renderText(int x, int y, const std::vector<QString> &lines)
{
    _fontMesh.resetIndex();
    _fontMesh.add(x, y, lines);
    _fontMesh.render(_r);
}

void OpenGLFontRenderer::endRender()
{
    _fontMesh.endRender(_r);
    OpenGLShaderProgram::useDefault();
}

int OpenGLFontRenderer::getWidth(const QString &str) const
{
    return _fontMesh.getWidth(str);
}
