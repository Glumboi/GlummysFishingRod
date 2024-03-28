#pragma once
#include "WinAPIHooking.h"

constexpr const char* const CONSOLE_TITLE = "DefaultRods";

class Hooking
{
public:
	static void Initialize();
private:
	static WinAPIHooking winAPIHooking;
};
