#pragma once
#include <Windows.h>

namespace DllUtils
{
	namespace
	{
		/**
		*  Create a registry key if one doesn't exist, and set a string value under that key.
		*  Return a handle to this key.
		*/
		HRESULT CreateRegKeyAndStringValue(
			HKEY hKey,                  // the root of the registry key
			PCWSTR pszSubKeyName,       // name of the subkey
			PCWSTR pszValueName,        // name of a value to create (or NULL if set the default value)
			PCWSTR pszData,             // the data in the value (string)
			HKEY* pResultKey            // return a pointer to the handle of the key
			)
		{
			*pResultKey = NULL;
			LONG result = ERROR_SUCCESS;

			// create the registry key requested - if the key already exists, just open it
			result = RegCreateKeyEx(
				hKey,                       // handle to the root key
				pszSubKeyName,              // name of the subkey
				0,                          // reserved - must be zero
				NULL,                       // class - can be ignored
				REG_OPTION_NON_VOLATILE,    // non-volatile - preserve between reboots
				KEY_ALL_ACCESS,             // security - allow all access
				NULL,                       // security attributes - none
				pResultKey,                 // pointer ot the result key
				NULL);                      // pValue indicating whether key was created or opened

			// if we have successfully created or opened the reg key, set the specified value
			// if the value name specified was NULL, then we are setting the "(Default)" value
			// of the key
			if (result == ERROR_SUCCESS)
			{
				result = RegSetValueExW(
					(*pResultKey),              // root key for the value - use the one just opened
					pszValueName,               // value name, or NULL for "(Default)" value
					0,                          // reserved - set to 0
					REG_SZ,                     // value type is string
					(LPBYTE) pszData,           // data itself
					((DWORD) wcslen(pszData) + 1) * sizeof(WCHAR)  // data buffer length
					);

				// if the operation failed, just close the result key
				if (result != ERROR_SUCCESS)
				{
					RegCloseKey(*pResultKey);
				}
			}

			return HRESULT_FROM_WIN32(result);
		}
	}

	/**
	* Deletes the registry entries for the COM object
	*/
	HRESULT UnregisterObject(const TCHAR* pszCOMKeyLocation)
	{
		// "HKLM\\Software\\Classes\\CLSID\\" + <Class ID>
		LONG lRes = RegDeleteTree(
			HKEY_LOCAL_MACHINE,         // root key - HKLM
			pszCOMKeyLocation);         // key name

		return HRESULT_FROM_WIN32(lRes);
	}

	/**
	* Create the registry entries for a COM object
	*/
	HRESULT RegisterCOMObject(HMODULE hmodule, const TCHAR* pszCOMKeyLocation, const TCHAR* pszDescription)
	{
		HRESULT hr = S_OK;
		HKEY hKey = NULL;
		HKEY hSubkey = NULL;

		WCHAR tempStr[MAX_PATH];

		// Create a new registry key under "HKLM\\Software\\Classes\\CLSID\\" + CoClass ID.
		hr = CreateRegKeyAndStringValue(
			HKEY_LOCAL_MACHINE,               // start in HKLM
			pszCOMKeyLocation,                // name + path of the key ("HKLM\\Software...")
			NULL,                             // name of a value under the key (none)
			pszDescription,                   // description of the filter
			&hKey);                           // returned handle of the key

		// get the filename and full path of this DLL and store it in the tempStr object        
		GetModuleFileName(hmodule, tempStr, MAX_PATH);
		hr = HRESULT_FROM_WIN32(GetLastError());

		// Create the "InprocServer32" subkey and set it to the name of the
		// DLL file
		hr = CreateRegKeyAndStringValue(
			hKey,                   // root key
			L"InProcServer32",      // name of the key
			NULL,                   // name of a value under the key (none)
			tempStr,                // write in the key the full path to the DLL
			&hSubkey);              // get a handle to the new key

		// close the InProcServer32 subkey immediately since we don't need anything else from it
		RegCloseKey(hSubkey);

		// Add the "ThreadingModel" subkey and set it to "Both"
		hr = CreateRegKeyAndStringValue(
			hKey,                   // root of the key
			L"InProcServer32",      // name of the key 
			L"ThreadingModel",      // name of the value which will be modified/added
			L"Both",                // value data - we support free and apartment thead model
			&hSubkey);              // get a handle to the new key

		// close the subkey handle immediately since we don't need it
		RegCloseKey(hSubkey);        

		// close hkeys since we are done
		RegCloseKey(hKey);

		return hr;
	}
}