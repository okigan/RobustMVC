// WebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <vector>

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")

extern "C" {
#include "mongoose.h"
};


#include <Core/Model/quad_model.h>
#include <Core/controller.h>
#include <Core/Controller/quad_model_controller.h>


#include <Visualization/Visualization.h>
#include <Visualization/Render/quad_model_render.h>


#include <Windows.h>

#include <iostream>
#include <Core/dictionary.h>

int ChoosePixelFormatEx(HDC hDC, bool isPrinting) 
{
    PIXELFORMATDESCRIPTOR pfd = {0};

    pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion		= 1;
    pfd.dwFlags			= 
        //PFD_DRAW_TO_WINDOW	|
        PFD_SUPPORT_OPENGL
        //	| PFD_DOUBLEBUFFER
        ;
    pfd.iPixelType		= PFD_TYPE_RGBA;
    pfd.cColorBits		= 32;
    pfd.cRedBits		= 0;
    pfd.cRedShift		= 0;
    pfd.cGreenBits		= 0;
    pfd.cGreenShift		= 0;
    pfd.cBlueBits		= 0;
    pfd.cBlueShift		= 0;
    pfd.cAlphaBits		= 0;
    pfd.cAlphaShift		= 0;
    pfd.cAccumBits		= 0;	
    pfd.cAccumRedBits	= 0;
    pfd.cAccumGreenBits	= 0;
    pfd.cAccumBlueBits	= 0;
    pfd.cAccumAlphaBits	= 0;
    pfd.cDepthBits		= 16;
    pfd.cStencilBits	= 0;
    pfd.cAuxBuffers		= 0;
    pfd.iLayerType		= PFD_MAIN_PLANE;
    pfd.bReserved		= 0;
    pfd.dwLayerMask		= 0;
    pfd.dwVisibleMask	= 0;
    pfd.dwDamageMask	= 0;

    if( isPrinting ) {
        pfd.dwFlags			= 
            PFD_DRAW_TO_BITMAP	|
            PFD_SUPPORT_OPENGL	|
            PFD_SUPPORT_GDI;
    }

    int iPixelFormat = ChoosePixelFormat(hDC, &pfd);

    return iPixelFormat;
}


bool write_quad_model_as_bmp(struct mg_connection *conn, quad_model* model)
{
    int width = 640;
    int height = 320;

    HWND hWND = CreateWindow(_T("BUTTON"), 
            NULL, 
            WS_OVERLAPPEDWINDOW /*| WS_VISIBLE*/ | CS_OWNDC, 
            0, 0, width, height, 
            NULL, NULL, NULL, NULL);

    HDC hDC = GetDC(hWND);
    int pixelForamt = ChoosePixelFormatEx(hDC, false);
    PIXELFORMATDESCRIPTOR pfd = {0};
    BOOL bRet = SetPixelFormat(hDC, pixelForamt, &pfd);
    HGLRC hRC = wglCreateContext(hDC);

    bRet = wglMakeCurrent(hDC, hRC);

    GLenum err = glewInit( );
    if( GLEW_OK != err )
        return false;

    if( !GLEW_VERSION_2_1 )  // check that the machine supports the 2.1 API.
        return false;
    //
    //    if( !GLEW_ARB_framebuffer_object)  // check that the machine supports the 2.1 API.
    //        return false;

    //if( !GLEW_VERSION_3_1 )  // check that the machine supports the 2.1 API.
    //    return false;


    // create a texture object
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a renderbuffer object to store depth info
    GLuint rboId = 0;
    glGenRenderbuffersEXT(1, &rboId);
    glBindRenderbufferEXT(GL_RENDERBUFFER, rboId);
    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbufferEXT(GL_RENDERBUFFER, 0);

    // create a framebuffer object
    GLuint fboId = 0;
    glGenFramebuffersEXT(1, &fboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER, fboId);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

    bool fboUsed = true;
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        fboUsed = false;

    glBindFramebufferEXT(GL_FRAMEBUFFER, fboId);

    glViewport(0, 0, width, height);
    glClearColor(1, 1, 0, 1);

    //glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
    //glDrawBuffer(GL_BACK);
    //glReadBuffer(GL_BACK);
    assert(0 == glGetError());

    //glClear(GL_FRONT_AND_BACK);
    assert(0 == glGetError());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspect = width / (double)height;
    glOrtho(-3, 3, -3/aspect, 3/aspect, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(-1.5, 0, 0);

    //glUseProgram(0);
    DrawQuad();

    glTranslated(1.5, 0, 0);
    //glPopAttrib();

    quad_modelRender qr;
    qr.Setquad_model(model);

    char logbuffer[1024] = "";
    int loglen = ARRAYSIZE(logbuffer);
    qr.Initialize(logbuffer, &loglen);
    assert(0 == glGetError());

    qr.Render();
    assert(0 == glGetError());

    glFinish();

    GLenum error = glGetError();

    int bits = 24;

    std::vector<unsigned char> buffer;
    buffer.resize( width * height * bits / 8 );

    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, &buffer[0]);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWND, hDC);
    //CloseWindow(hWND);

    BITMAPFILEHEADER bfh = {0};
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + buffer.size();
    bfh.bfType = 0x4D42;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER bih = {};
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth= width;
    bih.biHeight= height;
    bih.biPlanes= 1;
    bih.biBitCount= bits;

    // image/bmp
    mg_write(conn, &bfh, sizeof(bfh));
    mg_write(conn, &bih, sizeof(bih));
    mg_write(conn, &buffer[0], buffer.size());
}


