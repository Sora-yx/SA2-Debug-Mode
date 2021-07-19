#include "stdafx.h"
#include "save-state.h"

int currentSaveState = 0;
SaveStates* SaveStates::instance = 0;
SaveStates* obj1 = obj1->getInstance();
bool canDisplayMSG = false;
Trampoline* MechEggman_chkDmg_t;


void SaveStates::getGameInfo() {
	this->slots[currentSaveState].level = CurrentLevel;
	this->slots[currentSaveState].character = CurrentCharacter;
	this->slots[currentSaveState].lives = Life_Count[0];
	this->slots[currentSaveState].rings = RingCount[0];
	this->slots[currentSaveState].score = ScoreP1;
	this->slots[currentSaveState].timeF = TimerFrames;
	this->slots[currentSaveState].timeS = TimerSeconds;
	this->slots[currentSaveState].timeM = TimerMinutes;
	return;
}

void SaveStates::getPlayerInfo() {
	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];

	this->slots[currentSaveState].pos = data->Position;
	this->slots[currentSaveState].rot = data->Rotation;
	this->slots[currentSaveState].spd = co2->Speed;
	this->slots[currentSaveState].acc = co2->Acceleration;
	this->slots[currentSaveState].grv = Gravity;

	this->slots[currentSaveState].action = data->Action;
	this->slots[currentSaveState].anim = co2->AnimInfo.Current;
	this->slots[currentSaveState].Status = data->Status;
	this->slots[currentSaveState].hoverFrames = MainCharObj2[0]->field_12;

	this->slots[currentSaveState].HeldObject = co2->HeldObject;
	this->slots[currentSaveState].HoldTarget = co2->HoldTarget;
	this->slots[currentSaveState].Powerups = co2->Powerups;
	this->slots[currentSaveState].MechHP = co2->MechHP;
	return;
}

void SaveStates::getCameraInfo() {
	this->slots[currentSaveState].camInfo.Position = CameraData.Position;
	this->slots[currentSaveState].camInfo.Rotation = CameraData.Rotation;
	this->slots[currentSaveState].camInfo.field_0 = CameraData.field_0;
	this->slots[currentSaveState].camInfo.field_4 = CameraData.field_4;
	this->slots[currentSaveState].camInfo.field_8 = CameraData.field_8;

	return;
}

void SaveStates::getObjectsState() {
	for (int i = 2; i < 6; i++) {
		if (ObjectLists[i]) {
			this->slots[currentSaveState].ObjectList[i] = ObjectLists[i];
		}
	}
}

void SaveStates::restoreGameInfo() {
	Life_Count[0] = this->slots[currentSaveState].lives;
	RingCount[0] = this->slots[currentSaveState].rings;
	ScoreP1 = this->slots[currentSaveState].score;
	TimerFrames = this->slots[currentSaveState].timeF;
	TimerSeconds = this->slots[currentSaveState].timeS;
	TimerMinutes = this->slots[currentSaveState].timeM;
	return;
}

void SaveStates::restorePlayerInfo() {
	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];

	data->Action = this->slots[currentSaveState].action;
	co2->AnimInfo.Next = this->slots[currentSaveState].anim;
	co2->field_12 = this->slots[currentSaveState].hoverFrames;

	data->Position = this->slots[currentSaveState].pos;
	data->Rotation = this->slots[currentSaveState].rot;
	data->Status = this->slots[currentSaveState].Status;
	co2->Speed = this->slots[currentSaveState].spd;
	co2->Acceleration = this->slots[currentSaveState].acc;

	Gravity = this->slots[currentSaveState].grv;

	co2->HeldObject = this->slots[currentSaveState].HeldObject;
	co2->HoldTarget = this->slots[currentSaveState].HoldTarget;
	co2->Powerups = this->slots[currentSaveState].Powerups;
	co2->MechHP = this->slots[currentSaveState].MechHP;
	return;
}

void SaveStates::restoreCameraInfo() {
	CameraData.Position = this->slots[currentSaveState].camInfo.Position;
	CameraData.Rotation = this->slots[currentSaveState].camInfo.Rotation;
	CameraData.field_0 = this->slots[currentSaveState].camInfo.field_0;
	CameraData.field_4 = this->slots[currentSaveState].camInfo.field_4;
	CameraData.field_8 = this->slots[currentSaveState].camInfo.field_8;

	return;
}


void SaveStates::restoreObjectState() {
	ResetSetDataFlag();

	for (int i = 2; i < 6; i++)
	{
		ObjectMaster* obj = ObjectLists[i];
		ObjectMaster* obj_orig = obj;

		if (obj)
		{
			while (1)
			{
				ObjectMaster* previous = obj->PrevObject;

				if (obj->SETData)
				{
					obj->MainSub = DeleteObject_;
				}

				if (obj->Child)
				{
					DeleteChildObjects(obj);
				}

				if (previous == obj_orig)
				{
					break;
				}

				obj_orig = ObjectLists[i];

				if (!obj_orig)
				{
					break;
				}

				obj = previous;
			}
		}
	}
}

