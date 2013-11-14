// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllmain.h"
#include "DllUtils.h"

// Handle to the DLL
HMODULE g_hModule = NULL;
// module ref count
ULONG g_dllLockCount = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HMODULE)hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// Returns a class factory to create an object of the requested type.
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return E_OUTOFMEMORY;
}

STDAPI DllCanUnloadNow(void)
{
	if(g_dllLockCount == 0) 
		return S_OK;
	else
		return S_FALSE;
}

// DllRegisterServer - Adds entries to the system registry.
STDAPI DllRegisterServer(void)
{
	HRESULT hr = S_OK;

	// Register the COM object CLSID so that CoCreateInstance() can be called to 
	// instantiate the MFT object.
	hr = DllUtils::RegisterCOMObject(g_hModule, H264_DECODER_MFT_CLSID_STR, L"H264 Decoder MFT");

	//TODO: REGISTER with MFT

	return hr;
}

// DllUnregisterServer - Removes entries from the system registry.
STDAPI DllUnregisterServer(void)
{
	// Unregister the COM object itself
	DllUtils::UnregisterObject(H264_DECODER_MFT_CLSID_STR);

	//TODO: Unregister with MFT

	return S_OK;
}