#ifndef OGL_H
#define OGL_H

#ifdef WIN32
#    define GLEW_STATIC
#    include <GL/glew.h>
//#    include <GL/wglew.h>
#    define glfGetProcAddress wglGetProcAddress
#elif defined(linux) || defined(__linux)
#    include <GL/glew.h>
#    define GL_GLEXT_PROTOTYPES 1
#    include <GL/gl.h>
#    include <GL/glext.h>
#else
#    error "Unsupported platform"
#endif

#endif // OGL_H