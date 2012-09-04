// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "OpenGLHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

COpenGLHandlersModule _AtlModule;

class COpenGLHandlersApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(COpenGLHandlersApp, CWinApp)
END_MESSAGE_MAP()

COpenGLHandlersApp theApp;

BOOL COpenGLHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
	return CWinApp::InitInstance();
}

int COpenGLHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
