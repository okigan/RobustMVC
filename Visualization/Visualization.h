#pragma once

#include <gl/glew.h>
#include <gl/GL.h>

class Visualization
{
public:
    Visualization(void);
    ~Visualization(void);
};

void
display();


GLuint InitShader(const GLchar *vSource , const GLchar* fSource);