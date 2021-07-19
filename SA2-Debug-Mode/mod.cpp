#include "stdafx.h"

bool isSave;
bool isDebugTxt;
bool isFreemov;

HelperFunctions HelperFunctionsGlobal;

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		isSave = config->getBool("General", "isSave", true);
		isDebugTxt = config->getBool("General", "isDebugTxt", true);
		isFreemov = config->getBool("General", "isFreemov", true);
		delete config;

		Collisions_Init();

		if (isDebugTxt) {
			initializeDebugText();
			init_treasureHuntingDebug();
		}

		if (isSave)
			init_SaveState();
	}


	__declspec(dllexport) void __cdecl OnFrame() {

		if (GameState == GameStates_LoadItems) {
			LoadDebuggingObjects();
		}

		if (GameState != GameStates_Ingame && GameState != GameStates_Pause && GameMode != 14)
			return;

		DisplayDebugTextInfo();
		//DrawRecMaybe((int)dword_1A55924, 191.0, 145.0, 256.0, 256.0, 0.99989998, 0.0, 0.0, 1.0, 1.0, -1);

	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		Debugging_InputCheck();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}