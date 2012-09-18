#pragma once

#include <gl/glew.h>
#include <gl/GL.h>

class QuadModel;

/// QuadModelRender
class QuadModelRender
{
public:
    QuadModelRender(void);
    ~QuadModelRender(void);

    QuadModel const * GetQuadModel() const;
    void SetQuadModel(QuadModel const * val);
    bool Initialize( char * logbuffer, int * loglen );
    void Render();

protected:
    QuadModel const *  _model;
    
    GLuint _program;
};

