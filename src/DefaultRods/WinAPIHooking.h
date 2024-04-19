#pragma once
#include <wtypes.h>
#include "Helpers.h"

//Example class for hoooking MessageBoxA
//This will also show you some more or less guidelines to create your own stuff.
//Al this is not set in stone of course, you can do whatever you want, that is what programming is about afterall!

class WinAPIHooking
{
public:
	void Init() const;

private:
	//Can expose these to the public scope if needed
#pragma region TypesAndOriginals
	using MessageBoxA_t = int(WINAPI*)(HWND, LPCSTR, LPCSTR, UINT);
	static MessageBoxA_t MessageBoxA_o;
#pragma endregion

#pragma region Hooks
	static int MessageBoxA_hook(
		HWND hWnd,
		LPCSTR lpText,
		LPCSTR lpCaption,
		UINT uType);
#pragma endregion
};
