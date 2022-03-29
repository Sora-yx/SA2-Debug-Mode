#pragma once

DataPointer(int, CartTimer, 0x1D94428);
DataPointer(char, HudSpecialEnabled, 0x0174AFCB);
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
DataPointer(char, ShowHud, 0x0174AFCC);
DataPointer(int, dword_1A558BC, 0x1A558BC);
DataPointer(int, dword_17472BC, 0x17472BC);
DataPointer(int, dword_1A558B8, 0x1A558B8);
DataPointer(int, dword_1AEE5AC, 0x1AEE5AC);
DataPointer(int, dword_17483FC, 0x17483FC);


static const void* const sub_426620Ptr = (void*)0x426620;
static inline void sub_426620(int result, unsigned __int8 a2, unsigned __int8 a3)
{

	__asm
	{
		mov cl, [a3]
		mov dl, [a2]
		mov eax, [result]
		call sub_426620Ptr
	}
}