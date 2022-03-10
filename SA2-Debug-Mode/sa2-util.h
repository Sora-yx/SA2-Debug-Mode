#pragma once

static const void* const DrawSpritePtr = (void*)0x44EB00;
static inline void Draw2DSprite(NJS_SPRITE* a1)
{
	__asm
	{
		mov eax, [a1]
		call DrawChunkModelPtr
	}
}

DataPointer(int, CartTimer, 0x1D94428);
DataPointer(char, PauseEnabled, 0x174AFD6);
DataPointer(float, HorizontalStretch, 0x1A559E4);
DataPointer(float, VerticalStretch, 0x1733ABC);

DataPointer(float, GlobalMatColorR, 0x25EFFD4);
DataPointer(float, GlobalMatColorG, 0x25EFFD8);
DataPointer(float, GlobalMatColorB, 0x25EFFDC);
DataPointer(float, GlobalMatColorA, 0x25EFFD0);

DataPointer(int, DeathZoneDebug, 0x174B002);


VoidFunc(ResetSetDataFlag, 0x488AD0);
VoidFunc(DeleteMostObjects, 0x470AE0);

DataArray(char, PosRotBufferIndex, 0x1945910, 2);
DataPointer(char, camConstPastPosIDX, 0x19f173c);
DataPointer(__int16, ActiveLandTableColCount, 0x1DE9484);


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



struct CameraInfo_
{
	int field_0;
	int field_4;
	int field_8;
	char gapC[328];
	float field_154;
	float field_158;
	float field_15C;
	int field_160;
	char gap164[48];
	NJS_VECTOR Position;
	Rotation Rotation;
	NJS_VECTOR idk;
	NJS_VECTOR idk2;
	NJS_VECTOR PosAgain;
	char gap1AC[8964];
	int field_24D4;
};

DataPointer(float, flt_1DCFF3C, 0x1DCFF3C);
DataPointer(NJS_VECTOR, camPosR, 0x1DCFF0C);
DataPointer(Rotation, camRotR, 0x1DCFF18);
DataPointer(NJS_VECTOR, camIdk, 0x1DCFF24);
DataPointer(NJS_VECTOR, camTarget, 0x1DCFF30);
FastcallFunctionPointer(void, CameraFollowCharacter, (int playID), 0x4EC770);
DataPointer(char, cam_handle, 0x01DCFF00);

DataPointer(NJS_VECTOR, CamEventPos, 0x1DCFE10);
DataPointer(int, CamEventAngleZ, 0x1DCFDF8);
DataPointer(int, CamEventAngleY, 0x1DCFDFC);