
#ifndef GAMEFW_H
#define GAMEFW_H

// OpenGL
#ifdef WIN32
#	define GLEW_STATIC 
#	include <GL/glew.h>
//#	include <GL/wglew.h>
#	define glfGetProcAddress wglGetProcAddress
#elif defined(linux) || defined(__linux)
#	include <GL/glew.h>
#	define GL_GLEXT_PROTOTYPES 1
#	include <GL/gl.h>
#	include <GL/glext.h>
#else
#	error "Unsupported platform"
#endif

#include "../common.h"

/**
 * @brief Exception thrown by checkOpenGLError when there's an OpenGL internal error.
 */
class OpenGLError: public exception {};

/**
 * Checks if OpenGL has generated an error.
 * 
 * @throw OpenGLError When an error has occured.
 */
inline void checkOpenGLError()
{
    int error;
    if((error = glGetError()) != GL_NO_ERROR)
    {
        string s;
        switch(error)
        {
        case GL_INVALID_ENUM:
            s = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            s = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            s = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            s = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            s = "GL_OUT_OF_MEMORY";
            break;
        default:
            s = "UNKNOWN";
            break;
        }
        LOG(logERROR) << "OpenGL Error: " << s.c_str();
        throw OpenGLError();
    }
}

#include "game.h"
#include "gamestate.h"
#include "renderer.h"
#include "shaderfactory.h"
#include "shaderprogram.h"
#include "renderjob.h"
#include "entity.h"
#include "entityfactory.h"
#include "fileservice.h"
#include "locator.h"

#endif
