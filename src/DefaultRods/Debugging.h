#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>

#define LOGFILE "DefaultRod_log.txt"

#define TOPSEPERATOR "================================================\n"
#define BOTTOMSEPERATOR "\n================================================\n"

//This is very bad code, but it works, so fuck it
#define DEBUGTOFILEANDCONSOLE(msg) \
{ \
    std::ostringstream debug_output; \
    debug_output << msg << "\n"; \
    std::cout << debug_output.str(); \
    std::ofstream log(LOGFILE, std::ios_base::app | std::ios_base::out); \
    log << debug_output.str(); \
    log << "\n"; \
    log.close();\
}

#define DEBUGTOFILEANDCONSOLE_W(msg1) \
{ \
    std::wostringstream debug_output_w; \
    debug_output_w << msg1 << "\n"; \
    std::wcout << debug_output_w.str(); \
    std::wofstream log1(LOGFILE, std::ios_base::app | std::ios_base::out); \
    log1 << debug_output_w.str(); \
    log1 << "\n"; \
    log1.close();\
}

LONG WINAPI CatchAllExceptionsFilter(struct _EXCEPTION_POINTERS* exceptionInfo);