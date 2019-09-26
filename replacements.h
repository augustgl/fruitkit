// shoutout to zerosum0x0 for replacing getprocaddress
// https://github.com/zerosum0x0/LoadLibrary-GetProcAddress-Replacements

#pragma once

#include "vxheader.h"

#include <intrin.h>

void WINAPI RtlInitUnicodeString(
	PUNICODE_STRING target, /* [I/O] Buffered unicode string to be initialized */
	PCWSTR source)          /* [I]   '\0' terminated unicode string used to initialize target */
{
	if ((target->Buffer = (PWSTR)source))
	{
		unsigned int length = lstrlenW(source) * sizeof(WCHAR);
		if (length > 0xfffc)
			length = 0xfffc;
		target->Length = length;
		target->MaximumLength = target->Length + sizeof(WCHAR);
	}
	else target->Length = target->MaximumLength = 0;
}

HMODULE WINAPI GetModuleBaseAddress(LPCWSTR moduleName)
{
	PEB *pPeb = NULL;
	LIST_ENTRY *pListEntry = NULL;
	LDR_DATA_TABLE_ENTRY *pLdrDataTableEntry = NULL;


	pPeb = (PPEB)__readgsqword(0x60);


	if (pPeb == NULL)
		return NULL;

	pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)pPeb->Ldr->InMemoryOrderModuleList.Flink;
	pListEntry = pPeb->Ldr->InMemoryOrderModuleList.Flink;

	do
	{
		if (lstrcmpiW(pLdrDataTableEntry->FullDllName.Buffer, moduleName) == 0)
			return (HMODULE)pLdrDataTableEntry->Reserved2[0];

		pListEntry = pListEntry->Flink;
		pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry->Flink);

	} while (pListEntry != pPeb->Ldr->InMemoryOrderModuleList.Flink);

	return NULL;
}

FARPROC* WINAPI GetExportAddress(HMODULE hMod, const char *lpProcName)
{
	char *pBaseAddress = (char *)hMod;

	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER *)pBaseAddress;
	IMAGE_NT_HEADERS *pNtHeaders = (IMAGE_NT_HEADERS *)(pBaseAddress + pDosHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER *pOptionalHeader = &pNtHeaders->OptionalHeader;
	IMAGE_DATA_DIRECTORY *pDataDirectory = (IMAGE_DATA_DIRECTORY *)(&pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	IMAGE_EXPORT_DIRECTORY *pExportDirectory = (IMAGE_EXPORT_DIRECTORY *)(pBaseAddress + pDataDirectory->VirtualAddress);

	void **ppFunctions = (void **)(pBaseAddress + pExportDirectory->AddressOfFunctions);
	WORD *pOrdinals = (WORD *)(pBaseAddress + pExportDirectory->AddressOfNameOrdinals);
	ULONG *pNames = (ULONG *)(pBaseAddress + pExportDirectory->AddressOfNames);
	/* char **pNames = (char **)(pBaseAddress + pExportDirectory->AddressOfNames); /* */

	FARPROC *pAddress = NULL;

	typedef HMODULE(WINAPI *LoadLibraryAF)(LPCSTR lpFileName);
	LoadLibraryAF pLoadLibraryA = NULL;

	DWORD i;

	if (((DWORD_PTR)lpProcName >> 16) == 0)
	{
		WORD ordinal = LOWORD(lpProcName);
		DWORD dwOrdinalBase = pExportDirectory->Base;

		if (ordinal < dwOrdinalBase || ordinal >= dwOrdinalBase + pExportDirectory->NumberOfFunctions)
			return NULL;

		pAddress = (FARPROC*)(pBaseAddress + (DWORD_PTR)ppFunctions[ordinal - dwOrdinalBase]);
	}
	else
	{
		for (i = 0; i < pExportDirectory->NumberOfNames; i++)
		{
			char *szName = (char*)pBaseAddress + (DWORD_PTR)pNames[i];
			if (strcmp(lpProcName, szName) == 0)
			{
				pAddress = (FARPROC*)(pBaseAddress + ((ULONG*)(pBaseAddress + pExportDirectory->AddressOfFunctions))[pOrdinals[i]]);
				break;
			}
		}
	}

	if ((char *)pAddress >= (char *)pExportDirectory && (char *)pAddress < (char *)pExportDirectory + pDataDirectory->Size)
	{
		char *szDllName, *szFunctionName;
		HMODULE hForward;

		szDllName = _strdup((const char *)pAddress);
		if (!szDllName)
			return NULL;

		pAddress = NULL;
		szFunctionName = strchr(szDllName, '.');
		*szFunctionName++ = 0;

		LdrLoadDll ldll = (LdrLoadDll)GetExportAddress(GetModuleBaseAddress(L"ntdll.dll"), "LdrLoadDll");

		UNICODE_STRING input;

		RtlInitUnicodeString(&input, L"ntdll.dll");



		//		pLoadLibraryA = (LoadLibraryAF)GetExportAddress(GetModuleBaseAddress(kernel32), loadlibrarya);


		NTSTATUS stat = ldll(NULL, NULL, &input, &hForward);

		//pLoadLibraryA = (LoadLibraryAF)GetExportAddress(GetModuleBaseAddress(L"KERNEL32.DLL"), "LoadLibraryA");

		if (pLoadLibraryA == NULL)
			return NULL;

		hForward = pLoadLibraryA(szDllName);
		free(szDllName);

		if (!hForward)
			return NULL;

		pAddress = (FARPROC*)GetExportAddress(hForward, szFunctionName);
	}

	return pAddress;
}