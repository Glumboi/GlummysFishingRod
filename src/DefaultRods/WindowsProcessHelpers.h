#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "debugging.h"

class WindowsProcessHelpers
{
public:
	static DWORD GetProcessIdByName(const char* processName);
	static HMODULE ForceGetHmodule(const char* moduleName, bool isInApplicationFolder = false);
};
