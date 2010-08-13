/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#ifndef GAMEFW_H
#define GAMEFW_H

#include "../common.h"

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

/**
 * Checks if OpenGL has generated an error.
 * 
 * @return true if an error has been generated.
 */
inline bool checkOGLError()
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
            DLOG(ERROR) << "OpenGL Error(%s): %s\n" << s.c_str();
        }
        return error == GL_NO_ERROR;
    }

#include <bullet/btBulletDynamicsCommon.h>

#include "game.h"
#include "gamestate.h"
#include "rendercontext.h"
#include "shaderfactory.h"
#include "shaderprogram.h"
#include "renderjob.h"
#include "entity.h"
#include "entityfactory.h"
#include "fileservice.h"
#include "locator.h"

#endif
