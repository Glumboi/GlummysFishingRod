#include "Helpers.h"

DWORD Helpers::GetProcessIdByName(const char* processName)
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

VOID Helpers::DoConsole()
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "conout$", "w", stdout);
	freopen_s(&f, "conout$", "w", stderr);
}

VOID Helpers::InjectHooksToExistingProcess(const char* processExe, const char* dll)
{
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		Helpers::GetProcessIdByName(processExe));
	InjectDll(hProcess, dll);
}

VOID Helpers::InjectHooksToNewProcess(const char* executable, const char* dll)
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof startupInfo;
	ZeroMemory(&processInformation, sizeof(processInformation));

	BOOL prCreate = CreateProcess(executable,
		nullptr,
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&startupInfo,
		&processInformation);

	auto processExe = std::string(executable);
	size_t start = processExe.find_last_of('\\') + 1;
	processExe = processExe.substr(start, processExe.length() - start);

	if (prCreate)
	{
		WaitForSingleObject(processInformation.hProcess, 200);
		InjectDll(processInformation.hProcess, dll);
		CloseHandle(processInformation.hThread);
		CloseHandle(processInformation.hProcess);
	}
	else
	{
		DWORD error = GetLastError();
		std::cerr << "CreateProcess failed with error " << error << "\n";
		MessageBox(nullptr, "Cannot open process!", "Fail!", MB_OK | MB_ICONERROR);
	}
}

bool Helpers::InjectDll(HANDLE hProcess, const char* dll)
{
	if (hProcess != nullptr)
	{
		PVOID allocatedPath = VirtualAllocEx(hProcess, nullptr, strlen(dll) + 1, MEM_COMMIT, PAGE_READWRITE);

		if (allocatedPath != nullptr)
		{
			WriteProcessMemory(hProcess, allocatedPath, dll, strlen(dll) + 1, nullptr);

			HANDLE processthread = CreateRemoteThread(hProcess, nullptr, 0,
				(LPTHREAD_START_ROUTINE)GetProcAddress(
					GetModuleHandle("kernel32"), "LoadLibraryA"),
				allocatedPath, 0, nullptr);

			if (processthread != nullptr)
			{
				WaitForSingleObject(processthread, 200);

				char buff[MAX_PATH];
				ReadProcessMemory(hProcess, allocatedPath, buff, strlen(dll) + 1, nullptr);
				std::cout << "Allocated space value: " << buff << "\n";

				CloseHandle(processthread);
			}

			VirtualFreeEx(hProcess, allocatedPath, 0, MEM_RELEASE);
		}
		else
		{
			std::cerr << "Failed to allocate memory in the remote process\n";
			return false;
		}

		CloseHandle(hProcess);
		return true;
	}
	else
	{
		DWORD error = GetLastError();
		std::cerr << "OpenProcess failed with error " << error << "\n";
		return false;
	}
}