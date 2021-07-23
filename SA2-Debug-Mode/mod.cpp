#include "stdafx.h"

bool isSave;
bool isDebugTxt;
bool objSave;

HelperFunctions HelperFunctionsGlobal;

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		isSave = config->getBool("General", "isSave", true);
		objSave = config->getBool("General", "objSave", true);
		isDebugTxt = config->getBool("General", "isDebugTxt", true);


		delete config;

		Collisions_Init();
		initializeDebugText();

		if (isDebugTxt) {
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
		Save_Pause();

	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		Debugging_InputCheck();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}