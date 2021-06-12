#include "stdafx.h"

HelperFunctions HelperFunctionsGlobal;


extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{

		HelperFunctionsGlobal = helperFunctions;
		Collisions_Init();

	}


	__declspec(dllexport) void __cdecl OnFrame() {


	}


	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}