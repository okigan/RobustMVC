#include "StdAfx.h"
#include "Visualization.h"

#include <stdlib.h>
#include <stdio.h>

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")


Visualization::Visualization(void)
{
}


Visualization::~Visualization(void)
{
}


void
    display()
{
    /* rotate a triangle around */
    //glClear(GL_COLOR_BUFFER_BIT);
    //glBegin(GL_TRIANGLES);
    //glColor3f(1.0f, 0.0f, 0.0f);    glVertex2i(0,  1);
    //glColor3f(0.0f, 1.0f, 0.0f);    glVertex2i(-1, -1);
    //glColor3f(0.0f, 0.0f, 1.0f);    glVertex2i(1, -1);
    //glEnd();

    glBegin(GL_QUADS);
    {
        glColor3f(0.0f, 0.0f, 0.0f);   glVertex2i(0,  0);
        glColor3f(1.0f, 0.0f, 0.0f);   glVertex2i(1,  0);
        glColor3f(1.0f, 1.0f, 0.0f);   glVertex2i(1,  1);
        glColor3f(0.0f, 1.0f, 0.0f);   glVertex2i(0,  1);
    }
    glEnd();

}

GLuint program = 0;
void checkError(GLint status, const char *msg)
{
    if (!status)
    {
        printf("%s\n", msg);
        exit(EXIT_FAILURE);
    }
}

GLuint InitShader(const GLchar *vSource , const GLchar* fSource)
{
    GLint status = glGetError()==GL_NO_ERROR; 

    /* create program and shader objects */

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    glAttachShader(program, vShader);
    glShaderSource(vShader, 1, &vSource, NULL);
    glCompileShader(vShader);

    glAttachShader(program, fShader);
    glShaderSource(fShader, 1, &fSource, NULL);
    glCompileShader(fShader);

    /* error check */
    int loglen = 0;
    char logbuffer[1000] = "";

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vShader, sizeof(logbuffer), &loglen, logbuffer);
    checkError(status, "Failed to compile the vertex shader.");

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(fShader, sizeof(logbuffer), &loglen, logbuffer);
    checkError(status, "Failed to compile the fragment shader.");

    /* link */

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glGetProgramInfoLog(program, sizeof(logbuffer), &loglen, logbuffer);

    checkError(status, "Failed to link the shader program object.");

    return program;
}