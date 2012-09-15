#pragma once

#include <Windows.h>
#include <gl/glew.h>
#include <gl/GL.h>

class QuadModel;

class QuadModelRender
{
public:
    QuadModelRender(void);
    ~QuadModelRender(void);

    QuadModel const * GetQuadModel() const;
    void SetQuadModel(QuadModel const * val);
    bool Initialize( HDC hDC, HGLRC hRC, char * logbuffer, int * loglen );
    void Render();

protected:
    QuadModel const *  _quadModel;
    
    GLuint _program;
};

