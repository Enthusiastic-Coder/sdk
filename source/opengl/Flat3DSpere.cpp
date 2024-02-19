#include <jibbs/utilities/stdafx.h>
#include <jibbs/opengl/OpenGLPipeline.h>
#include <jibbs/math/qhdgtable.h>
#include <jibbs/opengl/Flat3DSpere.h>

#include <QString>
#include <QDebug>

#include <QNetworkReply>
#include <QPainter>
#include <QFont>
#include <QSettings>
#include <QAuthenticator>

namespace {
struct GPSCoordinates {
    double latitude;
    double longitude;
};

struct TileCoordinates {
    int x;
    int y;
};

struct TileCorners {
    GPSCoordinates topLeft;
    GPSCoordinates bottomRight;
};

TileCorners calculateTileCorners(int tileX, int tileY, int zoom) {
    // Calculate GPS coordinates of the top-left corner of the current tile
    double lon_deg_top_left = tileX / pow(2, zoom) * 360.0 - 180.0;
    double lat_rad_top_left = atan(sinh(M_PI * (1 - 2 * tileY / pow(2, zoom))));
    double lat_deg_top_left = lat_rad_top_left * 180.0 / M_PI;

    // Calculate GPS coordinates of the bottom-right corner of the current tile
    double lon_deg_bottom_right = (tileX + 1) / pow(2, zoom) * 360.0 - 180.0;
    double lat_rad_bottom_right = atan(sinh(M_PI * (1 - 2 * (tileY + 1) / pow(2, zoom))));
    double lat_deg_bottom_right = lat_rad_bottom_right * 180.0 / M_PI;

    TileCorners corners;
    corners.topLeft = {lat_deg_top_left, lon_deg_top_left};
    corners.bottomRight = {lat_deg_bottom_right, lon_deg_bottom_right};

    return corners;
}

TileCoordinates mapGPSToTile(double latitude, double longitude, int zoom) {
    int n = pow(2, zoom);
    double xtile = n * ((longitude + 180) / 360);
    double ytile = n * (1 - (log(tan(latitude * M_PI / 180) + 1 / cos(latitude * M_PI / 180)) / M_PI)) / 2;

    return {static_cast<int>(xtile), static_cast<int>(ytile)};
}

QString getTileURL(QString url, int zoom, int x, int y) {
    url = url.replace("{x}", QString::number(x));
    url = url.replace("{y}", QString::number(y));
    url = url.replace("{zoom}", QString::number(zoom));
    return url;
}

QString getTileId(int zoom, int tileX, int tileY) {
    return QString("%1/%2/%3").arg(zoom).arg(tileX).arg(tileY);
}

QImage getTileImage(const QString msg, QColor bgColor=Qt::darkGray, int fontSize=14)
{
    static std::unordered_map<QString,QImage> imgTiles;

    auto it = imgTiles.find(msg);
    if( it == imgTiles.end())
    {
        QImage img = QImage(100,100, QImage::Format_RGBA8888);
        QPainter p(&img);
        p.fillRect(img.rect(), bgColor);

        QPen pen(Qt::white);
        p.setPen(pen);

        QFont f("Verdana", fontSize);

        p.drawRect(0, 0, img.width(), img.height());
        QFontMetrics fm(f);
        p.setFont(f);
        p.drawText((-fm.horizontalAdvance(msg)+img.width())/2,(+fm.height()+img.height())/2, msg);
        imgTiles[msg] = img;
        return img;
    }

    return it->second;
}

}

