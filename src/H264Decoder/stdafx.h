// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

/* H264 decoder specific header files */
#include <mfapi.h>
#include <mftransform.h>
#include <mfidl.h>
#include <Mferror.h>

// {EBFF9D67-D774-4500-B4AA-EE243925B3A5}
DEFINE_GUID(CLSID_CH264DecoderMFT, 0xebff9d67, 0xd774, 0x4500, 0xb4, 0xaa, 0xee, 0x24, 0x39, 0x25, 0xb3, 0xa5);
