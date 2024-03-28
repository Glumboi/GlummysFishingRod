#pragma once
#include "Debugging.h"
#include "WindowsProcessHelpers.h"
#include <codecvt>
#include <locale>
#include <Minhook.h>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <Windows.h>

#define MAKE_HOOK(original, hook, name)\
DEBUGTOFILEANDCONSOLE("Created " << name << " with status: " <<\
MH_CreateHook(original, &hook, reinterpret_cast<LPVOID*>(&original)));

class Helpers
{
public:
	static inline std::string LPCWSTR_to_string(const LPCWSTR wideStr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wideStr);
	}

	static inline HMODULE ForceGetModuleAndPrint(const char* moduleName, bool isInApplicationFolder = false)

	{
		HMODULE mod = WindowsProcessHelpers::ForceGetHmodule(moduleName, isInApplicationFolder);
		DEBUGTOFILEANDCONSOLE(moduleName << ": " << mod << "\n");
		return mod;
	}

	static inline MODULEENTRY32 GetModuleEntry(const char* modName, DWORD proc_id)
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!strcmp(modEntry.szModule, modName))
					{
						CloseHandle(hSnap);
						return modEntry;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
		MODULEENTRY32 module = { -1 };
		return module;
	}

	static DWORD GetModuleBase(DWORD processId, const char* szModuleName);

	template<typename targetType>
	static inline VOID AssignAddressToOriginalUsingPointer(
		uintptr_t targetAddress,
		const char* targetName,
		typename targetType* target)
	{
		*target = reinterpret_cast<targetType>(targetAddress);
		DEBUGTOFILEANDCONSOLE("\nTarget address of " << targetName << " is " << std::hex << targetAddress);
	}

	template<typename targetType>
	inline static VOID AssignAddressToOriginalUsingModule(
		targetType& target,
		const char* targetName,
		HMODULE hModule)
	{
		if (hModule != nullptr)
		{
			FARPROC targetAddress = GetProcAddress(hModule, targetName);
			if (targetAddress != nullptr)
			{
				DEBUGTOFILEANDCONSOLE("\nTarget address of " << targetName << " is " << std::hex << targetAddress);
				target = reinterpret_cast<targetType>(targetAddress);
			}
			else
			{
				DEBUGTOFILEANDCONSOLE("\nFailed to get address of " << targetName << " from module " << hModule << ". Error code: " << GetLastError());
			}
		}
		else
		{
			DEBUGTOFILEANDCONSOLE("\nFailed to load module " << hModule << ". Error code: " << GetLastError());
		}
	}

	template<typename targetType>
	inline static VOID AssignAddressToOriginalUsingPatternScan(
		targetType& target,
		const char* targetName,
		HMODULE hModule,
		const char* pattern)
	{
		if (hModule != nullptr)
		{
			target = (targetType)Helpers::PatternScan(
				hModule,
				pattern);

			if (target == nullptr)
			{
				DEBUGTOFILEANDCONSOLE("\nCould not search target address of " << targetName << " with pattern scan");

				return;
			}

			DEBUGTOFILEANDCONSOLE("\nTarget address of " << targetName << " (searched via pattern scan) is " << std::hex << target);
		}
		else
		{
			DEBUGTOFILEANDCONSOLE("\nFailed to load module " << hModule << ". Error code: " << GetLastError());
		}
	}

	static inline VOID DoConsole(const char* consoleTitle)
	{
		AllocConsole();
		SetConsoleTitleA(consoleTitle);
		FILE* f;
		FILE* f2;
		freopen_s(&f, "conout$", "w", stdout);
		freopen_s(&f2, "conout$", "w", stderr);
	}

	static uint8_t* PatternScan(HMODULE module, const char* signature);
};