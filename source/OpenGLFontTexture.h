#pragma once

#include <jibbs/math/MathSupport.h>
#include <QOpenGLTexture>
#include <QString>
#include <vector>

class OpenGLFontTexture
{
public:

    void setFont(const QFont &font);
    void getUV(unsigned char ch, std::pair<float, float> outPts[]);
    QOpenGLTexture *texture();
    MathSupport<int>::size getFontSize(unsigned char ch);
    MathSupport<int>::size getMaxFontSize();
    int getWidth(const QString& str) const;

private:
    struct chInfo
    {
        float U[4];
        float V[4];
        int w, h;
    };

    std::unique_ptr<QOpenGLTexture> _fontTexture;
    MathSupport<int>::size _maxFontSize = {};
    std::vector<chInfo> _fontInfo;
};
