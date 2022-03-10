#pragma once

extern bool isSave;
extern bool isDebugTxt;
extern bool objSave;

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

void FreeCam_CheckInput();
void FreeCam_OnInput();
void Save_Pause();
void SendTimedDebugMessage(std::string msg, int timer);

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target()) //sf94