class web_view
{
public:
    web_view()
    {

    }

    ~web_view()
    {

    }

    virtual void render(struct mg_connection *conn) = 0;
};

class quad_model_image_web_view : public web_view
{
public:
    quad_model_image_web_view()
    {

    }

    ~quad_model_image_web_view()
    {

    }

    void set_model(quad_model * model)
    {
        _model = model;

    }

    void render(struct mg_connection *conn)
    {
        const struct mg_request_info *request_info = mg_get_request_info(conn);

        std::string str = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: image/bmp\r\n"
            "\r\n"
            ;

        mg_write(conn, str.c_str(), str.length());

        model* m = _model;
        quad_model* qm = static_cast<quad_model*>(m);
        write_quad_model_as_bmp(conn, qm);
    }

private:
    model * _model;
};

//if(     0 == strcmp("/", request_info->uri) 
//    ||  0 == strcmp("/favicon.ico", request_info->uri))
//{
    // json (for future)
    //char content[1024];
    //int content_length = snprintf(content, sizeof(content),
    //    "{                                                      \n"
    //    "   \"quad_model\" : {                                   \n"
    //    "       \"radius\" : \"%lf\"                            \n"
    //    "   }                                                   \n"
    //    "}                                                      \n",
    //    model.GetRadius());


//}

class quad_model_web_controller : public controller
{
public:
    quad_model_web_controller()
    {

    }

    ~quad_model_web_controller()
    {

    }

    void set_model(quad_model * model)
    {
        _controler.set_model(model);
    }

    std::shared_ptr<quad_model_image_web_view> process(const std::string & verb, 
        const std::string & id, 
        const std::string & action, 
        const std::string & view)
    {
        if( "GET" == verb ) {
            if( "view" == action ) {
                auto ptr = std::make_shared<quad_model_image_web_view>();
                ptr->set_model(static_cast<quad_model*>(_controler.get_model()));
                return ptr;
            } else if( "incr" == action ) {
                _controler.increase_radius();

                auto ptr = std::make_shared<quad_model_image_web_view>();
                ptr->set_model(static_cast<quad_model*>(_controler.get_model()));
                return ptr;
            } else if( "decr" == action ) {
                _controler.decrease_radius();

                auto ptr = std::make_shared<quad_model_image_web_view>();
                ptr->set_model(static_cast<quad_model*>(_controler.get_model()));
                return ptr;
            }
        }
    }

    model * get_model()
    {
        return _controler.get_model();
    }

    virtual model* get_model() const
    {
        throw std::exception("The method or operation is not implemented.");
    }

