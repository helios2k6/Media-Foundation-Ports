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

/* ATL Headers */
#include <atlbase.h>
#include <atlstr.h>

/* Annotation Headers */
#include <sal.h>

/* Application specific headers */
#include "immutable_string.h"
#include "standard_notation.h"