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
		objSave = config->getBool("General", "objSave", false);
		isDebugTxt = config->getBool("General", "isDebugTxt", true);

		delete config;

		Collisions_Init();
		initializeDebugText();
		init_FreeCamHack();

		if (isDebugTxt) {
			init_treasureHuntingDebug();
		}

		if (isSave) {
			init_SaveState();
			init_CartHack();
		}
	}

	__declspec(dllexport) void __cdecl OnFrame() {


		if (GameState == GameStates_LoadItems) {
			LoadDebuggingObjects();
		}

		DisplayDebugTextInfo();
		Save_Pause();

	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		
		Debugging_InputCheck();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
