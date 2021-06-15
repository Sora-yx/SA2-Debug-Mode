#include "stdafx.h"


HelperFunctions HelperFunctionsGlobal;

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		HelperFunctionsGlobal = helperFunctions;
		Collisions_Init();
		initializeDebugText();
	}


	__declspec(dllexport) void __cdecl OnFrame() {

		if (GameState != GameStates_Ingame && GameState != GameStates_Pause)
			return;

		DisplayDebugTextInfo();

	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		Debugging_InputCheck();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}