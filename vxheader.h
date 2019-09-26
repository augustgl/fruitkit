#define _WIN32_LEAN_AND_MEAN

#pragma once

#include <winsock.h>
#include <Windows.h>
#include <winternl.h>
#include <malloc.h>


#pragma comment(lib, "ws2_32.lib")


typedef int (WINAPI * MyRegOpenKeyA)(HKEY hKey, LPCSTR SubKey, PHKEY phkResult);
typedef int (WINAPI * MyRegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
typedef int (WINAPI * MyRegCloseKey)(HKEY hKey);
typedef int (WINAPI * MyRegQueryValueA)(HKEY hKey, LPCSTR lpSubKey, LPSTR lpData, PLONG lpcbData);
typedef BOOL(WINAPI * MyIsUserAnAdmin)();
typedef DWORD(WINAPI * MyGetModuleFileNameA)(HMODULE hModule, LPSTR lpFileName, DWORD nSize);
typedef BOOL(WINAPI * MyCreateProcessA)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributues, BOOL dInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, PPROCESS_INFORMATION lpProcessInformation);
typedef UINT(WINAPI * MyWinExec)(LPCSTR lpCmdLine, UINT nCmdShow);

typedef HMODULE(WINAPI *MyLoadLibrary)(LPCWSTR lpFileName);
typedef FARPROC(WINAPI *MyGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);

typedef ULONG(NTAPI *RtlNtStatusToDosErrorType)(NTSTATUS Status);
typedef VOID(NTAPI *RtlInitAnsiStringType)(PANSI_STRING DestinationString, PCSZ SourceString);
typedef VOID(NTAPI *RtlInitUnicodeStringType)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

typedef NTSTATUS(NTAPI *LdrLoadDll)(PWCHAR PathToFile, ULONG *Flags, UNICODE_STRING *ModuleFileName, HMODULE *ModuleHandle);
typedef NTSTATUS(NTAPI *LdrGetProcedureAddress)(HMODULE ModuleHandle, PANSI_STRING FunctionName, WORD Oridinal, PVOID *FunctionAddress);

typedef NTSTATUS(NTAPI *LdrLoadDll)(PWCHAR PathToFile, ULONG *Flags, UNICODE_STRING *ModuleFileName, HMODULE *ModuleHandle);
typedef NTSTATUS(NTAPI *LdrGetProcedureAddress)(HMODULE ModuleHandle, PANSI_STRING FunctionName, WORD Oridinal, PVOID *FunctionAddress);