Flat3DSphere::Flat3DSphere()
{
    _networkAccessManager = new QNetworkAccessManager(this);

    _networkAccessManager->setCookieJar(&cookieJar);

    connect(_networkAccessManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply)
            {
                reply->deleteLater();
#ifdef Q_OS_WIN
                qDebug() << QTime::currentTime() << " - " << reply->url();
#endif

                QNetworkReply::NetworkError nError = reply->error();

                const QString tileId  = reply->request().attribute(QNetworkRequest::User).toString();

                auto it = _unrealisedTextures.find(tileId);
                if( it == _unrealisedTextures.end())
                    return;

                const int index = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+1)).toInt();

                if( index >= it->second.items.size())
                    return;

                it->second.dirty = true;

                TileRetrieve::Item& item = it->second.items[index];

                if( nError == QNetworkReply::NoError)
                {
                    const QImage img = QImage::fromData(reply->readAll());

                    if( img.isNull())
                    {
                        item.downloadState = TileRetrieve::downloadState::downloadedUnavailable;
                    }
                    else
                    {
                        item.downloadState = TileRetrieve::downloadState::downloaded;
                        item.img = img;
                    }
                }
                else
                {
                    item.downloadState = TileRetrieve::downloadState::failed;
                    qDebug() << Q_FUNC_INFO << "-" << reply->errorString();
                }
            });

    connect(_networkAccessManager, &QNetworkAccessManager::authenticationRequired, this, [this](QNetworkReply* reply, QAuthenticator* auth)
            {
                Q_UNUSED(reply);
                auth->setUser(_tileMapUser);
                auth->setPassword(_tileMapPassword);
            });

    _timer = new QTimer(this);
    _timer->setSingleShot(true);

    connect(_timer, &QTimer::timeout, this, &Flat3DSphere::removeUnusedTextures, Qt::QueuedConnection);

    _renderer.reset(new Renderer);

    if( !_multTexShader.loadSrc(readAllRP("shaders/multTexShader.vert"), readAllRP("shaders/multTexShader.frag")))
    {
        qDebug() << QString::fromStdString(_multTexShader.getError());
    }

    if( !_texshader.loadSrc(readAllRP("shaders/texShader.vert"), readAllRP("shaders/texShader.frag")))
    {
        qDebug() << QString::fromStdString(_texshader.getError());
    }

    _texID.reset(new QOpenGLTexture(QImage(ResourcePath("images/earthmap.jpg")).mirrored()));
    _texNightID.reset(new QOpenGLTexture(QImage(ResourcePath("images/earthmap_night.jpg")).mirrored()));

    updateTexture();
}

void Flat3DSphere::render(const GPSLocation& g, bool dayLight)
{
    if(_bDirty)
    {
        _bDirty = false;
        if( _tileMapActive)
            BuildOpenStreetMapTile(g);
        else
            BuildGroundTile(g);
    }

    if( _tileMapActive)
        renderMap(g._height);

    else
        renderTextures(g._height,dayLight);
}

void Flat3DSphere::setDirty()
{
    _bDirty = true;
}

void Flat3DSphere::setCamera(EulerF cam)
{
    _cam = cam;
}

void Flat3DSphere::setTextureMinMag(QOpenGLTexture::Filter minification, QOpenGLTexture::Filter magnification)
{
    _minification = minification;
    _magnification = magnification;
    updateTexture();
}

void Flat3DSphere::updateTexture()
{
    _texID->setMinMagFilters(_minification, _magnification);
    _texNightID->setMinMagFilters(_minification, _magnification);
}

void Flat3DSphere::removeUnusedTextures()
{
#ifdef Q_OS_WIN
    qDebug() << "Flat3DSphere -- clean up Timer (" << _minNumberToDelete << ")";
#endif
    int numDeleted=0;
    for(auto it = _tiles.begin(); it != _tiles.end();)
    {
        if( it->second.bRequired)
        {
            ++it;
            continue;
        }

        it = _tiles.erase(it);
        _timer->start(50);
        numDeleted++;
        if( numDeleted > _minNumberToDelete)
            return;
    }

    if( numDeleted == 0)
        _minNumberToDelete = 0;
}

void Flat3DSphere::releaseAllTextures()
{
    for(auto&item:_tiles)
        item.second.bRequired = false;

    _minNumberToDelete = std::max( _minNumberToDelete, static_cast<int>(_tiles.size() / 5.0f/ 10.0f));

    if( !_timer->isActive() )
    {
        _timer->start(1000);
    }
}

