#include "SubScene.h"
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

SubScene::SubScene(std::string name, std::shared_ptr<std::vector<vertexData>>& vd, std::shared_ptr<std::vector<unsigned short>>& id, QString tex)
{
    _name = name;
    _isWheel = name.substr(0, 5) == "WHEEL";

    _isStrobe = name.substr(0, 6) == "STROBE" && name.length() > 7;

    if(_isStrobe)
    {
        _strobeLetter = name[6];
        _isStrobeOffset = name[7] =='O';
    }

    _texFilename = tex;
    _vd = vd;
    _id = id;
}


void SubScene::finalise()
{
    if( !_vd || !_id )
        return;

    if( _vd->empty() || _id->empty())
        return;

    QOpenGLFunctions::initializeOpenGLFunctions();

    _vertexBuffer.create();
    _vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    _vertexBuffer.bind();
    _vertexBuffer.allocate(_vd->data(), _vd->size()*sizeof(vertexData));
    _vertexBuffer.release();

    _indexBuffer.create();
    _indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    _indexBuffer.bind();
    _indexBuffer.allocate(_id->data(), _id->size()*sizeof(unsigned short));
    _indexBuffer.release();

    _vd.reset();
    _id.reset();
}

const std::string &SubScene::name() const
{
    return _name;
}

bool SubScene::isWheel() const
{
    return _isWheel;
}

bool SubScene::isStrobe() const
{
    return _isStrobe;
}

bool SubScene::isStrobeOffset() const
{
    return _isStrobeOffset;
}

bool SubScene::isStrobeR() const
{
    return _strobeLetter == 'R';
}

bool SubScene::isStrobeO() const
{
    return _strobeLetter == 'O';
}

bool SubScene::isStrobeB() const
{
    return _strobeLetter == 'B';
}

QString SubScene::texFilename() const
{
    return _texFilename;
}

bool SubScene::hasPivot() const
{
    return _pivot != nullptr;
}

bool SubScene::applyPivot(OpenGLMatrixStack &mat, pivotData& pd) const
{
    if( _pivot == nullptr)
        return false;

    if( _isWheel && pd.derivedSpeed < 0.5f)
        return false;

    return static_cast<void>(_pivot(mat, pd)), true;
}

void SubScene::setPivotHandler(std::function<void(OpenGLMatrixStack &, pivotData&)> handler)
{
    _pivot = handler;
}

void SubScene::draw(OpenGLShaderProgram &program, GLuint texId)
{
    //attribute vec3 vertex
    const int vertex = program.getAttribLocation("vertex");
    const int normal = program.getAttribLocation("normal");
    const int tangent = program.getAttribLocation( "tangent");
    const int color = program.getAttribLocation( "color");
    const int UV = program.getAttribLocation("texture");

    //texture0
    //texture1...
    //std::string str="texture";
    //for(size_t i=0;i<_textures.size();i++)
    if( texId != -1)
    {
        glActiveTexture(GL_TEXTURE0+0);
        glBindTexture(GL_TEXTURE_2D, texId);

        int texture0 = program.getUniformLocation("texture0");
        glUniform1i(texture0,0);
    }

    _vertexBuffer.bind();

    if( vertex >= 0)
    {
        glEnableVertexAttribArray(vertex);
        glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);
    }

    if( normal >= 0)
    {
        glEnableVertexAttribArray(normal);
        glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));
    }

    if( tangent >= 0)
    {
        glEnableVertexAttribArray(tangent);
        glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));
    }

    if( color >= 0)
    {
        glEnableVertexAttribArray(color);
        glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));
    }

    if( UV >= 0)
    {
        glEnableVertexAttribArray(UV);
        glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));
    }

    _indexBuffer.bind();
    glDrawElements(GL_TRIANGLES,_indexBuffer.size()/sizeof(unsigned short) ,GL_UNSIGNED_SHORT,0);

    if(vertex >= 0)    glDisableVertexAttribArray(vertex);
    if(normal >= 0)    glDisableVertexAttribArray(normal);
    if(tangent >= 0)   glDisableVertexAttribArray(tangent);
    if(color >= 0)    glDisableVertexAttribArray(color);
    if(UV >= 0)    glDisableVertexAttribArray(UV);

    _vertexBuffer.release();
    _indexBuffer.release();
}
