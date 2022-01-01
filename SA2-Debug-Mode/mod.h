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

VoidFunc(ResetSetDataFlag, 0x488AD0);
VoidFunc(DeleteMostObjects, 0x470AE0);

DataArray(char, PosRotBufferIndex, 0x1945910, 2);
DataPointer(char, camConstPastPosIDX, 0x19f173c);
DataPointer(__int16, ActiveLandTableColCount, 0x1DE9484);
void Save_Pause();

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target()) //sf94

#pragma pack(push, 1)
struct cartStruct
{
	NJS_POINT3 Velocity;
	Float idk;
	Float idk2;
	NJS_VECTOR Position;
	Rotation Forward;
	Float DistanceWall;
	int KartAction;
	Float idk3;
	Float SpeedY;
	Float SpeedX;
	Float Accel;
	Float idk5;
};
#pragma pack(pop)

