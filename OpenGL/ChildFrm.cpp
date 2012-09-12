
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "OpenGLApp.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	EnableActiveAccessibility();
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

//BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
//{
//	return m_wndSplitter.Create(this,
//		2, 2,			// TODO: adjust the number of rows, columns
//		CSize(10, 10),	// TODO: adjust the minimum pane size
//        pContext, WS_CHILD | WS_VISIBLE |
//        WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT | WS_CLIPCHILDREN);
//}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPCHILDREN;
    cs.style |= WS_MAXIMIZE;
    cs.style |= WS_VISIBLE;
	
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
