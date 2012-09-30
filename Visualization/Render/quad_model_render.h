#pragma once

#include <gl/glew.h>
#include <gl/GL.h>

class quad_model;

/// quad_modelRender
class quad_modelRender
{
public:
    quad_modelRender(void);
    ~quad_modelRender(void);

    quad_model const * Getquad_model() const;
    void Setquad_model(quad_model const * val);
    bool Initialize( char * logbuffer, int * loglen );
    void Render();

protected:
    quad_model const *  _model;
    
    GLuint _program;
};

