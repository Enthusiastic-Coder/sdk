#include <jibbs/opengl/OpenGLFontTexture.h>

#define NOMINMAX

#include <QFontMetrics>
#include <QPainter>

class FontMetaData
{
public:
    FontMetaData(QFont font);

    QImage process();
    void setFont(const QFont &);

    QSize maxFontSize() const;
    std::pair<float,float> getUV(unsigned char ch);
    std::pair<float,float> getCharUV(unsigned char ch, bool right, bool top);
    std::pair<int,int> getCharMetrics(unsigned char ch);

private:
    QSizeF _fontSize;
    QSize _imageSize;
    QFont _font;
};

int nextPowerOf2(int dims)
{
    int i = 1;
    while(i < dims)
        i <<= 1;

    return i;
}

FontMetaData::FontMetaData(QFont font)
    : _font(font)
{

}

QImage FontMetaData::process()
{
    QFontMetricsF fm(_font);

    _fontSize.setWidth(fm.maxWidth());
    _fontSize.setHeight(fm.height());

    const int chWidth = std::ceil(_fontSize.width());
    const int chHeight = std::ceil(_fontSize.height());

    const int width = nextPowerOf2( chWidth * 16);
    const int height = nextPowerOf2( chHeight * 16);

    _imageSize.setWidth(width);
    _imageSize.setHeight(height);

    QImage image( width, height, QImage::Format_ARGB32);

    image.fill(Qt::transparent);

    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);
    p.setFont(_font);

    p.fillRect(0, 0, image.width(), image.height()-chHeight*16, QBrush(QColor(0,0,255)));
    p.fillRect(chWidth * 16, image.height()-chHeight*16, image.width()-chWidth * 16, chHeight * 16, QBrush(QColor(0,0,255)));

    p.setPen( qRgba(255,255,255,0));

    for( int i = 0; i < 256 ; ++i )
    {
        unsigned char ch = i;
        auto uv = getUV(ch);
        float U = uv.first;
        float V = 1-uv.second;

        p.drawText(QRect(U*width, V*height-_fontSize.height(), _fontSize.width(), _fontSize.height()), Qt::AlignCenter, QString(QChar(ch)));
    }

    return image;
}

std::pair<float,float> FontMetaData::getUV(unsigned char ch)
{
    float U = (ch % 16) * float(_fontSize.width()) / _imageSize.width();
    float V = (ch /16 * float(_fontSize.height())) / _imageSize.height();
    return std::make_pair(U,V);
}

std::pair<float, float> FontMetaData::getCharUV(unsigned char ch, bool right, bool top)
{
    QFontMetrics fm(_font);
    auto uv = getUV(ch);
    float U = uv.first;
    float V = uv.second;

    float x = U*_imageSize.width()+(_fontSize.width()-fm.horizontalAdvance(QChar(ch)))/2;
    float y = V*_imageSize.height();

    if(right) x += fm.horizontalAdvance(QChar(ch));
    if(top)  y += fm.height();

    return std::make_pair(x/_imageSize.width(), y / _imageSize.height());
}

std::pair<int, int> FontMetaData::getCharMetrics(unsigned char ch)
{
    QFontMetrics fm(_font);
    return std::make_pair(fm.horizontalAdvance(QChar(ch)), fm.height());
}

void OpenGLFontTexture::setFont(const QFont& font)
{
    FontMetaData data(font);

    QImage img = data.process();

    _fontInfo.resize(256);

    for( size_t i=0; i <256; ++i)
    {
        unsigned char ch = (unsigned char)i;

        auto UV0 = data.getCharUV(ch,false,false);
        auto UV1 = data.getCharUV(ch,true,false);
        auto UV2 = data.getCharUV(ch,true,true);
        auto UV3 = data.getCharUV(ch,false,true);

        chInfo& info = _fontInfo[i];

        info.U[0] = UV0.first; info.V[0] = UV0.second;
        info.U[1] = UV1.first; info.V[1] = UV1.second;
        info.U[2] = UV2.first; info.V[2] = UV2.second;
        info.U[3] = UV3.first; info.V[3] = UV3.second;
        auto charMetrics = data.getCharMetrics(ch);
        info.w = charMetrics.first;
        info.h = charMetrics.second;
    }
    _fontTexture.reset(new QOpenGLTexture(img.mirrored()));
    _fontTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::Linear);
}

QOpenGLTexture* OpenGLFontTexture::texture()
{
    return _fontTexture.get();
}

MathSupport<int>::size OpenGLFontTexture::getFontSize(unsigned char ch)
{
    if( _fontInfo.size()==0)
        return {0,0};

    const chInfo& info = _fontInfo[ch];
    return { info.w, info.h};
}

MathSupport<int>::size OpenGLFontTexture::getMaxFontSize()
{
    return _maxFontSize;
}

int OpenGLFontTexture::getWidth(const QString &str) const
{
    const int length = str.length();
    int totalWidth = 0;
    for(int i=0; i < length; ++i)
    {
        totalWidth += _fontInfo[(unsigned char)str.at(i).toLatin1()].w;
    }
    return totalWidth;
}

void OpenGLFontTexture::getUV(unsigned char ch, std::pair<float, float> outPts[])
{
    if( _fontInfo.size()==0)
        return;

    const chInfo& info = _fontInfo[ch];
    outPts[0].first = info.U[0];
    outPts[0].second = info.V[0];
    outPts[1].first = info.U[1];
    outPts[1].second = info.V[1];
    outPts[2].first = info.U[2];
    outPts[2].second = info.V[2];
    outPts[3].first = info.U[3];
    outPts[3].second = info.V[3];
}