void Flat3DSphere::consumeUnrealisedTextures()
{
    auto minification = _tileMapActive? QOpenGLTexture::Linear:_minification;
    auto magnification = _tileMapActive ? QOpenGLTexture::Linear:_magnification;

    std::vector<QString> toDelete;

    for( auto& item: _unrealisedTextures)
    {
        auto it = _tiles.find(item.first);

        if( it == _tiles.end())
            continue;

        TileRetrieve& tr = item.second;

        if( !tr.dirty)
            continue;

        tr.dirty = false;

        const TileRetrieve::downloadState state = tr.status();

        if( state == TileRetrieve::downloadState::downloaded)
        {
            toDelete.push_back(it->first);

            if( tr.showZoom)
                it->second.tex = _textureCacheManager.getTexture(tr.getImage());
            else
                it->second.tex = std::make_shared<QOpenGLTexture>(tr.getImage());
        }

        else if( state == TileRetrieve::downloadState::downloadedUnavailable)
        {
            toDelete.push_back(it->first);
            it->second.tex = _textureCacheManager.getTexture(getTileImage("Unavailable"));
        }

        else if( tr.level == 0 && state == TileRetrieve::downloadState::downloading)
            it->second.tex = _textureCacheManager.getTexture(getTileImage("Loading", Qt::darkGray, 12));

        else if( state == TileRetrieve::downloadState::failed)
            it->second.tex = _textureCacheManager.getTexture(getTileImage("Failed"));

        if( it->second.tex)
            it->second.tex->setMinMagFilters(minification, magnification);
    }

    for(const auto& id:toDelete)
        _unrealisedTextures.erase(id);
}

void Flat3DSphere::downloadTile(const QString& finalURL, const QString& tileId, int idx )
{
    QUrl url(finalURL);
    QNetworkRequest request(url);
    // Set a user-agent header
    request.setHeader(QNetworkRequest::UserAgentHeader, _userInfo);
    request.setAttribute(QNetworkRequest::User, tileId);
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+1), idx);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    _networkAccessManager->get(request);
};

bool Flat3DSphere::retryTile(int zoom, int tileX, int tileY)
{
    const QString tileId = getTileId(zoom, tileX, tileY);

    auto it = _unrealisedTextures.find(tileId);

    bool downloadHappened = false;

    if( it != _unrealisedTextures.end())
    {
        TileRetrieve& tr = it->second;

        for(size_t i = 0; i < tr.items.size(); ++i)
        {
            TileRetrieve::Item& item = tr.items[i];

            if( item.downloadState == TileRetrieve::downloadState::failed)
            {
                item.downloadState = TileRetrieve::downloadState::downloading;
                downloadTile(item.tileUrl, tileId, static_cast<int>(i));
                downloadHappened = true;
            }
        }
    }

    return downloadHappened;
}

void Flat3DSphere::requestTile(int zoom, int tileX, int tileY)
{
    const QString tileId = getTileId(zoom, tileX, tileY);

    TileRetrieve tr;
    tr.level = zoom-10;
    tr.showZoom = false;

    for(const auto& tileUrl : _tileMapURLs)
    {
        if( tileUrl.isEmpty())
            continue;

        const QString finalURL = getTileURL(tileUrl, zoom, tileX, tileY);

        TileRetrieve::Item item;
        item.tileUrl = finalURL;
        item.downloadState = TileRetrieve::downloadState::downloading;
        tr.items.push_back(item);
        downloadTile(finalURL, tileId, static_cast<int>(tr.items.size()-1));
    }

    if( tr.items.empty())
    {
        TileRetrieve::Item item;
        item.downloadState = TileRetrieve::downloadState::downloaded;
        item.img = getTileImage("Not Set");
        tr.items.push_back(item);
    }

    _unrealisedTextures[tileId] = tr;
}

