#pragma once

class OpenGLFontTexture;
class Renderer;
class OpenGLShaderProgram;

#include "OpenGLFontMeshBuilder.h"

class OpenGLFontRenderer
{
public:
    void selectRenderer(Renderer* r);
    void selectShader(OpenGLShaderProgram* p);
    void selectFont(OpenGLFontTexture* texture);
    void beginRender();
    void setColor( const Vector4F& color);
    void setBkColor( const Vector4F& color);

    void bindMatrices();
    void renderText(int x, int y, const QString &str);
    void renderText( int x, int y, const std::vector<QString>& lines);
    void endRender();

    int getWidth(const QString& str) const;
private:
	Renderer* _r = nullptr;
    OpenGLFontMeshBuilder _fontMesh;
    Vector4F _color = {1,1,1,1};
    Vector4F _backColor = {0,0,0,1};
    OpenGLShaderProgram* _shader = nullptr;
};
