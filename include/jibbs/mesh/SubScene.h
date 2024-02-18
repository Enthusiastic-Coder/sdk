#ifndef SUBSCENE_H
#define SUBSCENE_H

#include <jibbs/boundary/BoundingBox.h>
#include <vector>
#include <functional>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include <OpenGLShaderProgram.h>
#include <OpenGLPipeline.h>
#include <vertexData.h>
#include "pivotData.h"


class SubScene : private QOpenGLFunctions
{
public:
    SubScene(std::string name, std::shared_ptr<std::vector<vertexData> > &vd, std::shared_ptr<std::vector<unsigned short> > &id, QString tex);

    void draw(OpenGLShaderProgram &program, GLuint texId);
    const std::string& name() const;

    bool isWheel() const;
    bool isStrobe() const;
    bool isStrobeOffset() const;
    bool isStrobeR() const;
    bool isStrobeO() const;
    bool isStrobeB() const;
    QString texFilename() const;

    bool hasPivot() const;
    bool applyPivot(OpenGLMatrixStack& mat, pivotData &pivotData) const;

    void setPivotHandler(std::function<void(OpenGLMatrixStack &, pivotData&)> handler);

    void finalise();

private:
    std::string _name;
    QOpenGLBuffer _vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer _indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    QString _texFilename;
    bool _isWheel = false;
    bool _isStrobe = false;
    bool _isStrobeOffset = false;
    char _strobeLetter;
    std::function<void(OpenGLMatrixStack& mat, pivotData&)> _pivot = nullptr;
    std::shared_ptr<std::vector<vertexData>> _vd;
    std::shared_ptr<std::vector<unsigned short>> _id;
};

#endif // SUBSCENE_H
