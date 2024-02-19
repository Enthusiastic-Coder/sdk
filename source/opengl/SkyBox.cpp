
#include <jibbs/utilities/stdafx.h>
#include <jibbs/mesh/MeshData.h>
#include <jibbs/opengl/OpenGLRenderer.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/opengl/SkyBox.h>

#include <QDebug>


SkyBox::SkyBox() : _texManager("SkyBox")
{
}

void SkyBox::initCube()
{
    const float d = 50.0f;

    meshData data;

    auto addTex = [&data]{
        data.addTexture(1,1);
        data.addTexture(0,1);
        data.addTexture(1,0);
        data.addTexture(0,1);
        data.addTexture(0,0);
        data.addTexture(1,0);
    };

    ///LEFT
    data.addVertex(-d, -d, -d);
    data.addVertex(-d, -d, d );
    data.addVertex(-d, d, -d);

    data.addVertex(-d, -d, d);
    data.addVertex(-d, d, d);
    data.addVertex(-d, d, -d);

    addTex();

    ///RIGHT
    data.addVertex(d, -d, d);
    data.addVertex(d, -d, -d);
    data.addVertex(d, d, d);

    data.addVertex(d, -d, -d);
    data.addVertex(d, d, -d);
    data.addVertex(d, d, d);

    addTex();

    ///TOP
    data.addVertex(-d, d, d);
    data.addVertex(d, d, d);
    data.addVertex(-d, d, -d);

    data.addVertex(d, d, d);
    data.addVertex(d, d, -d);
    data.addVertex(-d, d, -d);

    addTex();

    ///BOTTOM
    data.addVertex(-d, -d, -d);
    data.addVertex(d, -d, -d);
    data.addVertex(-d, -d, d);

    data.addVertex(d, -d, -d);
    data.addVertex(d, -d, d);
    data.addVertex(-d, -d, d);

    addTex();

    ///FRONT
    data.addVertex(-d, -d, d);
    data.addVertex(d, -d, d);
    data.addVertex(-d, d, d);

    data.addVertex(d, -d, d);
    data.addVertex(d, d, d);
    data.addVertex(-d, d, d);

    addTex();

    ///BACK
    data.addVertex(d, -d, -d);
    data.addVertex(-d, -d, -d);
    data.addVertex(d, d, -d);

    data.addVertex(-d, -d, -d);
    data.addVertex(-d, d, -d);
    data.addVertex(d, d, -d);

    addTex();

    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    _textureBuffer.create();
    _textureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    data.allocateVertex(_vertexBuffer);
    data.allocateTexture(_textureBuffer);
}

void SkyBox::init()
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    _texManager.init();
    initCube();

    if( !_skyLineProgram.loadSrc(readAllRP("shaders/skyLineShader.vert"), readAllRP("shaders/skyLineShader.frag")))
        qDebug() << QString::fromStdString(_skyLineProgram.getError());

    _skyDome.reset(new SkyDome(50, 20, 25, true));
}

void SkyBox::addCube(const QString& folder, bool isDay, QRgb brightness)
{
    QString fullFolder = ResourcePath("skyboxes/%1/").arg(folder);

    TexturesInfo t;

    t.filenames[0] = fullFolder + "left.jpg";
    t.filenames[1] = fullFolder + "right.jpg";
    t.filenames[2] = fullFolder + "top.jpg";
    t.filenames[3] = fullFolder + "bottom.jpg";
    t.filenames[4] = fullFolder + "front.jpg";
    t.filenames[5] = fullFolder + "back.jpg";
    t.brightness = brightness;
    t.isDay = isDay;
    t.isCube = true;
    t.id = folder;

    _cubes[folder] = t;
    _Ids << folder;
}

void SkyBox::addDome(const QString& name, const QString &filename, bool isDay, QRgb brightness)
{
    TexturesInfo t;

    t.filenames[0] = filename;
    t.id = name;
    t.isCube = false;
    t.brightness = brightness;
    t.isDay = isDay;
    _cubes[name] = t;

    _Ids << name;
}

bool SkyBox::selectId(const QString &folder)
{
    QHash<QString,TexturesInfo>::Iterator it;

    if( folder.isEmpty())
    {
        if( _Ids.length() > 0)
            it = _cubes.find(_Ids[0]);
    }
    else
    {
        it = _cubes.find(folder);
        if( it == _cubes.end())
            return false;
    }

    auto itUnload = _cubes.find(_current.id);

    if( itUnload != _cubes.end())
        unLoadCubeTextures(*itUnload);

    _current = it.value();
    loadCubeTextures(_current);
    return true;
}

int SkyBox::rBrightness() const
{
    return qRed(_current.brightness);
}

int SkyBox::gBrightness() const
{
    return qGreen(_current.brightness);
}

int SkyBox::bBrightness() const
{
    return qBlue(_current.brightness);
}

bool SkyBox::isDayTime() const
{
    return _current.isDay;
}

void SkyBox::Render(Renderer *r, OpenGLPipeline &pipe)
{
    r->useProgram(_skyLineProgram);
    _skyLineProgram.sendUniform("texID", 0);
    pipe.bindMatrices(_skyLineProgram);

    const TexturesInfo& t = _current;

    if( t.isCube)
    {
        _skyLineProgram.sendUniform("textColor", 1,1,1,1);
        renderCube(r, t);
    }
    else
    {
        r->bindTexture(t.ids[0]->textureId());

        if( t.isDay )
            _skyLineProgram.sendUniform("textColor", 1,1,1,1);
        else
            _skyLineProgram.sendUniform("textColor", 0.25f,0.25f,0.25f,1);

        _skyDome->Render(r);
    }
}

void SkyBox::renderCube(Renderer *r, const TexturesInfo& t)
{
    _vertexBuffer.bind();
    r->bindVertex(Renderer::Vertex, 3);

    _textureBuffer.bind();
    r->bindVertex(Renderer::TexCoord, 2);

    for(int i =0; i < 6; ++i)
    {
        r->bindTexture(t.ids[i]->textureId());
        r->drawArrays(GL_TRIANGLES, 6, i*6);
    }

    _vertexBuffer.release();
    _textureBuffer.release();
    r->unBindBuffers();
    glBindTexture(GL_TEXTURE_2D, 0);
}

const QStringList &SkyBox::getIds() const
{
    return _Ids;
}

void SkyBox::loadCubeTextures(TexturesInfo &t)
{
    for( int i=0; i < 6; ++i)
        t.ids[i] = loadTexture(t.filenames[i]);
}

void SkyBox::unLoadCubeTextures(TexturesInfo &t)
{
    for( int i=0; i < 6; ++i)
        t.ids[i].reset();
}

std::shared_ptr<QOpenGLTexture> SkyBox::loadTexture(const QString &file)
{
    auto id = _texManager.peekTexture(file);

    if( id == nullptr)
        id = _texManager.loadTexture(file, QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::Linear);

#ifdef Q_OS_WIN
    if( id)
        qDebug() << "SkyDOME : " << file << " -> " << id->textureId();
#endif
    return id;
}

