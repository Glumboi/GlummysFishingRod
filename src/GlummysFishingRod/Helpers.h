#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <iostream>

class Helpers
{
public:
	static DWORD GetProcessIdByName(const char* processName);
	static VOID DoConsole();
	static VOID InjectHooksToExistingProcess(const char* processExe, const char* dll);
	static VOID InjectHooksToNewProcess(const char* executable, const char* dll);
private:
	static bool InjectDll(HANDLE hProcess, const char* dll);
};
