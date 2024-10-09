// dllmain.h : Declaration of module class.

class CNCMHandlersModule : public ATL::CAtlDllModuleT<CNCMHandlersModule>
{
public :
	DECLARE_LIBID(LIBID_NCMHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NCMHANDLERS, "{1fce1411-1662-4951-9541-319403e0ac66}")
};

extern class CNCMHandlersModule _AtlModule;
