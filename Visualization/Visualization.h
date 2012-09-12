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
draw_quad();


GLuint init_shader(const GLchar *vSource , const GLchar* fSource);