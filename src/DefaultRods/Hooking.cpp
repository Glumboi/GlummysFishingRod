#include "Debugging.h"
#include "Hooking.h"
#include <fileapi.h>
#include <MinHook.h>

WinAPIHooking Hooking::winAPIHooking;

void Hooking::Initialize()
{
	DeleteFile(LOGFILE); //Clear log from last session

	//Catch all exceptions,
	//remove after debugging to ensure a fluid handling of errors by target executable
	SetUnhandledExceptionFilter(CatchAllExceptionsFilter);

	DEBUGTOFILEANDCONSOLE("=== " << CONSOLE_TITLE << " ===\n\n");

	MH_STATUS initStat = MH_Initialize();

	if (initStat == MH_OK)
	{
		//Your hooks go here

		//Example hooking (this one seems to be broken currently, investigating soon):
		winAPIHooking.Init();
	}
}