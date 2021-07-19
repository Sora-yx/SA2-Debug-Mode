#include "stdafx.h"

bool isFreeMov = false;

void DrawFreeMovementText() {

	if (!isFreeMov)
		return;

	DisplayDebugStringFormatted(NJM_LOCATION(22, 10), "FREE MOVEMENT ENABLED");
	return;
}

//The hunters originally can't use free movements since their action is shared with something else, so we force a different action and manually call the function.
void Hunters_FreeMovements() {

	if (!isFreeMov)
		return;

	for (int i = 0; i < 2; i++) {

		if (MainCharObj2[i]) {
			if (MainCharObj2[i]->CharID == Characters_Knuckles || MainCharObj2[i]->CharID == Characters_Rouge) {

				if (MainCharObj1[i]->Action == 120)
				{
					FreeMovement_Tails(MainCharObj2[i], MainCharObj1[i]);
				}
				else {
					return;
				}
			}
		}
	}

	return;
}

void SetFreeMovements() {

	if (isFreeMov)
		return;

	for (int i = 0; i < 2; i++) {

		if ( (Controllers[i].on & Buttons_A && Controllers[i].press & Buttons_Y)) {

			isFreeMov = true;
			DeathZoneDebug = 1; //death zone can no longer kill player
			Life_Count[i] = 99;

			if (MainCharObj2[i]->CharID == Characters_Knuckles || MainCharObj2[i]->CharID == Characters_Rouge)
				MainCharObj1[i]->Action = 120;
			else
				MainCharObj1[i]->Action = Action_Noclip;

			return;
		}
	}
}

void UnsetFreeMovements() {
	if (!isFreeMov)
		return;

	for (int i = 0; i < 2; i++) {

		if (Controllers[i].press & Buttons_A) {
			isFreeMov = false;
			DeathZoneDebug = 0;
			MainCharObj1[i]->Action = Action_Jump;
			return;
		}
	}
}

void CheckFreeMovements() {

	if (!isFreeMov)
		return;

	SetFreeMovements();
	UnsetFreeMovements();
	DrawFreeMovementText();
	Hunters_FreeMovements();
	return;
}

void Debugging_InputCheck() {

	if (GameState != GameStates_Ingame)
		return;

	CheckFreeMovements();
	CheckController_ColDebug();
	DebugTextUpdateCurrentPage();
}

void LoadDebuggingObjects() {
	LoadObjDrawBG();
	LoadObjSaveState();
}