#pragma once
#include <jibbs/matrix/Matrix4x4.h>
#include <jibbs/vector/vector3.h>
#include <jibbs/vector/vector4.h>
#include <jibbs/gps/GPSLocation.h>
#include <jibbs/math/euler.h>
#include <stack>

#include "OpenGLShaderProgram.h"

class OpenGLMatrixStack
{
public:

    enum MatrixType { None, View, Model, Projection };

    OpenGLMatrixStack(MatrixType type);

    const Matrix4x4F& Top() const;
    Matrix4x4F& Top();
    void Push();
    void Pop();
    void LookAt(const Vector3F& eye, const Vector3F& center, Vector3F up = Vector3F(0, 1, 0));
    void LookAt(const GPSLocation& eye, const GPSLocation& center, Vector3F up = Vector3F(0, 1, 0));
    void SetPerspective(float fovy, float aspect, float zNear, float zFar);
    void SetOrthographic(float zLeft, float zRight, float zTop, float zBottom, float zNear, float zFar);
    void ApplyShadowModelMatrix(const GPSLocation& loc, float heightAbovePlane, const Vector4F& lightDir, const QuarternionF& qPlane);
    void LoadIdentity();
    void Load(const Matrix4x4F& mat);
    void PreScale(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z);
    template<typename U>
    void Rotate(const Euler<U>& e)
    {
        Rotate(e._pitch, e._heading, e._bank);
    }

    template<typename U>
    void RotateNoBank(const Euler<U>& e)
    {
        Rotate(-e._pitch, e._heading, 0);
    }

    void TranslateLocation(const GPSLocation& loc, bool ignoreHeight=false);

    template<typename U>
    void Rotate(const Vector3<U> v)
    {
        Rotate(v.x, v.y, v.z);
    }

    template<typename U>
    void Translate(const Vector3<U> v)
    {
        Translate(v.x, v.y, v.z);
    }

private:
    std::stack<Matrix4x4F> _OpenGLMatrix;
    MatrixType _Type;
};

class OpenGLPipeline
{
public:
    OpenGLPipeline();

    static void applyScreenProjection(OpenGLPipeline &p, int x, int y, int cx, int cy);
    static OpenGLPipeline& Get(unsigned int camID);
    void Push();
    static void ApplyMaterialToShader(OpenGLShaderProgram &progID,
                                      Vector4F ambient,
                                      Vector4F diffuse,
                                      Vector4F specualar,
                                      float shininess);
    static void ApplyLightToShader(OpenGLShaderProgram& progID,
                                   Vector4F ambient,
                                   Vector4F diffuse,
                                   Vector4F specualar,
                                   float shininess);

    static int GetVertexAttribId(OpenGLShaderProgram& progID);
    static int GetColorAttribId(OpenGLShaderProgram& progID);
    static int GetNormalAttribId(OpenGLShaderProgram& progID);
    static int GetTextureAttribId(OpenGLShaderProgram& progID);
    static int GetTexture2AttribId(OpenGLShaderProgram& progID);

    static void bindMatrices(OpenGLShaderProgram& progID,
        const Matrix4x4F& MV,const Matrix4x4F& MVP,const Matrix4x4F& P,const Matrix3x3F& N);

    void bindMatrices(OpenGLShaderProgram& progID, bool bFustrum=false);
    void updateFustrum();
    void ApplyLightBPMV(OpenGLShaderProgram &progID);

    void Pop();
    void setLocation(GPSLocation loc);
    void setViewZoom(float zoom);
    const Frustum& GetFrustum() const;
    OpenGLMatrixStack& GetProjection();
    OpenGLMatrixStack& GetModel();
    OpenGLMatrixStack& GetView();
    Matrix4x4F GetModelView() const;
    Matrix4x4F GetMVP() const;
    Matrix4x4F GetBMVP();
    Matrix3x3F GetNormal();
    unsigned int GetCamID() const;

protected:
    void updateFustrumHelper(const Matrix4x4F &MV, const Matrix4x4F &MVP);

private:
    std::stack<Frustum>	_Frustum;
    OpenGLMatrixStack _p;
    OpenGLMatrixStack _m;
    OpenGLMatrixStack _v;
    Matrix4x4F _zoom;
    unsigned int _camID = 0;
    GPSLocation _location;
};
