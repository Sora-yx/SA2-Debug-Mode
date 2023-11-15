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


DataPointer(NJS_VECTOR, camPos, 0x01DCFF0C);
DataPointer(NJS_VECTOR, camRot, 0x01DCFF18);
DataPointer(NJS_VECTOR, camIDK, 0x01DCFF24);
DataPointer(NJS_VECTOR, camTarget, 0x01DCFF30);

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
ThiscallFunctionPointer(int, PauseSound, (UINT this_), 0x436020);

DataPointer(ObjectListHead*, SETFile_, 0x01DDE274);

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


static const void* const AddConstantAttrPtr = (void*)0x446CF0;
static inline void AddConstantAttr(int a1, int a2)
{

	__asm
	{
		mov eax, [a2]
		mov ecx, [a1]
		call AddConstantAttrPtr
	}
}


VoidFunc(sub_42D340, 0x42D340);
DataPointer(int, nj_constant_attr_and_, 0x01DEB6A8);
DataPointer(int, nj_constant_attr_or_, 0x01DEB6A0);
DataPointer(char, PauseDisabled, 0x0174AFD6);

using Angle3 = Rotation;
struct taskwk;

struct c_colli_hit_info
{
	char my_num;
	char hit_num;
	unsigned __int16 flag;
	taskwk* hit_twp;
};

struct CCL_INFO
{
	char kind;
	char form;
	char push;
	char damage;
	unsigned int attr;
	NJS_POINT3 center;
	float a;
	float b;
	float c;
	float d;
	int angx;
	int angy;
	int angz;
};

struct colliwk
{
	unsigned __int16 id;
	__int16 nbHit;
	unsigned __int16 flag;
	unsigned __int16 nbInfo;
	float colli_range;
	CCL_INFO* info;
	c_colli_hit_info hit_info[16];
	NJS_POINT3 normal;
	ObjectMaster* mytask;
	__int16 my_num;
	__int16 hit_num;
	colliwk* hit_cwp;
};

struct taskwk
{
	char mode;
	char smode;
	char id;
	char btimer;
	__int16 flag;
	unsigned __int16 wtimer;
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
	colliwk* cwp;
};