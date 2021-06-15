#include "stdafx.h"

bool isFreeMov = false;
PolygonPoint coolSquare[4] = { 0 };
extern int currentPage;

void DrawFreeMovementText() {

	if (!isFreeMov)
		return;

	DisplayDebugStringFormatted(NJM_LOCATION(10, 10), "FREE MOVEMENTS ENABLED");
	return;
}

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

		if ( (Controllers[i].on & Buttons_A && Controllers[i].press & Buttons_R)) {

			isFreeMov = true;
			DeathZoneDebug = 1; //death zone can no longer kill player

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
