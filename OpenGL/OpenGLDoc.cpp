
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

#include "Core/Model/QuadModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenGLDoc

IMPLEMENT_DYNCREATE(COpenGLDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenGLDoc, CDocument)
    ON_COMMAND(ID_COMMAND_INCREASE, &COpenGLDoc::OnCommandIncrease)
    ON_COMMAND(ID_COMMAND_DECREASE, &COpenGLDoc::OnCommandDecrease)
END_MESSAGE_MAP()


// COpenGLDoc construction/destruction

COpenGLDoc::COpenGLDoc()
{
	m_PixelFormat = -1;
    m_RenderingContext = NULL;

    m_QuadModel.reset(new QuadModel());

    Model::Callback callback = std::bind(&COpenGLDoc::OnPropertyChangeCallback, this, std::placeholders::_1);

    m_QuadModel->SetCallback(callback);
}

COpenGLDoc::~COpenGLDoc()
{
    wglDeleteContext(m_RenderingContext);

    m_RenderingContext = NULL;


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


int COpenGLDoc::GetPixelFormat(void)
{
    return m_PixelFormat;
}

void COpenGLDoc::SetPixelFormat(int pixelFormat)
{
    m_PixelFormat = pixelFormat;
}

HGLRC COpenGLDoc::GetRenderingContext(void)
{
    return m_RenderingContext;
}

void COpenGLDoc::SetRenderingContext(HGLRC renderingContext)
{
    m_RenderingContext = renderingContext;
}

void COpenGLDoc::OnPropertyChangeCallback( const Model::callback_params & params)
{
    if( params.model == m_QuadModel.get() )
    {
        if( Model::e_changing == params.stage )
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

const QuadModel * COpenGLDoc::GetQuadModel() const
{
    return m_QuadModel.get();
}


void COpenGLDoc::OnCommandIncrease()
{
    m_QuadModel->SetRadius(m_QuadModel->GetRadius() + 0.1);
}


void COpenGLDoc::OnCommandDecrease()
{
    m_QuadModel->SetRadius(m_QuadModel->GetRadius() - 0.1);
}
