// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DllUtils.h"

/* Static const strings */
#define DECODER_NAME L"Media Foundation Ports H264 Decoder"
#define H264_DECODER_MFT_CLSID_STR L"Software\\Classes\\CLSID\\{EBFF9D67-D774-4500-B4AA-EE243925B3A5}"

/* Handle to the DLL */
static HMODULE g_hModule = NULL;

/* Dll module ref count */
static ULONG g_dllLockCount = 0;

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
	//TODO: FINISH THIS OFF
	return E_OUTOFMEMORY;
}

STDAPI DllCanUnloadNow(void)
{
	if (g_dllLockCount == 0)
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
	hr = DllUtils::RegisterCOMObject(g_hModule, H264_DECODER_MFT_CLSID_STR, DECODER_NAME);

	if (FAILED(hr))
	{
		goto finished;
	}

	hr = MFTRegister(
		CLSID_CH264DecoderMFT,
		MFT_CATEGORY_VIDEO_DECODER,
		DECODER_NAME,
		MFT_ENUM_FLAG_ASYNCMFT,
		0,
		NULL,
		0,
		NULL,
		NULL);

finished:
	return hr;
}

// DllUnregisterServer - Removes entries from the system registry.
STDAPI DllUnregisterServer(void)
{
	// Unregister the COM object itself
	HRESULT hr = DllUtils::UnregisterObject(H264_DECODER_MFT_CLSID_STR);

	if (FAILED(hr))
	{
		goto finished;
	}

	// Unregister the MFT
	hr = MFTUnregister(CLSID_CH264DecoderMFT);

finished:
	return hr;
}