void SaveStates::displaySaveText() {


	if (this->timerMessage != 0 && this->message != nullptr)
	{
		timerMessage--;
		if (currentSaveState < slot_count)
			DisplayDebugStringFormatted(NJM_LOCATION(10, 7), this->message, currentSaveState + 1);
		else
			DisplayDebugStringFormatted(NJM_LOCATION(10, 7), this->message, currentSaveState);
	}
	else {
		return;
	}
}


void SaveStates::saveOnSlot() {

	if (!MainCharObj1[0] || CurrentLevel == LevelIDs_ChaoWorld) {

		this->timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO SAVE ON SLOT %d";
		return;
	}

	this->timerMessage = 60;
	this->getCameraInfo();
	this->getGameInfo();
	this->getPlayerInfo();
	this->getObjectsState();
	SetDebugFontColor(0xFF1dcf01);
	this->message = "Saved on slot: %d";

	return;
}

void SaveStates::loadSlot(ObjectMaster* obj) {

	if (!MainCharObj1[0] || CurrentLevel != this->slots[currentSaveState].level || CurrentLevel == LevelIDs_ChaoWorld) {
		timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO LOAD SAVE ON SLOT: %d";
		if (obj)
			obj->Data1.Entity->NextAction = 0;

		return;
	}

	this->timerMessage = 60;
	this->restoreGameInfo();
	this->restorePlayerInfo();
	this->restoreObjectState();
	SetDebugFontColor(0xFF29c8e1);
	this->message = "Loaded Save State on slot %d";

	if (obj)
		obj->Data1.Entity->NextAction = 1;

	return;
}

void SaveStates::changeSlot(Buttons input) {

	SetDebugFontColor(0xFFBFBFBF);
	timerMessage = 60;


	if (input == Buttons_Up)
		currentSaveState++;

	if (input == Buttons_Down)
		currentSaveState--;


	if (currentSaveState > slot_count)
		currentSaveState = 0;
	else if (currentSaveState < 0)
		currentSaveState = slot_count;

	this->message = "Current Slot %d";
	return;
}


void SavestatesCheckInput(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (!obj1 || Controllers[0].on & Buttons_Y)
		return;

	if (Controllers[0].press & Buttons_Left) {
		data->Action = SaveStateMode;
		return;
	}

	if (Controllers[0].press & Buttons_Right) {
		data->Action = LoadSaveMode;
		return;
	}

	if (Controllers[0].press & Buttons_Up) {
		obj1->changeSlot(Buttons_Up);
		return;
	}

	if (Controllers[0].press & Buttons_Down) {
		obj1->changeSlot(Buttons_Down);
		return;
	}
}

ObjectMaster* savestateObj;

void DeleteSaveManager(ObjectMaster* obj) {
	if (savestateObj) {
		savestateObj = nullptr;
	}
}

void SaveStateDisplay(ObjectMaster* obj) {
	if (!obj1)
		return;

	obj1->displaySaveText();
}

void SaveStateManager(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	switch (data->Action) {
	case InitSaveManager:
		obj->DisplaySub = SaveStateDisplay;
		obj->DeleteSub = DeleteSaveManager;
		data->Action = CheckInputs;
		break;
	case CheckInputs:
		data->field_6 = 0;
		data->Index = 0;
		data->NextAction = 0;
		SavestatesCheckInput(obj);
		break;
	case SaveStateMode:
		obj1->saveOnSlot();
		data->Action = SaveDelay;
		break;
	case LoadSaveMode:
		obj1->loadSlot(obj);
		data->Action = SaveDelay;
		break;
	case SaveDelay:
		if (data->NextAction == 1)
			obj1->restoreCameraInfo(); //for some reason, this needs to be called multiple times to restore the camera properly, funny game.

		if (++data->field_6 == 8) {
			data->Action = CheckInputs;
		}
		break;
	}
}


void LoadObjSaveState() {
	if (!savestateObj && isSave) {
		savestateObj = LoadObject(0, "saveState", SaveStateManager, LoadObj_Data1);
	}
}

//fix non sense crash with Mech Character.
void __cdecl MechEggman_ChecksDamage_r(EntityData1* a1, EntityData2* a3, CharObj2Base* a4, CharObj2Base* a2) {

	if (savestateObj) {
		if (savestateObj->Data1.Entity->Action >= LoadSaveMode) {
			return;
		}
	}

	FunctionPointer(void, original, (EntityData1 * a1, EntityData2 * a3, CharObj2Base * a4, CharObj2Base * a2), MechEggman_chkDmg_t->Target());
	original(a1, a3, a4, a2);
}

void init_SaveState() {
	MechEggman_chkDmg_t = new Trampoline((int)0x742C10, (int)0x742C17, MechEggman_ChecksDamage_r);
}