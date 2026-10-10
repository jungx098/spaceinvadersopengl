/* The game includes GLUT and does not call it. Pull in gl.h so later WGL headers see GL types. */
#ifndef PLATFORM_MACOS_GL_GLUT_H
#define PLATFORM_MACOS_GL_GLUT_H
#include <gl/gl.h>
#include <gl/glu.h>
#endif
