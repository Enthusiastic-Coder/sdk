#ifndef FLAT3DSPERE_H
#define FLAT3DSPERE_H

#include <jibbs/gps/GPSLocation.h>
#include <jibbs/math/euler.h>
#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/opengl/QtTextureManager.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/mesh/MeshData.h>
#include <jibbs/utilities/TextureCacheManager.h>

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QTimer>

#include <memory>
#include <unordered_map>

#include "color3.h"

class Flat3DSphere : public QObject
{
    Q_OBJECT
public:
    Flat3DSphere();

    void init();

    void render(const GPSLocation &g, bool dayLight);

    void setDirty();
    void setCamera(EulerF cam);
    void setTextureMinMag(QOpenGLTexture::Filter minification, QOpenGLTexture::Filter magnification);
    void setMod(float r, float g, float b, float a);

    bool getTileActive() const;
    bool getTileShowRunway() const;
    QString getTileMapHost() const;

    void setTileActive(bool bActive);
    void setTileMapURL(size_t idx, QString url);
    void setTileShowZoom(bool show);
    void setTileShowRunway(bool show);
    void setTileMapUser(QString user);
    void setTileMapPassword(QString password);

    bool isZoomVisible(int zoom) const;
    void setZoomVisibility(int zoom, bool visible);

private:
    void resetTiles();
    void requestZoomTile(int zoom, const QString &tileId, QColor color);
    void requestTile(int zoom, int tileX, int tileY);
    bool retryTile(int zoom, int tileX, int tileY);
    void downloadTile(const QString& finalURL, const QString& tileId, int idx );
    void BuildOpenStreetMapTile(const GPSLocation &g);
    void BuildGroundTile(const GPSLocation &g);
    void renderTextures(float fHeight, bool dayLight);
    void renderMap(float fHeight);

    void releaseAllTextures();
    void updateTexture();
    void removeUnusedTextures();
    void consumeUnrealisedTextures();

private:

    QNetworkAccessManager* _networkAccessManager = nullptr;
    QNetworkCookieJar cookieJar;

    bool _bDirty = false;
    QTimer* _timer =nullptr;
    OpenGLShaderProgram _multTexShader;
    OpenGLShaderProgram _texshader;
    std::unique_ptr<QOpenGLTexture> _texID;
    std::unique_ptr<QOpenGLTexture> _texNightID;
    std::unique_ptr<Renderer> _renderer;
    QOpenGLTexture::Filter _minification = QOpenGLTexture::LinearMipMapNearest;
    QOpenGLTexture::Filter _magnification = QOpenGLTexture::Linear;
    color3 _mod;
    EulerF _cam;

    QString _strBuildDateTime;
    QString _userInfo;

    struct Tile
    {
        Tile();
        bool bRequired=true;
        std::shared_ptr<QOpenGLTexture> tex;
        void addGPSPoint(const GPSLocation& origin, const GPSLocation& gps, float offsetY=0);
        void editGPSPoint(size_t idx, const GPSLocation& origin, const GPSLocation& gps, float offsetY=0);

        void createBuffers();
        void updateBuffers();

        void bindVertex() {
            _vertexBuffer.bind();
        }

        void bindTexture() {
            _textureBuffer.bind();
        }

        const meshData& getVertices() const;

    private:
        meshData _vertices;
        QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        QOpenGLBuffer _textureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    private:
        std::pair<float, float> toUV(const GPSLocation &loc);
    };

    struct TileRetrieve
    {
        enum class downloadState {
            none,
            downloading,
            failed,
            downloaded,
            downloadedUnavailable
        };

        struct Item {
            downloadState downloadState = downloadState::none;
            QString tileUrl;
            QImage img;
        };

        downloadState status() const;
        QImage getImage() const;

        bool dirty = true;
        int level = -1;
        bool showZoom = false;
        std::vector<Item> items;
    };

    int _minNumberToDelete = 1;
    std::vector<QString> _tileMapURLs;
    QString _tileMapHost;
    bool _tileMapShowZoom = false;
    bool _tileMapActive = false;
    bool _tileMapShowRunway = true;
    QString _tileMapUser;
    QString _tileMapPassword;

    TextureCacheManager _textureCacheManager;

    std::unordered_map<int,bool> _zoomVisibility;
    std::unordered_map<QString, Tile> _tiles;
    std::unordered_map<QString, TileRetrieve> _unrealisedTextures;

    QOpenGLBuffer _defaultTextureBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
};

#endif // FLAT3DSPERE_H
