#include "OpenGLPipeline.h"
#include "OpenGLShaderProgram.h"

OpenGLMatrixStack::OpenGLMatrixStack(MatrixType type) :
    _Type(type)
{
    _OpenGLMatrix.push(Matrix4x4F());
    _OpenGLMatrix.top().LoadIdentity();
}

const Matrix4x4F &OpenGLMatrixStack::Top() const
{
    return _OpenGLMatrix.top();
}

Matrix4x4F &OpenGLMatrixStack::Top()
{
    return _OpenGLMatrix.top();
}

void OpenGLMatrixStack::Push()
{
    _OpenGLMatrix.push(_OpenGLMatrix.top());
}

void OpenGLMatrixStack::Pop()
{
    _OpenGLMatrix.pop();
}

void OpenGLMatrixStack::LookAt(const Vector3F &eye, const Vector3F &center, Vector3F up)
{
    Top().LookAt(eye, center, up);
}

void OpenGLMatrixStack::LookAt(const GPSLocation& eye, const GPSLocation& center, Vector3F up)
{
    Top().LookAt(eye, center, up);
}

void OpenGLMatrixStack::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
    Top().SetPerspective(fovy, aspect, zNear, zFar);
}

void OpenGLMatrixStack::SetOrthographic(float zLeft, float zRight, float zTop, float zBottom, float zNear, float zFar)
{
    Top().SetOrthographic(zLeft, zRight, zTop, zBottom, zNear, zFar);
}

void OpenGLMatrixStack::ApplyShadowModelMatrix(const GPSLocation& loc, float heightAbovePlane, const Vector4F &lightDir, const QuarternionF &qPlane)
{
    Top().ApplyShadowModelMatrix(loc, heightAbovePlane, lightDir, qPlane);
}

void OpenGLMatrixStack::LoadIdentity()
{
    Top().LoadIdentity();
}

void OpenGLMatrixStack::Load(const Matrix4x4F &mat)
{
    Top() = mat;
}

void OpenGLMatrixStack::PreScale(float x, float y, float z)
{
    if (_Type == View)
        Top().PreScale(1.0f / x, 1.0f / y, 1.0f / z);
    else
        Top().PreScale(x, y, z);
}

void OpenGLMatrixStack::Scale(float x, float y, float z)
{
    if (_Type == View)
        Top().Scale(1.0f / x, 1.0f / y, 1.0f / z);
    else
        Top().Scale(x, y, z);
}

void OpenGLMatrixStack::Translate(float x, float y, float z)
{
    if (_Type == View)
        Top().Translate(-x, -y, -z);
    else
        Top().Translate(x, y, z);
}

void OpenGLMatrixStack::Rotate(float x, float y, float z)
{
    if (_Type == View)
        Top().RotateView(x, y, z);
    else
        Top().RotateModel(x, y, z);
}

void OpenGLMatrixStack::TranslateLocation(const GPSLocation& loc,bool ignoreHeight)
{
    if (_Type == View)
        Top().ApplyViewMatrix(loc, ignoreHeight);
    else
        Top().ApplyModelMatrix(loc, ignoreHeight);
}

OpenGLPipeline::OpenGLPipeline() :
    _p(OpenGLMatrixStack::Projection),
    _m(OpenGLMatrixStack::Model),
    _v(OpenGLMatrixStack::View)
{
    _Frustum.push(Frustum());
    _zoom.LoadIdentity();
}

void OpenGLPipeline::applyScreenProjection(OpenGLPipeline& p, int x, int y, int cx, int cy)
{
    p.GetProjection().LoadIdentity();
    p.GetProjection().SetOrthographic(x, x+cx, y+cy, y , -1, 1);
    p.GetModel().LoadIdentity();
    p.GetView().LoadIdentity();
}

OpenGLPipeline& OpenGLPipeline::Get(unsigned int camID)
{
    static std::map<unsigned int, OpenGLPipeline> cameras;
    static int lastcamID = -1;
    static OpenGLPipeline* lastPipeline = nullptr;

    if (lastcamID != camID)
    {
        lastcamID = camID;
        OpenGLPipeline& pipeline = cameras[camID];
        pipeline._camID = camID;
        lastPipeline = &pipeline;
    }

    return *lastPipeline;
}

void OpenGLPipeline::Push()
{
    _m.Push();
    _v.Push();
    _Frustum.push(Frustum());
}


