/**
 * gl_includes.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_GL_INCLUDES_HPP
#define GRAPHICSLAB_GL_INCLUDES_HPP

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif


#ifdef WIN32
// Win32
#include <GL/glew.h>

#include <windows.h>
#include <glut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>

#else 
// MacOS X
#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else 
// UNIX, Linux

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#endif

#endif // GRAPHICSLAB_GL_INCLUDES_HPP


