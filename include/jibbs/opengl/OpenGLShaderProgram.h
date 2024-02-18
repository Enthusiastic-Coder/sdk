#pragma once

#include <unordered_map>
#include <jibbs/vector/vector3.h>
#include <jibbs/vector/vector4.h>
#include <jibbs/matrix/Matrix3x3.h>
#include <jibbs/matrix/Matrix4x4.h>
#include <QOpenGLFunctions>

class OpenGLShaderProgram : private QOpenGLFunctions
{
public:
	OpenGLShaderProgram();
	virtual ~OpenGLShaderProgram();

    OpenGLShaderProgram(const OpenGLShaderProgram& rhs) = delete;
    OpenGLShaderProgram& operator=(const OpenGLShaderProgram& rhs) = delete;

	void clear();
    bool loadFiles(const std::string &vertex, const std::string &fragment);
    bool loadSrc(const std::string &vertex, std::string fragment);

	void sendUniform(const std::string& name, int value);
	void sendUniform(const std::string& name, float value);
    void sendUniform(const std::string& name, float x, float y);
	void sendUniform(const std::string& name, float x, float y, float z);
    void sendUniform(const std::string& name, float x, float y, float z, float w);
    void sendUniform(const std::string& name, const Vector3F& v);
    void sendUniform(const std::string& name, const Vector4F& v);
    void sendUniform(const std::string& name, const Matrix3x3F& m);
    void sendUniform(const std::string& name, const Matrix4x4F& m);

	GLint getUniformLocation(const std::string& name);
    GLint getAttribLocation(const std::string& name);

    unsigned int getProgramID();
    unsigned int getVertexShaderID();
    unsigned int getFragmentShaderID();

	operator GLuint();

    void use();
	static void useDefault();

	std::string getError() const;
	std::string getVertexName() const;
	std::string getFragmentName() const;
protected:
    GLuint createShader(GLenum type, const char *charSource);
    std::string loadCode(const std::string & filename);

private:
    GLuint _programShaderID;
    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;
	std::string _error;
	std::string _vertexName;
	std::string _fragmentName;
    std::unordered_map<std::string, GLint> _uniforms;
    std::unordered_map<std::string, GLint> _attribs;
    bool _isGLES = true; //Must be TRUE for Android
};
