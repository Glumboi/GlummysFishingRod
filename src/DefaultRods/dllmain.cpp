#include <wtypes.h>
#include "Helpers.h"
#include "Hooking.h"

VOID Run()
{
	Helpers::DoConsole(CONSOLE_TITLE);
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hooking::Initialize, nullptr, 0, nullptr);
}

extern "C" {
	VOID RunFromImport()
	{
		Run();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Run();
	case DLL_THREAD_ATTACH:
		//handle detaching and so on, some apps might randomly detach during execution, thus I didn't use it here
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}