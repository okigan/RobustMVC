#pragma once

#include <gl/glew.h>
#include <gl/GL.h>

class Visualization
{
public:
    Visualization(void);
    ~Visualization(void);
};

void DrawQuad();


GLuint CreateShaderProgram(GLuint * program, 
    const GLchar *vSource , 
    const GLchar* fSource, 
    char * logbuffer, 
    int loglen);
