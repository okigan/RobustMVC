
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

    int view_count = 0;
    {
        POSITION pos_dt = theApp.GetFirstDocTemplatePosition();

        if( NULL != pos_dt) {
            for(CDocTemplate* dt = theApp.GetNextDocTemplate(pos_dt); dt != NULL; dt = pos_dt ? theApp.GetNextDocTemplate(pos_dt) : nullptr) {
                POSITION pos_doc = dt->GetFirstDocPosition();

                if( NULL != pos_doc) {
                    for(CDocument* doc = dt->GetNextDoc(pos_doc); doc != NULL; doc = pos_doc ? dt->GetNextDoc(pos_doc) : nullptr){
                        POSITION pos_view = doc->GetFirstViewPosition();

                        if( NULL != pos_view) {
                            for(CView* view = doc->GetNextView(pos_view); view != NULL; view = pos_view ? doc->GetNextView(pos_view) : nullptr){
                                view_count++;
                            }
                        }
                    }
                }
            }
        }
    }

    if( 0 == view_count ) {
        cs.style |= WS_MAXIMIZE;
        cs.style |= WS_VISIBLE;
    }
	
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
