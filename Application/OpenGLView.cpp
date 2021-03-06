
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

#include <Core/Model/quad_model.h>
#include <Core/Controller/quad_model_controller.h>

#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")

#include <Visualization/Visualization.h>

#include <Visualization/Render/quad_model_render.h>




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
    ON_COMMAND_EX(ID_COMMAND_INCREASE, &COpenGLView::OnCommand)
    ON_COMMAND_EX(ID_COMMAND_DECREASE, &COpenGLView::OnCommand)
    ON_UPDATE_COMMAND_UI(ID_COMMAND_INCREASE, &COpenGLView::OnUpdateCommandUI)
    ON_UPDATE_COMMAND_UI(ID_COMMAND_DECREASE, &COpenGLView::OnUpdateCommandUI)

END_MESSAGE_MAP()

// COpenGLView construction/destruction

COpenGLView::COpenGLView() : m_pDocument (reinterpret_cast<COpenGLDoc*&>(CView::m_pDocument))
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
    HGLRC hRC = boost::any_cast<HGLRC>(*m_RenderingContext);

    BOOL bRet = wglMakeCurrent(hDC, hRC);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspect = width / (double)height;
    glOrtho(-3, 3, -3/aspect, 3/aspect, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(-1.5, 0, 0);

    glUseProgram(0);
    DrawQuad();

    glTranslated(1.5, 0, 0);


    //glUseProgram(m_program);
    //GLint location = glGetUniformLocation(m_program, "radius");
    //glUniform1f(location, (float)GetDocument()->Getquad_model()->GetRadius());
    //display();

    if( nullptr != m_render ) {
        m_render->Render();
    }

    glFinish();

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
#endif //_DEBUG

COpenGLDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return m_pDocument;
}



int ChoosePixelFormat(HDC hDC, bool isPrinting) 
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

    if( isPrinting ) {
        pfd.dwFlags			= 
            PFD_DRAW_TO_BITMAP	|
            PFD_SUPPORT_OPENGL	|
            PFD_SUPPORT_GDI;
    }

    int iPixelFormat = ChoosePixelFormat(hDC, &pfd);

    return iPixelFormat;
}


// COpenGLView message handlers
void DeleterBoostAnyHGLRC( HGLRC* ptr)
{
    BOOL bRet = wglDeleteContext(*ptr);

    delete ptr;
}

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    auto pb = GetDocument()->GetSharedRuntimePropertyBag();
    auto & value = pb->find("pixel_format");
    int pixelFormat = ( !value.empty() ) ? boost::any_cast<int>(value) : -1;

    CDC* pDC = GetDC();
    {
        HDC hDC = pDC->GetSafeHdc();
        bool isPrinting = pDC->IsPrinting() != FALSE;

        HGLRC hRC = NULL;

        BOOL bRet = SetPixelFormatOrCreateRenderingContext(hDC, isPrinting, &pixelFormat, &hRC);

        if( value.empty() ) {
            m_PixelFormat.reset(new int(pixelFormat));
            m_RenderingContext.reset(new HGLRC(hRC), DeleterBoostAnyHGLRC);
            
            pb->insert_as_weak_ptr("pixel_format", m_PixelFormat);
            pb->insert_as_weak_ptr("rendering_context", m_RenderingContext);
        } else {
            m_PixelFormat       = boost::any_cast<decltype(m_PixelFormat        )>(pb->find("pixel_format"));
            m_RenderingContext  = boost::any_cast<decltype(m_RenderingContext   )>(pb->find("rendering_context"));
        }
    }
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
}

BOOL COpenGLView::SetPixelFormatOrCreateRenderingContext(HDC hDC, bool isPrinting, int * pPixelFormat, HGLRC * phRC)
{
    BOOL bRet = FALSE;

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    if( *pPixelFormat < 0 ) {
        *pPixelFormat = ChoosePixelFormat(hDC, isPrinting);

        bRet = SetPixelFormat( hDC, *pPixelFormat, &pfd );
        if( TRUE != bRet )
            return bRet;

        *phRC = wglCreateContext( hDC );

        bRet = wglMakeCurrent( hDC, *phRC );
        if( TRUE != bRet )
            return bRet;

        GLenum err = glewInit( );
        if( GLEW_OK != err )
            return FALSE;

        if( !GLEW_VERSION_2_1 )  // check that the machine supports the 2.1 API.
            return FALSE;

        bRet = wglMakeCurrent( NULL, NULL );

    } else {
        bRet = ::SetPixelFormat( hDC, *pPixelFormat, &pfd );
        if( TRUE != bRet )
            return bRet;
    }

    return bRet;
}


void COpenGLView::OnInitialUpdate( )
{
    CView::OnInitialUpdate( );

    m_controller.reset( new quad_model_controller() );

    GetDocument()->AddModelController(m_controller.get());

    m_render.reset( new quad_modelRender() );

    m_render->Setquad_model( GetDocument()->Getquad_model() );

    CDC* pDC = GetDC( );
    HDC hDC = pDC->GetSafeHdc( );
    HGLRC hRC = boost::any_cast<HGLRC>( *m_RenderingContext );

    BOOL bRet = wglMakeCurrent( hDC, hRC );
    {
        char logbuffer[1000] = "";
        int loglen = ARRAYSIZE(logbuffer);

        m_render->Initialize( logbuffer, &loglen );
    }
    bRet = wglMakeCurrent( NULL, NULL );

    ReleaseDC( pDC );
}

BOOL COpenGLView::OnCommand(UINT id)
{
    switch( id )
    {
    case ID_COMMAND_INCREASE: m_controller->increase_radius(); break;
    case ID_COMMAND_DECREASE: m_controller->decrease_radius(); break;
    }

    return TRUE;
}

void COpenGLView::OnUpdateCommandUI( CCmdUI* pUI )
{
    if( !m_controller.get() )
        return ; 

    switch( pUI->m_nID)
    {
    case ID_COMMAND_INCREASE: pUI->Enable(m_controller->is_action_enabled(quad_model_controller::e_increase_radius)); break;
    case ID_COMMAND_DECREASE: pUI->Enable(m_controller->is_action_enabled(quad_model_controller::e_decrease_radius)); break;
    }
}