void Flat3DSphere::requestZoomTile(int zoom, const QString& tileId, QColor color)
{
    TileRetrieve tr;
    tr.showZoom = _tileMapShowZoom;
    TileRetrieve::Item item;
    item.downloadState = TileRetrieve::downloadState::downloaded;
    item.img = getTileImage(QString::number(zoom), color, 22);
    tr.items.push_back(item);
    _unrealisedTextures[tileId] = tr;
}
void Flat3DSphere::BuildOpenStreetMapTile(const GPSLocation &g)
{
#ifdef Q_OS_WIN
    qDebug() << Q_FUNC_INFO;
#endif
    releaseAllTextures();

    const float pitch = std::max(2.0f, fabs(_cam._pitch));

    const float radius = g._height / tan( DegreesToRadians(pitch * 1.2));
    const QuarternionF& qHdg = QHDG(static_cast<int>(_cam._heading));
    const Vector3F pos = QVRotate( qHdg, Vector3F(0,0,-radius));
    const GPSLocation gps = g + pos;

    struct TileData {
        QColor bgColor;
        int tileCountX;
        int tileCountY;
        int offsetHeight;
    };

    std::map<int, TileData> tileLayers;


    const int diffZ = 5;
    int zHeight = diffZ;

    if( isZoomVisible(14))
        tileLayers[14] = {Qt::darkYellow, 1, 1, zHeight -=diffZ};

    if( isZoomVisible(13))
        tileLayers[13] = {Qt::darkGreen, 1, 1, zHeight -=diffZ};

    if( isZoomVisible(12))
        tileLayers[12] = {Qt::darkCyan, 1, 1, zHeight -=diffZ};

    tileLayers[10] = {Qt::darkBlue, 3, 3, zHeight -=diffZ};


    const int depthFactor = g._height /1000 + 1.0f;

    auto setTileHeightFactor = [&tileLayers](int tile, int factor, int maxFactor) {
        auto it = tileLayers.find(tile);
        if( it == tileLayers.end())
            return;

        TileData& data = it->second;
        data.tileCountX = data.tileCountY = std::min(factor,maxFactor);
    };

    setTileHeightFactor(12, g._height /4000 + 1.0, 4);
    setTileHeightFactor(10, g._height /2000 + 1.0, 5);

    auto addPts = [g,depthFactor](Tile& t, const GPSLocation& tL, const GPSLocation& tR, const GPSLocation& bL, const GPSLocation& bR, float offSetY) {

        t.addGPSPoint(g, bL, offSetY * depthFactor);
        t.addGPSPoint(g, bR, offSetY * depthFactor);
        t.addGPSPoint(g, tL, offSetY * depthFactor);

        t.addGPSPoint(g, bR, offSetY * depthFactor);
        t.addGPSPoint(g, tR, offSetY * depthFactor);
        t.addGPSPoint(g, tL, offSetY * depthFactor);

        t.createBuffers();
    };

    auto editPts = [g,depthFactor](Tile& t, const GPSLocation& tL, const GPSLocation& tR, const GPSLocation& bL, const GPSLocation& bR, float offSetY) {

        t.editGPSPoint(0, g, bL, offSetY * depthFactor);
        t.editGPSPoint(1, g, bR, offSetY * depthFactor);
        t.editGPSPoint(2, g, tL, offSetY * depthFactor);

        t.editGPSPoint(3, g, bR, offSetY * depthFactor);
        t.editGPSPoint(4, g, tR, offSetY * depthFactor);
        t.editGPSPoint(5, g, tL, offSetY * depthFactor);

        t.updateBuffers();
    };

    bool bFirst = true;
    double latitude;
    double longitude;
    std::pair<GPSLocation,GPSLocation> boundary0;

    for( auto& tileLayer : tileLayers)
    {
        const int zoom = tileLayer.first;

        if( bFirst)
        {
            latitude = g._lat;
            longitude = g._lng;
        }
        else
        {
            latitude = gps._lat;
            longitude = gps._lng;
        }

        TileCoordinates centralTile = mapGPSToTile(latitude, longitude, zoom);

        for( int x=-tileLayer.second.tileCountX; x <=tileLayer.second.tileCountX; ++x)
        {
            for(int y=tileLayer.second.tileCountY; y >=-tileLayer.second.tileCountY; --y)
            {
                const int tileX = centralTile.x + x;
                const int tileY = centralTile.y + y;

                const TileCorners corner = calculateTileCorners(tileX, tileY, zoom);

                const GPSLocation tL = {corner.topLeft.latitude, corner.topLeft.longitude};
                const GPSLocation tR = {corner.topLeft.latitude, corner.bottomRight.longitude};

                const GPSLocation bL = {corner.bottomRight.latitude, corner.topLeft.longitude};
                const GPSLocation bR = {corner.bottomRight.latitude, corner.bottomRight.longitude};

                if( bFirst )
                {
                    if(x == -tileLayer.second.tileCountX)
                        boundary0.first._lng = tL._lng;

                    if(x == tileLayer.second.tileCountX)
                        boundary0.second._lng = bR._lng;

                    if(y == -tileLayer.second.tileCountY)
                        boundary0.first._lat = tL._lat;

                    if(y == tileLayer.second.tileCountY)
                        boundary0.second._lat = bR._lat;
                }
                else
                {
                    if( tL._lng < boundary0.first._lng)
                        return;

                    if( tL._lat > boundary0.first._lat)
                        return;

                    if( bR._lng > boundary0.second._lng)
                        return;

                    if( bR._lat < boundary0.second._lat)
                        return;
                }

                const QString tileId = getTileId(zoom, tileX, tileY);

                auto it = _tiles.find(tileId);

                if( it == _tiles.end())
                {
                    Tile t;

                    addPts(t, tL, tR, bL, bR, tileLayer.second.offsetHeight);
                    _tiles[tileId] = std::move(t);

                    if( _tileMapShowZoom)
                        requestZoomTile(zoom, tileId, tileLayer.second.bgColor);
                    else
                        requestTile(zoom, tileX, tileY);
                }
                else
                {
                    Tile& t= it->second;
                    t.bRequired = true;

                    retryTile(zoom, tileX, tileY);

                    editPts(t, tL, tR, bL, bR, tileLayer.second.offsetHeight);
                }
            }
        }

        bFirst = false;
    }
}

