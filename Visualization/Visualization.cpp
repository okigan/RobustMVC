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


void DrawQuad()
{
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
void CheckError(GLint status, const char *msg)
{
    if (!status)
    {
        printf("%s\n", msg);
        exit(EXIT_FAILURE);
    }
}

GLint CreateAndAttachShader( GLuint program, GLenum shaderType, const GLchar * source, char * logbuffer, int * loglen)
{
    GLuint vShader = glCreateShader(shaderType);
    glShaderSource(vShader, 1, &source, NULL);
    glCompileShader(vShader);

    GLint status = 0;

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vShader, sizeof(logbuffer), loglen, logbuffer);

    glAttachShader(program, vShader);
    return status;
}


GLuint CreateShaderProgram( GLuint * program, const GLchar *vSource , const GLchar* fSource, char * logbuffer, int * loglen )
{
    int initlen = *loglen;

    GLint status = glGetError()==GL_NO_ERROR; 

    GLuint p = glCreateProgram();

    *loglen = initlen;
    status = CreateAndAttachShader(p, GL_VERTEX_SHADER, vSource, logbuffer, loglen);
    CheckError(status, "Failed to link the shader program object.");

    *loglen = initlen;
    status = CreateAndAttachShader(p, GL_FRAGMENT_SHADER, fSource, logbuffer, loglen);
    CheckError(status, "Failed to link the shader program object.");

    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &status);

    *loglen = initlen;
    glGetProgramInfoLog(p, sizeof(logbuffer), loglen, logbuffer);

    CheckError(status, "Failed to link the shader program object.");

    *program = p;

    return status;
}

/*
GLint CreateAndAttachShaderARB( GLuint program, GLenum shaderType, const GLchar * source, char * logbuffer, int * loglen)
{
    GLhandleARB shaderObj = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    glShaderSource(vShader, 1, &source, NULL);
    glCompileShader(vShader);

    GLint status = 0;

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(vShader, sizeof(logbuffer), loglen, logbuffer);

    return status;
}

GLuint CreateShaderProgramARB( GLuint * program, const GLchar *vSource , const GLchar* fSource, char * logbuffer, int * loglen )
{

    int initlen = *loglen;

    GLint status = glGetError()==GL_NO_ERROR; 

    GLuint p = glCreateProgram();

    *loglen = initlen;
    status = CreateAndAttachShader(p, GL_VERTEX_SHADER, vSource, logbuffer, loglen);
    CheckError(status, "Failed to link the shader program object.");

    *loglen = initlen;
    status = CreateAndAttachShader(p, GL_FRAGMENT_SHADER, fSource, logbuffer, loglen);
    CheckError(status, "Failed to link the shader program object.");

    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &status);

    *loglen = initlen;
    glGetProgramInfoLog(p, sizeof(logbuffer), loglen, logbuffer);

    CheckError(status, "Failed to link the shader program object.");

    *program = p;

    return status;
}
*/
