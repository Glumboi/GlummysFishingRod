#include "Helpers.h"
#include "ini.h"
#include <filesystem>
#include <iostream>
#include <TlHelp32.h>
#include <Windows.h>

#define INI_FILE "fishingRod.ini"

#define ABORT(waitTime) \
{\
Sleep(waitTime); \
exit(0); \
}

//TODO: Break up into smaller functions
void Init()
{
	mINI::INIFile iniFile(INI_FILE);
	mINI::INIStructure iniStruct;

	if (!std::filesystem::exists(INI_FILE))
	{
		iniStruct["Config"]["TargetExecutable"] = "UNDEFINED";
		iniStruct["Config"]["RoddingDll"] = "DefaultRods";
		iniStruct["Config"]["NoConsole"] = "0";
		iniFile.generate(iniStruct, true);
		std::cout << "Didn't find an ini file, creating default...\n";
	}

	if (iniFile.read(iniStruct))
	{
		std::cout << "Reading fishingrod config...\n";

		std::string targetExe = iniStruct["Config"]["targetExecutable"];
		std::string roddingDll = iniStruct["Config"]["RoddingDll"];

		if (bool noConsole = std::stoi(iniStruct["Config"]["NoConsole"].c_str());
			!noConsole)
		{
			Helpers::DoConsole();
		}

		if (targetExe == "UNDEFINED" || roddingDll == "UNDEFINED")
		{
			std::cout << "One or more targets weren't specified, aborting rodding in 5 seconds!\n";
			ABORT(5000);
		}

		targetExe.append(".exe");
		roddingDll.append(".dll");

		std::cout << "Starting rodding process for: " << targetExe << "\n";

		char buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);
		std::string workingDir = std::string(buffer);
		std::string hookBaits = workingDir + "\\";
		hookBaits.append(roddingDll);

		if (!std::filesystem::exists(hookBaits))
		{
			std::cout << hookBaits << " is not existing in the specified location, aborting in 5 seconds..." << "\n";
			ABORT(5000);
		}

		if (!OpenProcess(
			PROCESS_ALL_ACCESS,
			FALSE,
			Helpers::GetProcessIdByName(targetExe.c_str())))
		{
			//Launch target if not running (target must be in same folder as fishingrod.exe)
			std::cout << targetExe << " is not running, starting process..." << "\n";
			std::string executable = workingDir + ("\\" + targetExe);

			if (!std::filesystem::exists(executable))
			{
				std::cout << executable << " is not existing in the specified location, aborting in 5 seconds..." << "\n";
				ABORT(5000);
			}
			//Launches a new process, doesnt inject to exising
			Helpers::InjectHooksToNewProcess(executable.c_str(), hookBaits.c_str());
		}
		else
		{
			Helpers::InjectHooksToExistingProcess(targetExe.c_str(), hookBaits.c_str());
		}
		return;
	}
	std::cout << "Could not read ini file, aborting rodding in 5 seconds!\n";
	ABORT(5000);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Init();
	std::cout << "Done! (auto closing in 5 seconds...)\n";
	ABORT(5000);
}