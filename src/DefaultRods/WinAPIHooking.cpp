#include "WinAPIHooking.h"

WinAPIHooking::MessageBoxA_t WinAPIHooking::MessageBoxA_o = nullptr;

void WinAPIHooking::Init() const
{
	//ForceGetModuleAndPrint defaults to system32 if isInApplicationFolder is false
	HMODULE user32Mod = Helpers::ForceGetModuleAndPrint("User32.dll");

	//Assign the exported function address to our original type
	Helpers::AssignAddressToOriginalUsingModule(MessageBoxA_o, "MessageboxA", user32Mod);
	//Make a minhook hook/detour with the assigned original and it's hook
	MAKE_HOOK(MessageBoxA_o, MessageBoxA_hook, "MessageBoxA_hook");
}

int WinAPIHooking::MessageBoxA_hook(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType)
{
	//Modifying the original parameters, there are way more capiblities of changing behaviour though
	lpText = "Changed from rod";
	lpCaption = "Changed from rod";
	uType = MB_OK | MB_ICONERROR;

	//How to call the original
	int originalResult = MessageBoxA(
		hWnd, lpText,
		lpCaption, uType);

	return originalResult;
}