void OpenGLPipeline::ApplyMaterialToShader(OpenGLShaderProgram& progID, Vector4F ambient,Vector4F diffuse,Vector4F specualar,float shininess)
{
    progID.sendUniform("material.ambient", ambient);
    progID.sendUniform("material.diffuse", diffuse);
    progID.sendUniform("material.specular", specualar);
    progID.sendUniform("material.shininess", shininess);
}

void OpenGLPipeline::ApplyLightToShader(OpenGLShaderProgram &progID, Vector4F ambient, Vector4F diffuse, Vector4F specualar, float shininess)
{
    progID.sendUniform("light0.ambient", ambient);
    progID.sendUniform("light0.diffuse", diffuse);
    progID.sendUniform("light0.specular", specualar);
    progID.sendUniform("light0.shininess", shininess);
}

int OpenGLPipeline::GetVertexAttribId(OpenGLShaderProgram &progID)
{
    return (int)progID.getAttribLocation("vertex");
}

int OpenGLPipeline::GetColorAttribId(OpenGLShaderProgram &progID)
{
    return (int)progID.getAttribLocation("color");
}

int OpenGLPipeline::GetNormalAttribId(OpenGLShaderProgram &progID)
{
    return (int)progID.getAttribLocation("normal");
}

int OpenGLPipeline::GetTextureAttribId(OpenGLShaderProgram &progID)
{
    return (int)progID.getAttribLocation("texture");
}

int OpenGLPipeline::GetTexture2AttribId(OpenGLShaderProgram &progID)
{
    return (int)progID.getAttribLocation("texture2");
}

void OpenGLPipeline::bindMatrices(OpenGLShaderProgram &progID,
                                  const Matrix4x4F &MV,
                                  const Matrix4x4F &MVP,
                                  const Matrix4x4F &P,
                                  const Matrix3x3F &N)
{
    progID.sendUniform("modelViewMatrix", MV);
    progID.sendUniform("modelViewProjectionMatrix", MVP);
    progID.sendUniform("projectionMatrix", P);
    progID.sendUniform("normalMatrix", N);
}

void OpenGLPipeline::bindMatrices(OpenGLShaderProgram& progID , bool bFustrum)
{
    const Matrix4x4F& MV = GetModelView();
    const Matrix4x4F& MVP = GetProjection().Top() * MV;

    if(bFustrum)
        updateFustrumHelper(MV, MVP);

    bindMatrices(progID, MV, MVP, GetProjection().Top(), GetNormal());
}

void OpenGLPipeline::updateFustrum()
{
    const Matrix4x4F& MV = GetModelView();
    const Matrix4x4F& MVP = GetProjection().Top() * MV;

    updateFustrumHelper(MV, MVP);
}

void OpenGLPipeline::updateFustrumHelper(const Matrix4x4F& MV, const Matrix4x4F& MVP)
{
    Frustum& frustum = _Frustum.top();

    for (int i = Frustum::Left; i < Frustum::Frustrum_Plane_Count; ++i)
        MVP.ExtractPlane(frustum, i);
}

void OpenGLPipeline::ApplyLightBPMV(OpenGLShaderProgram& progID)
{
    char buf[64];
    sprintf(buf, "lightModelViewProjectionMatrix%d", _camID);
    progID.sendUniform(buf, GetBMVP());
}

void OpenGLPipeline::Pop()
{
    _Frustum.pop();
    _m.Pop();
    _v.Pop();
}

void OpenGLPipeline::setLocation(GPSLocation loc)
{
    _location = loc;
}

void OpenGLPipeline::setViewZoom(float zoom)
{
    _zoom.m33 = 1.0f / zoom;
    GetView().Scale(1.0f, 1.0f, zoom);
}

const Frustum& OpenGLPipeline::GetFrustum() const
{
    return _Frustum.top();
}

OpenGLMatrixStack& OpenGLPipeline::GetProjection()
{
    return _p;
}

OpenGLMatrixStack& OpenGLPipeline::GetModel()
{
    return _m;
}

OpenGLMatrixStack& OpenGLPipeline::GetView()
{
    return _v;
}

Matrix4x4F OpenGLPipeline::GetModelView() const
{
    return _v.Top() * _m.Top();
}

Matrix4x4F OpenGLPipeline::GetMVP() const
{
    return _p.Top() * GetModelView();
}

Matrix4x4F OpenGLPipeline::GetBMVP()
{
    return (BiasMatrix4x4F::get() * _p.Top() * GetModelView());

}

Matrix3x3F OpenGLPipeline::GetNormal()
{
    return _zoom * GetModelView().Inverse().Transpose();
}

unsigned int OpenGLPipeline::GetCamID() const
{
    return _camID;
}