void Flat3DSphere::BuildGroundTile(const GPSLocation& g)
{
#ifdef Q_OS_WIN
    qDebug() << Q_FUNC_INFO;
#endif

    releaseAllTextures();

    const int tileCount = g._height > 6000 ? 3 : (g._height > 3000 ? 2 : 1);

    const QString tilePath = ":/dim600";

    const int lat = std::floor(g._lat);
    const int lng = std::floor(g._lng);

    const GPSLocation g0(lat,lng);

    auto addPts = [g,g0](Tile& t, int x, int y) {

        t.addGPSPoint(g, g0 + GPSLocation(y,x));
        t.addGPSPoint(g, g0 + GPSLocation(y,x+1));
        t.addGPSPoint(g, g0 + GPSLocation(y+1,x));

        t.addGPSPoint(g, g0 + GPSLocation(y,x+1));
        t.addGPSPoint(g, g0 + GPSLocation(y+1,x+1));
        t.addGPSPoint(g, g0 + GPSLocation(y+1,x));

        t.createBuffers();
    };

    auto editPts = [g,g0](Tile& t, int x, int y) {

        t.editGPSPoint(0, g, g0 + GPSLocation(y,x));
        t.editGPSPoint(1, g, g0 + GPSLocation(y,x+1));
        t.editGPSPoint(2, g, g0 + GPSLocation(y+1,x));

        t.editGPSPoint(3, g, g0 + GPSLocation(y,x+1));
        t.editGPSPoint(4, g, g0 + GPSLocation(y+1,x+1));
        t.editGPSPoint(5, g, g0 + GPSLocation(y+1,x));

        t.updateBuffers();
    };

    for( int x=-tileCount; x <= tileCount; ++x)
    {
        for(int y = -tileCount; y <= tileCount; ++y)
        {
            const QString filename = QString("%1/%2%3/%2%3%4%5.png")
                    .arg(tilePath, (y+lat)>= 0?"n":"s")
                    .arg( qAbs(y+lat), 2, 10, QChar('0'))
                    .arg((x+lng) < 0 ?"w":"e")
                    .arg(qAbs(x+lng), 3, 10, QChar('0'));


            auto it = _tiles.find(filename);

            if( it == _tiles.end())
            {
#ifndef LITE
                if( (y+lat) < 60 && (y+lat) > -55) //SRTM Data has nothing above 60 LAT
                {
                    QImage img = QImage(filename);

                    if( img.isNull())
                        continue;

                    TileRetrieve tr;
                    TileRetrieve::Item item;
                    item.downloadState = TileRetrieve::downloadState::downloaded;
                    item.img = img;
                    tr.items.push_back(item);

                    _unrealisedTextures[filename] = tr;
                }
#endif

                Tile t;
                addPts(t,x, y);
                _tiles[filename] = std::move(t);
            }
            else
            {
                Tile& t = it->second;
                t.bRequired = true;
                editPts(t, x, y);
            }
        }
    }
}

