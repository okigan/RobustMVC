#include "StdAfx.h"
#include "quad_model_render.h"

#include <exception>

#include <Core/Model/quad_model.h>


#include "../Visualization.h"


quad_modelRender::quad_modelRender(void)
{
    _model = nullptr;

    _program = 0;
}


quad_modelRender::~quad_modelRender(void)
{
}

quad_model const * quad_modelRender::Getquad_model() const
{
    return _model;
}

void quad_modelRender::Setquad_model( quad_model const * val )
{
    _model = val;
}


bool quad_modelRender::Initialize( char * logbuffer, int * loglen )
{
    const GLubyte * vendor = glGetString(GL_VENDOR);
    const GLubyte * render = glGetString(GL_RENDERER);
    const GLubyte * version = glGetString(GL_VERSION);
    const GLubyte * shversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte * extensions = glGetString(GL_EXTENSIONS);

    GLchar* vSource = 
        "varying float x;                                                               \r"
        "varying float y;                                                               \r"
        "void main() {                                                                  \r"
        "   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                     \r"
        "   gl_FrontColor = gl_Vertex;                                                  \r"
        "   x = gl_Vertex.x;                                                            \r"
        "   y = gl_Vertex.y;                                                            \r"
        "}                                                                              \r"
    ;

    GLchar* fSource = 
        "uniform float radius;                                                          \r"
        "varying float x;                                                               \r"
        "varying float y;                                                               \r"
        "void main()                                                                    \r"
        "{                                                                              \r"
        "    float r = sqrt(x*x + y*y);                                                 \r"
        "    if( r <= radius ) {                                                        \r"
        "        gl_FragColor = gl_Color;                                               \r"
        "    } else  {                                                                  \r"
        "        gl_FragColor = vec4(0.01, 0.01, 0.01, 1);                              \r"
        "   }                                                                           \r"
        "}                                                                              \r"
    ;    

    GLuint status = CreateShaderProgram(&_program, vSource, fSource, logbuffer, loglen);

    return GL_NO_ERROR ==  status;
}

void quad_modelRender::Render()
{
    glUseProgram(_program);
    //glUseProgram(0);
    {
        GLint location = glGetUniformLocation(_program, "radius");
        glUniform1f(location, (float)_model->get_radius());

        DrawQuad();
    }
    glUseProgram(0);
}
