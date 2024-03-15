#include <jibbs/utilities/stdafx.h>
#include <jibbs/opengl/OpenGLPipeline.h>
#include <jibbs/maptiles/WorldTileRenderer.h>

#include <QDebug>

WorldTileRenderer::WorldTileRenderer(QObject *parent) : QObject(parent)
{
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
}

void WorldTileRenderer::init(std::shared_ptr<QtTextureManager> texManager)
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    _texManager = texManager;

    _renderer.reset(new Renderer);

    if( !_shader.loadSrc(readAllRP("shaders/texFlatShader.vert"),readAllRP("shaders/texFlatShader.frag")))
    {
        qDebug() << QString::fromStdString(_shader.getError());
    }

    connect( _timer, &QTimer::timeout, this, [this]
    {
#ifdef Q_OS_WIN
        qDebug() << "WorldTileRenderer -- clean up Timer";
#endif
        for(auto it = _textures.begin(); it != _textures.end();)
        {
            if( !it->second.bRequired)
            {
                it = _textures.erase(it);
            }
            else
                ++it;
        }
    });

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _textureBuffer.bind();

    float uv[] = { 0,1,
                  1,1,
                  1,0,
                  0,0};

    _textureBuffer.allocate(uv, sizeof(uv));
    _textureBuffer.release();

    _indexBuffer.create();
    _indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _indexBuffer.bind();

    unsigned short indices[] = {0,1,3, 1,2,3};

    _indexBuffer.allocate(indices, sizeof(indices));
    _indexBuffer.release();
}

void WorldTileRenderer::setGPSOrigin(GPSLocation location)
{
    _getGPSOrigin = location;
}

void WorldTileRenderer::setGPSToScreenCallBack(std::function<QPointF (GPSLocation)> callBack)
{
    _toScreenCallBack = callBack;
}

void WorldTileRenderer::setDimensions(float width, float height)
{
    _width = width;
    _height = height;
}

void WorldTileRenderer::releaseAllTextures()
{
    for(auto&item:_textures)
        item.second.bRequired = false;

    if( !_timer->isActive() && !_textures.empty())
        _timer->start(100);
}

void WorldTileRenderer::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void WorldTileRenderer::BuildGroundTile(int tileWidth, int tileHeight)
{
#ifdef Q_OS_WIN
    //qDebug() << "****" << Q_FUNC_INFO << "****";
#endif
    const GPSLocation& loc = _getGPSOrigin;

    const QString tilePath = ":/dim600";

    const int lat = loc._lat;
    const int lng = loc._lng;

//#define __TILE__ADJ__
#ifdef __TILE__ADJ__
    QPoint topLeft = rv->toScreen(GPSLocation(lat+1,lng));
    QPoint bottomRight = rv->toScreen(GPSLocation(lat,lng+1));
    QSize tileSize = QRect(topLeft, bottomRight).size();
#endif

    releaseAllTextures();

    if( _enabled)
    for( int x=-tileWidth; x < tileWidth; ++x)
    {
        for(int y = -tileHeight; y < tileHeight; ++y)
        {
#ifdef __TILE__ADJ__
            QPoint tL = QPoint(topLeft.x() + tileSize.width() *x, topLeft.y() - tileSize.height()*y);
            QPoint bR;
            bR.setX( tL.x()+ tileSize.width());
            bR.setY( tL.y() + tileSize.height());
#else
            QPointF tL = _toScreenCallBack(GPSLocation(y+1+lat,x+lng));
            QPointF bR = _toScreenCallBack(GPSLocation(y+lat,x+1+lng));
#endif

            if( !QRectF(tL,bR).intersects(QRectF(0,0, _width, _height)))
                continue;

            const QString filename = QString("%1/%2%3/%2%3%4%5.png")
                    .arg(tilePath)
                    .arg( (y+lat)>= 0?"n":"s")
                    .arg( qAbs(y+lat), 2, 10, QChar('0'))
                    .arg((x+lng) < 0 ?"w":"e")
                    .arg(qAbs(x+lng), 3, 10, QChar('0'));

            Tile t;

            if( (y+lat) < 60 && (y+lat) > -55) //SRTM Data has nothing above 60 LAT
            {
                auto it = _textures.find(filename);

                if( it != _textures.end())
                    t.tex = it->second.tex;

                if( t.tex == nullptr)
                    t.tex = _texManager->loadTexture(filename, _minification, _magnification);

                if( t.tex == nullptr)
                    continue;
            }
            else
                continue;

            t.bRequired = true;
            t.tL = tL;
            t.bR = bR;

            t.vertices[0] = tL.x();
            t.vertices[1] = bR.y();

            t.vertices[3] = bR.x();
            t.vertices[4] = bR.y();

            t.vertices[6] = bR.x();
            t.vertices[7] = tL.y();

            t.vertices[9] = tL.x();
            t.vertices[10] = tL.y();

            t.updateBuffer();

            _textures[filename] = t;
        }
    }
}

void WorldTileRenderer::render(float compassValue)
{
    if( !_enabled)
        return;

    glDisable(GL_DEPTH_TEST);
    _renderer->camID = 1;
    _renderer->useProgram( _shader);

    OpenGLPipeline& pipeline = OpenGLPipeline::Get(_renderer->camID);
    pipeline.Push();
    pipeline.GetModel().LoadIdentity();
    pipeline.GetView().LoadIdentity();
    pipeline.GetView().Translate(-_width/2, -_height/2,0);
    pipeline.GetView().Rotate(0,0,-compassValue);
    pipeline.GetView().Translate(_width/2, _height/2,0);
    pipeline.GetProjection().LoadIdentity();
    pipeline.GetProjection().SetOrthographic(0, _width, _height, 0, -1, 1);

    pipeline.bindMatrices(_renderer->progId());

    _renderer->progId().sendUniform("sea", _sea.redF(), _sea.greenF(), _sea.blueF(), _sea.alphaF());
    _renderer->progId().sendUniform("ground", _ground.redF(), _ground.greenF(), _ground.blueF(), _ground.alphaF());

    _textureBuffer.bind();
    _renderer->bindVertex(Renderer::TexCoord, 2);

    _indexBuffer.bind();

    for(auto&item:_textures)
    {
        Tile& t = item.second;

        if( !t.bRequired)
            continue;

        if( t.tex == nullptr)
            continue;

        t._vertexBuffer.bind();
        _renderer->bindVertex(Renderer::Vertex, 3);
        glBindTexture(GL_TEXTURE_2D, t.tex->textureId());

        _renderer->drawElements(GL_TRIANGLES, _indexBuffer.size()/sizeof(unsigned short));
    }

    _renderer->unBindBuffers();
    glBindTexture(GL_TEXTURE_2D, 0);

    pipeline.Pop();
}

void WorldTileRenderer::setSea(QColor c)
{
    _sea = c;
}

void WorldTileRenderer::setGround(QColor c)
{
    _ground = c;
}


