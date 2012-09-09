
// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGLApp.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")

#include "../Visualization/Visualization.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// COpenGLView construction/destruction

COpenGLView::COpenGLView()
{
	EnableActiveAccessibility();
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= CS_OWNDC;
    cs.style |= CS_BYTEALIGNCLIENT;
    cs.style |= WS_CLIPCHILDREN;
    cs.style |= WS_CLIPSIBLINGS;
    
	return CView::PreCreateWindow(cs);
}



// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    CRect rcClient;
    GetClientRect(&rcClient);

    int width = rcClient.Width();
    int height = rcClient.Height();

    HDC hDC = pDC->GetSafeHdc();
    HGLRC hRC = pDoc->GetRenderingContext();

    BOOL bRet = wglMakeCurrent(hDC, hRC);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspect = width / (double)height;
    glOrtho(-3, 3, -3/aspect, 3/aspect, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glUseProgram(0);
    display();

	glTranslated(1.5, 0, 0);

	glUseProgram(program);
    display();


    //GLint timeParam = glGetUniformLocation(program, "time");

    bRet = SwapBuffers(hDC);

    bRet = wglMakeCurrent(NULL, NULL);
}


// COpenGLView printing


void COpenGLView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


int ChoosePixelFormat(CDC* pDC) 
{
    PIXELFORMATDESCRIPTOR pfd = {0};

    pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion		= 1;
    pfd.dwFlags			= 
        PFD_DRAW_TO_WINDOW	|
        PFD_SUPPORT_OPENGL	| 
        PFD_DOUBLEBUFFER;
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

    if( pDC->IsPrinting() == TRUE ) {
        pfd.dwFlags			= 
            PFD_DRAW_TO_BITMAP	|
            PFD_SUPPORT_OPENGL	|
            PFD_SUPPORT_GDI;
    }

    int iPixelFormat = ChoosePixelFormat(pDC->m_hAttribDC, &pfd);

    return iPixelFormat;
}






// COpenGLView message handlers


int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    int pixelFormat = GetDocument()->GetPixelFormat();

    CDC* pDC = GetDC();
    HDC hDC = pDC->GetSafeHdc();

    BOOL bRet = FALSE;

    if( pixelFormat < 0 ) {
        pixelFormat = ChoosePixelFormat(pDC);

        GetDocument()->SetPixelFormat(pixelFormat);
    }

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    bRet = ::SetPixelFormat(hDC, pixelFormat, &pfd);

    HGLRC hRC = GetDocument()->GetRenderingContext();
    
    if( NULL == hRC) {
        hRC = wglCreateContext(hDC);

        GetDocument()->SetRenderingContext(hRC);
    }

   bRet = wglMakeCurrent(hDC, hRC);
 
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
        exit(1);

    GLchar* vSource = 
        "varying float x;                                                               "
        "varying float y;                                                               "
        "void main() {                                                                  "
        "   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                     "
        "   gl_FrontColor = gl_Vertex; "
        "   x = gl_Vertex.x;                                                          "
        "   y = gl_Vertex.y;                                                          "
        "}                                                                              "
        ;

        //"uniform float time;                                                           "
        //"                                                                              "
        //"void main()                                                                   "
        //"{                                                                             "
        //"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                   "
        //"                                                                              "
        //"    vec4 eyePosition = gl_ModelViewMatrix * gl_Vertex;                        "
        //"    vec4 eyeLightPos = gl_LightSource[0].position;                            "
        //"                                                                              "
        //"    vec3 eyeNormalVec = normalize(gl_NormalMatrix * gl_Normal);               "
        //"    vec3 eyeLightVec = normalize(eyeLightPos.xyz - eyePosition.xyz);          "
        //"    vec3 eyeViewVec = -normalize(eyePosition.xyz);                            "
        //"    vec3 eyeHalfVec = normalize(eyeLightVec + eyeViewVec);                    "
        //"                                                                              "
        //"    float Kd = max(dot(eyeLightVec, eyeNormalVec), 0.0);                      "
        //"    float Ks = pow(dot(eyeNormalVec, eyeHalfVec),                             "
        //"    gl_FrontMaterial.shininess);                                              "
        //"    float Ka = 1.0;                                                           "
        //"                                                                              "
        //"    gl_FrontColor = Kd * gl_FrontLightProduct[0].diffuse +                    "
        //"                Ks * gl_FrontLightProduct[0].specular +                       "
        //"            gl_FrontLightModelProduct.sceneColor;                             "
        //"}                                                                             "
        //;

    GLchar* fSource = ""
        "varying float x;                                                               "
        "varying float y;                                                               "
       "void main()                         "
        "{                                   "
		"    float r = sqrt(x*x + y*y);                                                            "
		"    if( r <= 1 ) {                                                     "
        "        gl_FragColor = gl_Color;        "
		"    } else  {											"
		"        gl_FragColor = vec4(0, 0, 0, 1);				"
		"	}"
        "}                                   "
    ;

    InitShader(vSource, fSource);

    bRet = wglMakeCurrent(NULL, NULL);

    ReleaseDC(pDC);

    return 0;
}


BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
    return FALSE;
}


void COpenGLView::OnDestroy()
{
    CView::OnDestroy();
}


void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    RedrawWindow();
}