    virtual bool is_action_enabled( int action_id ) 
    {
        return _controler.is_action_enabled(action_id);
    }

private:
    quad_model_controller _controler;

};



#if _MSC_VER
#define snprintf _snprintf
#endif

void tokenize( const std::string & uri, std::vector<std::string> & parts, char* delims ) 
{
    size_t prev = 0;
    size_t curr = 0;

    while( curr != std::string::npos ) {
        curr = uri.find_first_of(delims, prev);

        if( std::string::npos != curr ) {
            std::string token = uri.substr(prev, curr - prev);
            parts.push_back(token);
            prev = curr + 1;
        } else {
            std::string token = uri.substr(prev);
            parts.push_back(token);
        }
    }
}

bool isdigit(const std::string & str)
{
    return str.size() == std::count_if(str.cbegin(), 
        str.cend(), 
        [](const std::string::value_type & c) { return isdigit(c); } 
    );
}

void parse( const std::vector<std::string> &route, 
    dictionary* routing, 
    quad_model_web_controller* &controller, 
    std::string &id, 
    std::string &action, 
    std::string &view )
{
    enum e_part
    {
        e_part_controller,
        e_part_id,
        e_part_action,
        e_part_view,
    };

    // skip the first ""
    size_t part_index = 1;

    for(enum e_part ep = e_part_controller; ep <= e_part_view; ep = static_cast<enum e_part>(ep + 1))
    {
        if( part_index >= route.size() ) {
            break;
        }

        std::string token = route[part_index];

        switch(ep)
        {
        case e_part_controller:
            {
                boost::any c;
                if( routing->find(token, c) ) {
                    controller = boost::any_cast<quad_model_web_controller*>(c);
                    part_index++;
                }
            }
            break;
        case e_part_id:
            {
                if( isdigit(token) ) {
                    id = token;
                    part_index++;
                }
            }
            break;
        case e_part_action:
            {
                action = token;
                part_index++;
            }
            break;
        case e_part_view:
            {
                view = token;
                part_index++;
            }
            break;
        }
    }
}

static void *callback(enum mg_event event, struct mg_connection *conn) 
{
    if (event == MG_NEW_REQUEST) {
        const struct mg_request_info *request_info = mg_get_request_info(conn);

        printf("verb='%s' remote_port='%d', uri='%s'\n", 
            request_info->request_method, 
            request_info->remote_port,
            request_info->uri);

        std::string uri = request_info->uri;
        std::vector<std::string> route;

        tokenize(uri, route, "/?");

        dictionary* routing = static_cast<dictionary*>(request_info->user_data);
        
        std::string verb = request_info->request_method;
        quad_model_web_controller* controller = NULL;
        std::string id;
        std::string action = "view";
        std::string view = "";

        parse(route, routing, controller, id, action, view);

        if( nullptr != controller ) {

            auto v = controller->process(verb, id, action, view);

            v->render(conn);
        }

        // Mark as processed
        return "";
    } else {
        return NULL;
    }
}

int _tmain(int argc, _TCHAR* argv[]) {
    quad_model model;
    quad_model_web_controller web_controller;

    web_controller.set_model(&model);

    dictionary routes;
    routes.insert(""    , boost::any(&web_controller));
    routes.insert("home", boost::any(&web_controller));

    int nport = 8080;

    printf("See more information on: http://www.codeproject.com/Articles/464158/Robust-MVC" "\n");
    printf("\n");

    char port[80] = "";
    snprintf(port, ARRAYSIZE(port), "%d", nport);

    const char *options[] = {"listening_ports", port, NULL};

    struct mg_context *ctx = mg_start(&callback, &routes, options);

    if( NULL == ctx ) {
        printf("Failed to start server (is the %d port already taken?)\n", nport);
        return -1;
    }
    
    printf("\n");
    printf("Navigate to: http://localhost:%d\n", nport);
    printf("\n\n");

    printf("Press Control+C to stop web server and exit.\n");
    printf("\n\n");

    getchar();  // Wait until user hits "enter"

    mg_stop(ctx);

    return 0;
}

