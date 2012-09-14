#include "StdAfx.h"
#include "QuadModelRender.h"

#include <exception>

#include <Core/Model/QuadModel.h>


#include "../Visualization.h"


QuadModelRender::QuadModelRender(void)
{
    _quadModel = nullptr;

    _program = 0;
}


QuadModelRender::~QuadModelRender(void)
{
}

QuadModel const * QuadModelRender::GetQuadModel() const
{
    return _quadModel;
}

void QuadModelRender::SetQuadModel( QuadModel const * val )
{
    _quadModel = val;
}

void QuadModelRender::Initialize( HDC hDC, HGLRC hRC )
{
    GLchar* vSource = 
        "varying float x;                                                               "
        "varying float y;                                                               "
        "void main() {                                                                  "
        "   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                     "
        "   gl_FrontColor = gl_Vertex;													"
        "   x = gl_Vertex.x;															"
        "   y = gl_Vertex.y;															"
        "}                                                                              "
    ;

    GLchar* fSource = 
        "uniform float radius;                                                          "
        "varying float x;                                                               "
        "varying float y;                                                               "
        "void main()                         											"
        "{                                   											"
        "    float r = sqrt(x*x + y*y);                                                 "
        "    if( r <= radius ) {                                                     	"
        "        gl_FragColor = gl_Color;        										"
        "    } else  {																	"
        "        gl_FragColor = vec4(0, 0, 0, 1);										"
        "	}																			"
        "}                                   											"
    ;


    
    char logbuffer[1000] = "";
    int loglen = ARRAYSIZE(logbuffer);
    

    GLuint status = CreateShaderProgram(&_program, vSource, fSource, logbuffer, loglen);
}

void QuadModelRender::Render()
{
    glUseProgram(_program);
    {
        GLint location = glGetUniformLocation(_program, "radius");
        glUniform1f(location, (float)_quadModel->GetRadius());

        DrawQuad();
    }
    glUseProgram(0);
}
