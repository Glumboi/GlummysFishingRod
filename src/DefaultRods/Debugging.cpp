#include "Debugging.h"

LONG __stdcall CatchAllExceptionsFilter(_EXCEPTION_POINTERS* exceptionInfo)
{
	DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
	PVOID exceptionAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;

	// Get the base address of the module containing the exception address
	HMODULE hModule;
	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		static_cast<LPCSTR>(exceptionAddress), &hModule))
	{
		// Get the offset by subtracting the base address of the module from the exception address
		DWORD_PTR moduleBase = reinterpret_cast<DWORD_PTR>(hModule);
		DWORD_PTR functionOffset = reinterpret_cast<DWORD_PTR>(exceptionAddress) - moduleBase;

		DEBUGTOFILEANDCONSOLE(
			"An exception occurred in the rodding process!\n"
			<< "Exception code: 0x" << std::hex << exceptionCode << "\n"
			<< "Exception address: 0x" << exceptionAddress << "\n"
			<< "Function offset: 0x" << functionOffset << "\n");
	}
	else
	{
		DEBUGTOFILEANDCONSOLE(
			"An exception occurred in the rodding process!\n"
			<< "Exception code: 0x" << std::hex << exceptionCode << "\n"
			<< "Exception address: 0x" << exceptionAddress << "\n"
			<< "Unable to determine function offset.");
	}

	DEBUGTOFILEANDCONSOLE("Aborting after exception...");

	MessageBox(NULL,
		"An Exception happened in the rodding process!\nMore info in the console output.",
		"Error",
		MB_OK | MB_ICONERROR);

	return EXCEPTION_EXECUTE_HANDLER;
}