void Flat3DSphere::renderTextures(float fHeight, bool dayLight)
{
    consumeUnrealisedTextures();

    _renderer->camID = 0;
    _renderer->useProgram( _multTexShader);

    OpenGLPipeline& pipeline = OpenGLPipeline::Get(_renderer->camID);
    pipeline.Push();
    pipeline.GetModel().LoadIdentity();
    pipeline.GetModel().Translate(0,-fHeight,0);

    pipeline.bindMatrices(_renderer->progId());
    _renderer->progId().sendUniform("Color", _mod.r, _mod.g, _mod.b, _mod.a);
    _renderer->progId().sendUniform("texture0", 0);
    _renderer->progId().sendUniform("texture2", 1);

    _renderer->bindTexture( dayLight? _texID->textureId() : _texNightID->textureId());

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<Tile*> nullTextures;

    for(auto&item:_tiles)
    {
        Tile& t = item.second;

        if( !t.bRequired)
            continue;

        if( t.tex == nullptr)
        {
            nullTextures.push_back(&t);
            continue;
         }

        t.bindVertex();
        _renderer->bindVertex(Renderer::Vertex, 3);

        t.bindTexture();
        _renderer->bindVertex(Renderer::TexCoord, 2);

        _defaultTextureBuffer.bind();
        _renderer->bindVertex(Renderer::TexCoord2, 2);

        _renderer->bindTexture(t.tex->textureId(), 1);

        _renderer->drawArrays(GL_TRIANGLES, t.getVertices().vertexSize());
    }

    if( !nullTextures.empty())
    {
        _renderer->camID = 0;
        _renderer->useProgram( _texshader);

        pipeline.bindMatrices(_renderer->progId());

        _renderer->progId().sendUniform("Color", _mod.r, _mod.g, _mod.b, _mod.a);
        _renderer->progId().sendUniform("texture0", 0);

        _renderer->bindTexture( dayLight? _texID->textureId() : _texNightID->textureId());

        for(auto& t:nullTextures)
        {
            t->bindVertex();
            _renderer->bindVertex(Renderer::Vertex, 3);

            t->bindTexture();
            _renderer->bindVertex(Renderer::TexCoord, 2);

            _renderer->drawArrays(GL_TRIANGLES, t->getVertices().vertexSize());
        }
    }

    _renderer->unBindBuffers();
    _texID->release();
    _texNightID->release();

    pipeline.Pop();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Flat3DSphere::renderMap(float fHeight)
{
    consumeUnrealisedTextures();

    _renderer->camID = 0;
    _renderer->useProgram(_texshader);

    OpenGLPipeline& pipeline = OpenGLPipeline::Get(_renderer->camID);
    pipeline.Push();
    pipeline.GetModel().LoadIdentity();
    pipeline.GetModel().Translate(0,-fHeight,0);

    pipeline.bindMatrices(_renderer->progId());
    _renderer->progId().sendUniform("Color", _mod.r, _mod.g, _mod.b, _mod.a);
    _renderer->progId().sendUniform("texture0", 0);

    for(auto&item:_tiles)
    {
        Tile& t = item.second;

        if( !t.bRequired)
            continue;

        if( t.tex == nullptr)
            continue;

        t.bindVertex();
        _renderer->bindVertex(Renderer::Vertex, 3);

        _defaultTextureBuffer.bind();
        _renderer->bindVertex(Renderer::TexCoord, 2);

        _renderer->bindTexture(t.tex->textureId());

        _renderer->drawArrays(GL_TRIANGLES, t.getVertices().vertexSize());
    }

    _renderer->unBindBuffers();
    pipeline.Pop();
}

void Flat3DSphere::resetTiles()
{
    _tiles.clear();
    _unrealisedTextures.clear();
    _bDirty = true;
}

void Flat3DSphere::setMod(float r, float g, float b, float a)
{
    _mod.r = r;
    _mod.g = g;
    _mod.b = b;
    _mod.a = a;
}

bool Flat3DSphere::getTileActive() const
{
    return _tileMapActive;
}

bool Flat3DSphere::getTileShowRunway() const
{
    return _tileMapShowRunway;
}

QString Flat3DSphere::getTileMapHost() const
{
    return _tileMapHost;
}

void Flat3DSphere::setTileActive(bool bActive)
{
    if( _tileMapActive == bActive)
        return;

    _tileMapActive = bActive;

    resetTiles();
}

void Flat3DSphere::setTileMapURL(size_t idx, QString url)
{
    if( idx < _tileMapURLs.size())
    {
        if( _tileMapURLs[idx] == url)
            return;
    }
    else
    {
        _tileMapURLs.resize(idx+1);
    }

    _tileMapURLs[idx] = url.trimmed();

    if( idx == 0)
        _tileMapHost = QUrl(url).host();

    resetTiles();
}

void Flat3DSphere::setTileShowZoom(bool show)
{
    if( _tileMapShowZoom == show)
        return;

    _tileMapShowZoom = show;

    resetTiles();
}

void Flat3DSphere::setTileShowRunway(bool show)
{
    _tileMapShowRunway = show;
}

void Flat3DSphere::setTileMapUser(QString user)
{
    _tileMapUser = user;
}

void Flat3DSphere::setTileMapPassword(QString password)
{
    _tileMapPassword = password;
}

bool Flat3DSphere::isZoomVisible(int zoom) const
{
    auto it = _zoomVisibility.find(zoom);
    if( it == _zoomVisibility.end())
        return true;

    return it->second;
}

void Flat3DSphere::setZoomVisibility(int zoom, bool visible)
{
    _zoomVisibility[zoom] = visible;
    _bDirty = true;
}

void Flat3DSphere::init()
{
    QSettings s;

    _strBuildDateTime = s.value(BuildVersion::BuildDateTime, "").toString();
    _userInfo = "ADSBFlightTracker ";
    _userInfo += _strBuildDateTime;
    _userInfo += " ";
    _userInfo += QSysInfo::prettyProductName();

#ifdef Q_OS_WIN
    qDebug() << "UserAgent : " << _userInfo;
#endif

    std::vector<float> uvs = std::vector<float>({0.0f, 1.0f,
                               1.0f, 1.0f,
                               0.0f, 0.0f,

                               1.0f, 1.0f,
                               1.0f, 0.0f,
                               0.0f, 0.0f});

    _defaultTextureBuffer.create();
    _defaultTextureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _defaultTextureBuffer.bind();
    _defaultTextureBuffer.allocate(uvs.data(), uvs.size()*sizeof(uvs[0]));
    _defaultTextureBuffer.release();
}

Flat3DSphere::Tile::Tile()
{
    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

void Flat3DSphere::Tile::addGPSPoint(const GPSLocation &origin, const GPSLocation &gps, float offsetY)
{
    Vector3F offset = origin.offSetTo(gps);
    _vertices.addVertex(offset.x, offsetY, offset.z);

    const auto& uv = toUV(gps);
    _vertices.addTexture(uv.first, uv.second);
}

void Flat3DSphere::Tile::editGPSPoint(size_t idx, const GPSLocation &origin, const GPSLocation &gps, float offsetY)
{
    Vector3F offset = origin.offSetTo(gps);
    offset.y = offsetY;
    _vertices.setVertex(idx, offset);

    const auto& uv = toUV(gps);
    _vertices.setTexture(idx, uv.first, uv.second );
}

void Flat3DSphere::Tile::createBuffers()
{
    _vertices.allocateVertex(_vertexBuffer);
    _vertices.allocateTexture(_textureBuffer);
}

void Flat3DSphere::Tile::updateBuffers()
{
    _vertices.bufferVertex(_vertexBuffer);
    _vertices.bufferTexture(_textureBuffer);
}

const meshData &Flat3DSphere::Tile::getVertices() const
{
    return _vertices;
}

std::pair<float, float> Flat3DSphere::Tile::toUV(const GPSLocation &loc)
{
    return std::make_pair( (loc._lng + 180)/360.0f, (loc._lat+90)/180.0f);
}

Flat3DSphere::TileRetrieve::downloadState Flat3DSphere::TileRetrieve::status() const
{
    if( items.empty())
        return downloadState::none;

    bool downloading = false;

    for(const auto&item : items)
    {
        if( item.downloadState ==downloadState::failed)
            return downloadState::failed;

        downloading |= item.downloadState == downloadState::downloading;
    }

    if( downloading)
        return downloadState::downloading;

    return items[0].downloadState;
}

QImage Flat3DSphere::TileRetrieve::getImage() const
{
    if( items.size() == 1)
        return items[0].img;

    std::vector<QImage> imageList;

    for(const auto& item:items)
        imageList.push_back(item.img);

    // Your logic to fill the imageList

    // Use std::find_if to find the first image with hasAlpha false
    auto it = std::find_if(imageList.begin(), imageList.end(), [](const QImage& image) {
        return !image.hasAlphaChannel();
    });

    // If an image with hasAlpha false is found, move it to the beginning of the list
    if (it != imageList.end())
        std::iter_swap(imageList.begin(), it);

    QImage resultImage;
    QPainter painter;

    bool initalised = false;

    for(const auto& image:imageList)
    {
        if( !initalised)
        {
            initalised = true;
            resultImage = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
            resultImage.fill(Qt::transparent);
            painter.begin(&resultImage);
        }

        painter.drawImage(0, 0, image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }

    return resultImage;
}
