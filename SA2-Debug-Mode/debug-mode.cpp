#include "stdafx.h"

bool isFreeMov = false;

void SetFreeMovements() {

	if (isFreeMov)
		return;

	for (int i = 0; i < 2; i++) {

		if ( (Controllers[i].press & Buttons_A && Controllers[i].press & Buttons_R) || (Controllers[i].on & Buttons_A && Controllers[i].on & Buttons_R)) {

			if (MainCharObj2[i]->CharID == Characters_Knuckles)
				return;

			isFreeMov = true;
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
			MainCharObj1[i]->Action = Action_Jump;
			return;
		}
	}
}

void CheckFreeMovements() {
	SetFreeMovements();
	UnsetFreeMovements();
	return;
}

void Debugging_InputCheck() {

	if (GameState != GameStates_Ingame)
		return;

	CheckFreeMovements();
	CheckController_ColDebug();
}
