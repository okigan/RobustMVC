// WebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <string.h>

#include <vector>

#include <Windows.h>

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")

#include "mongoose.h"
#include "mongoose.c"

#include <Core/Model/QuadModel.h>
#include <Core/Controller/QuadModelController.h>
#include <Visualization/Visualization.h>
#include <Visualization/Render/QuadModelRender.h>

QuadModel model;
QuadModelController controler;

#if _MSC_VER
#define snprintf _snprintf
#endif

int ChoosePixelFormat(HDC hDC, bool isPrinting) 
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

HWND g_hWND = NULL;

bool f(struct mg_connection *conn)
{
    int width = 512;
    int height = 512;

    HWND hWND = NULL;
    if( NULL == g_hWND) {
        hWND = CreateWindow(_T("BUTTON"), 
            NULL, 
            WS_OVERLAPPEDWINDOW /*| WS_VISIBLE*/ | CS_OWNDC, 
            0, 0, width, height, 
            NULL, NULL, NULL, NULL);
        g_hWND = hWND;
    } else {
        hWND = g_hWND;
    }

    HDC hDC = GetDC(hWND);
    int pixelForamt = ChoosePixelFormat(hDC, false);
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

    QuadModelRender qr;
    qr.SetQuadModel(&model);

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

static void *callback(enum mg_event event,
struct mg_connection *conn) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);

    printf("verb='%s' remote_ip='%d' remote_port='%d', uri='%s'\n", 
        conn->request_info.request_method, 
        conn->request_info.remote_ip,
        conn->request_info.remote_port,
        conn->request_info.uri);


    if (event == MG_NEW_REQUEST) {

        if( 0 == strcmp("/", conn->request_info.uri) ) {
            char content[1024];
            int content_length = snprintf(content, sizeof(content),
                "{                                                      \n"
                "   \"QuadModel\" : {                                   \n"
                "       \"radius\" : \"%lf\"                            \n"
                "   }                                                   \n"
                "}                                                      \n",
                model.GetRadius());

            std::string str = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: image/bmp\r\n"
                "\r\n"
                // "hello"
                ;

            mg_write(conn, str.c_str(), str.length());
            f(conn);
        }


        //mg_printf(conn,
        //    "HTTP/1.1 200 OK\r\n"
        //    "Content-Type: text/plain\r\n"
        //    "Content-Length: %d\r\n"        // Always set Content-Length
        //    "\r\n"
        //    "%s",
        //    content_length, content);

        // Mark as processed
        return "";
    } else {
        return NULL;
    }
}

int _tmain(int argc, _TCHAR* argv[]) {
    controler.SetModel(&model);

    struct mg_context *ctx;
    const char *options[] = {"listening_ports", "8080", NULL};

    ctx = mg_start(&callback, NULL, options);
    getchar();  // Wait until user hits "enter"
    mg_stop(ctx);

    return 0;
}