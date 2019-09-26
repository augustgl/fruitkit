#pragma once

#include "vxheader.h"

BOOL IsInsideVirtualBox()
{
	HKEY HK = 0;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ, &HK) == ERROR_SUCCESS)
	{
		return TRUE;
	}
}

BOOL vmware_artifact_check() {
	HKEY hKey;
	char lszValue[256];

	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum", 0L, KEY_READ, &hKey);
	RegQueryValueA(hKey, "0", lszValue, (PLONG)sizeof(lszValue));

	RegCloseKey(hKey);

	if (strstr(lszValue, "VMware")) {
		return TRUE;
	}
}