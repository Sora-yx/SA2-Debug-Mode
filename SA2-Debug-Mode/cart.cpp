#include "stdafx.h"

Trampoline* CartExecuter_Main_t;

cartStruct* cartPointer;
char cartCO2Copy[0x2F8];
extern bool isCartSaved;

void DeleteKartPointer() {

	if (cartPointer)
	{
		delete(cartPointer);
	}

	cartPointer = nullptr;
}

void SaveCartPointer() {

	if (cartPointer)
	{
		memcpy(cartCO2Copy, cartPointer, 0x2F8);
		return;
	}
}

void __cdecl CartExecuter_Main_r(ObjectMaster* obj)
{

	TARGET_DYNAMIC(CartExecuter_Main)(obj);

	if (CurrentLevel != LevelIDs_KartRace) {

		if (isCartSaved)
		{
			obj->EntityData2 = (UnknownData2*)cartCO2Copy;
			isCartSaved = false;
		}

		cartPointer = (cartStruct*)obj->EntityData2;
	}
}

cartStruct* getCartPointer() {
	return cartPointer;
}

void init_CartHack() {

	CartExecuter_Main_t = new Trampoline((int)CartExecuter_Main, (int)CartExecuter_Main + 0x6, CartExecuter_Main_r);
}