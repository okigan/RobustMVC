
// OpenGLDoc.cpp : implementation of the COpenGLDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGLApp.h"
#endif

#include "OpenGLDoc.h"
#include "resource.h"

#include <propkey.h>

#include <Core/Model/quad_model.h>
#include <Core/Controller/quad_model_controller.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenGLDoc

IMPLEMENT_DYNCREATE(COpenGLDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenGLDoc, CDocument)
END_MESSAGE_MAP()


// COpenGLDoc construction/destruction

COpenGLDoc::COpenGLDoc()
{
    m_model.reset(new quad_model());

    model::Callback callback = std::bind(&COpenGLDoc::_OnPropertyChangeCallback, this, std::placeholders::_1);

    m_model->SetCallback(callback);
}

COpenGLDoc::~COpenGLDoc()
{

}

BOOL COpenGLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// COpenGLDoc serialization

void COpenGLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COpenGLDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COpenGLDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COpenGLDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenGLDoc diagnostics

#ifdef _DEBUG
void COpenGLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenGLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenGLDoc commands

void COpenGLDoc::_OnPropertyChangeCallback( const model::callback_params & params)
{
    if( params.model == m_model.get() )
    {
        if( model::e_changing == params.stage )
        {

        }
        else
        {
            UpdateAllViews(NULL, 0, 0);
        }
    }
    else
    {

    }
}

const quad_model * COpenGLDoc::Getquad_model() const
{
    return m_model.get();
}


dictionary * COpenGLDoc::GetSharedRuntimePropertyBag()
{
    return &m_SharedRuntimePropertyBag;
}

void COpenGLDoc::AddModelController( quad_model_controller* controller )
{
    controller->set_model(m_model.get());
}
