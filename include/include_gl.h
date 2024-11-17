#ifndef INCLUDE_GL_H
#define INCLUDE_GL_H

#ifdef IS_QT_ANDROID
#include <QOpenGLFunctions>
#elif IS_SDL_ANDROID
#include <SDL_opengles2.h>
#elif ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif WIN32
#include "GLee.h"
#include "wglext.h"
#pragma comment(lib, "opengl32.lib")
#else
#error Unknown Platform
#endif

#endif // INCLUDE_GL_H
