#include "stdafx.h"

Trampoline* EmeraldPiece_main_t;
Trampoline* EmeraldManager_Main_t;
Trampoline* EmeraldPiece_Load_t;
Trampoline* idk_t;
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

NJS_VECTOR* emerald1Pos;
int id = 0;

void DisplayEmeraeldLocation() {



	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "Piece 3 loc : %f", emerald1Pos->x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "Piece 3 loc : %f", emerald1Pos->y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 11), "Piece 3 loc : %f", emerald1Pos->z);
}

DataArray(int, dword_B43448, 0xB43448, 1024);
void EmeraldPiece_main_r(ObjectMaster* obj) {

	if (obj->DisplaySub_Delayed4 == nullptr)
		obj->DisplaySub_Delayed4 = EmeraldPiece_Display_r;

	id = dword_B43448[9 * (obj->Data1.Entity->Rotation.x) % 11];


	ObjectFunc(origin, EmeraldPiece_main_t->Target());
	origin(obj);
}

void __cdecl DrawEmeraldPiece_r(NJS_VECTOR* pos, signed int* a2, int arg)
{

	FunctionPointer(void, original, (NJS_VECTOR * pos, signed int* a2, int arg), DrawEmeraldPiece_t->Target());
	original(pos, a2, arg);


	DisplayDebugStringFormatted(NJM_LOCATION(3, 12), "Piece loc X : %f", pos->x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13), "Piece loc Y: %f", pos->y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 14), "Piece  loc Z : %f", pos->z);
}



void EmeraldPiece_Load_r(NJS_VECTOR* a1, unsigned __int8 a2, unsigned __int8 a3) {



	FunctionPointer(void, original, (NJS_VECTOR* a1, unsigned __int8 a2, unsigned __int8 a3), EmeraldPiece_Load_t->Target());
	original(a1, a2, a3);
}

void init_treasureHuntingDebug() {
	EmeraldPiece_main_t = new Trampoline((int)EmeraldPiece_Main, (int)EmeraldPiece_Main + 0x5, EmeraldPiece_main_r);
	DrawEmeraldPiece_t = new Trampoline((int)0x6D0160, 0x6D016A, DrawEmeraldPiece_r);

	WriteData<10>((int*)0x6a27c0, 0x90); //test lol
}