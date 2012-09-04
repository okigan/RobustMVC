// dllmain.h : Declaration of module class.

class COpenGLHandlersModule : public ATL::CAtlDllModuleT< COpenGLHandlersModule >
{
public :
	DECLARE_LIBID(LIBID_OpenGLHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OPENGLHANDLERS, "{35140916-02E2-4C51-A4C5-81B458A44C18}")
};

extern class COpenGLHandlersModule _AtlModule;
