
// OpenGLDoc.h : interface of the COpenGLDoc class
//


#pragma once

#include <memory>

#include <Core/Model.h>
#include <Core/SharedPropertyBag.h>

class QuadModel;
class QuadModelController;




class COpenGLDoc : public CDocument
{
protected: // create from serialization only
	COpenGLDoc();
	DECLARE_DYNCREATE(COpenGLDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~COpenGLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

public:
    const QuadModel * GetQuadModel() const;

    void AddModelController(QuadModelController* controller);

    SharedPropertyBag * GetSharedRuntimePropertyBag() ;


private:
    void _OnPropertyChangeCallback(const Model::callback_params &);

protected:

    std::unique_ptr<QuadModel> m_model;
#ifndef DOXYGEN
    SharedPropertyBag m_SharedRuntimePropertyBag;
#endif


};
