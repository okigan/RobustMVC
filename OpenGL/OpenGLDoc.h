
// OpenGLDoc.h : interface of the COpenGLDoc class
//


#pragma once

#include <memory>

#include <Core/Model.h>
#include <Core/dictionary.h>

class quad_model;
class quad_model_controller;




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
    const quad_model * Getquad_model() const;

    void AddModelController(quad_model_controller* controller);

    dictionary * GetSharedRuntimePropertyBag() ;


private:
    void _OnPropertyChangeCallback(const model::callback_params &);

protected:

    std::unique_ptr<quad_model> m_model;
#ifndef DOXYGEN
    dictionary m_SharedRuntimePropertyBag;
#endif


};
