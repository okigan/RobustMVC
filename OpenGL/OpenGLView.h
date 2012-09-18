
// OpenGLView.h : interface of the COpenGLView class
//

#pragma once

#include <afxwin.h>

#include <gl/glew.h>
#include <gl/GL.h>

#include <memory>

class QuadModelRender;
class QuadModelController;


class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Attributes
public:
	COpenGLDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    afx_msg BOOL OnCommand(UINT id);

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnUpdateCommandUI(CCmdUI* pUI);
	DECLARE_MESSAGE_MAP()

protected:

    BOOL SetPixelFormatOrCreateRenderingContext(HDC hDC, bool isPrinting, int * pPixelFormat, HGLRC * phRC);

protected:
    std::unique_ptr<QuadModelController> m_controller;

    std::unique_ptr<QuadModelRender>    m_render;

    std::shared_ptr<boost::any>         m_PixelFormat;
    std::shared_ptr<boost::any>         m_RenderingContext;

    COpenGLDoc*&    m_pDocument;

public:
    virtual void OnInitialUpdate();

};


