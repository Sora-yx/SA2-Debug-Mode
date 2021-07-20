#pragma once

extern bool isSave;
extern bool isDebugTxt;

extern HelperFunctions HelperFunctionsGlobal;
void Collisions_Init();
void Debugging_InputCheck();
void CheckController_ColDebug();
void DebugTextUpdateCurrentPage();

void DisplayDebugTextInfo();
void LoadObjDrawBG();
void LoadObjSaveState();
void LoadDebuggingObjects();
void init_treasureHuntingDebug();
void init_SaveState();

VoidFunc(ResetSetDataFlag, 0x488AD0);
VoidFunc(DeleteMostObjects, 0x470AE0);