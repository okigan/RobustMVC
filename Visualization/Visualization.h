#pragma once

class Visualization
{
public:
    Visualization(void);
    ~Visualization(void);
};

void
display();

#include <gl/glew.h>
#include <gl/GL.h>

extern GLuint program;


void InitShader(const GLchar *vSource , const GLchar* fSource);