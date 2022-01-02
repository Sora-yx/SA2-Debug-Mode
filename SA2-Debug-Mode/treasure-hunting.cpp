#include "stdafx.h"

Trampoline* EmeraldPiece_main_t;
Trampoline* EmeraldManager_Main_t;
Trampoline* EmeraldPiece_Load_t;
Trampoline* DrawEmeraldPiece_t;

DataPointer(NJS_CNK_MODEL, EmeraldModel, 0xB42DF4);

extern bool isFreeMov;

void EmeraldPiece_Display_r(ObjectMaster* obj) {

	if (!isFreeMov)
		return;

	EntityData1* v1 = obj->Data1.Entity;

	njSetTexture(texlist_objtex_common);
	njPushMatrix(0);
	njTranslateV(0, &v1->Position);
	njRotateZXY(&v1->Rotation);
	DrawChunkModel(&EmeraldModel);
	njPopMatrix(1u);
}

void EmeraldPiece_main_r(ObjectMaster* obj) {

	if (obj->DisplaySub_Delayed4 == nullptr)
		obj->DisplaySub_Delayed4 = EmeraldPiece_Display_r;

	EntityData1* data = obj->Data1.Entity;
	data->Rotation.x = data->Rotation.x;
	int num = data->Rotation.x % 11;

	num = num;

	ObjectFunc(origin, EmeraldPiece_main_t->Target());
	origin(obj);
}

void __cdecl DrawEmeraldPiece_r(NJS_VECTOR* pos, signed int* a2, int arg)
{

	FunctionPointer(void, original, (NJS_VECTOR * pos, signed int* a2, int arg), DrawEmeraldPiece_t->Target());
	original(pos, a2, arg);
}

extern bool isEmePtrSaved;
char EmePtrCopy[116];
EmeManObj2* emeraldPtrCopy = nullptr;


void SaveEmePtr() {

	if (EmeraldManagerObj2)
	{
		memcpy(EmePtrCopy, EmeraldManagerObj2, 116);
		return;
	}
}


void EmeraldPiece_Load_r(NJS_VECTOR* a1, unsigned __int8 a2, unsigned __int8 a3) {

	FunctionPointer(void, original, (NJS_VECTOR* a1, unsigned __int8 a2, unsigned __int8 a3), EmeraldPiece_Load_t->Target());
	original(a1, a2, a3);
}

DataPointer(void*, somethingAboutHint, 0x01AF016C);
DataPointer(int, curHintFileIdk, 0x1AF0170);

char* getHintText_r(void* a1, char pieceID)
{
	char* result; // eax
	char* v2; // edx
	EmeManThing* copyHintFile; // ebp
	int v4; // ecx
	char* v5; // eax
	int v6; // esi
	int v7; // edi
	char v8; // al
	int v9; // eax
	int hintCount;

	if (!EmeraldManagerObj2)
	{
		return 0;
	}
	v2 = (char*)somethingAboutHint;
	if (!somethingAboutHint)
	{
		return 0;
	}
	copyHintFile = (EmeManThing*)curHintFileIdk;
	if (!curHintFileIdk)
	{
		return 0;
	}
	v4 = 0;
	v5 = &EmeraldManagerObj2->byte2C[pieceID].byte1;

	if (!v5)
		return 0;

	do
	{
		v6 = *(v5 - 1);
		v7 = *v5;
		if (v6 != -2)
		{
			break;
		}
		++v4;
		v5 += 16;
	} while (v4 < 3);
	v8 = *(BYTE*)somethingAboutHint;
	if (*(BYTE*)somethingAboutHint == 0xFF)
	{
		return (char*)a1;
	}
	while (v8 != v6 || v2[1] != v7)
	{
		v8 = v2[8];
		v2 += 8;
		if (v8 == -1)
		{
			return (char*)a1;
		}
	}
	v9 = 0;
	if (v9 < 3)
	{
		hintCount = v9 + 1;
	}
	else
	{
		hintCount = 3;
	}
	if (hintCount <= 0)
	{
		result = 0;
	}
	else
	{
		result = &copyHintFile->byte0 + *((DWORD*)&copyHintFile->byte0 + *(__int16*)&v2[2 * hintCount]);
		result = result + 3;
	}

	return result;
}

void init_treasureHuntingDebug() {
	EmeraldPiece_main_t = new Trampoline((int)EmeraldPiece_Main, (int)EmeraldPiece_Main + 0x5, EmeraldPiece_main_r);
}