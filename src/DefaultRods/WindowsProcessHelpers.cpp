#include <iostream>
#include "WindowsProcessHelpers.h"

DWORD WindowsProcessHelpers::GetProcessIdByName(const char* processName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &processEntry))
	{
		do
		{
			if (_stricmp(processEntry.szExeFile, processName) == 0)
			{
				CloseHandle(snapshot);
				return processEntry.th32ProcessID;
			}
		} while (Process32Next(snapshot, &processEntry));
	}

	CloseHandle(snapshot);
	return 0;
}

HMODULE WindowsProcessHelpers::ForceGetHmodule(const char* moduleName, bool isInApplicationFolder)
{
	HMODULE returnModule = GetModuleHandleA(moduleName);

	if (!returnModule)
	{
		DEBUGTOFILEANDCONSOLE("Force loading " << moduleName << "\n");
		char buff[MAX_PATH];

		if (isInApplicationFolder)
		{
			GetCurrentDirectoryA(MAX_PATH, buff);
			strcat_s(buff, "\\");
		}
		else
		{
			GetWindowsDirectoryA(buff, MAX_PATH);
			strcat_s(buff, "\\system32\\");
		}

		strcat_s(buff, moduleName);
		returnModule = LoadLibraryA(buff);
	}

	return returnModule;
}