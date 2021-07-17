#include "stdafx.h"
#include "save-state.h"

unsigned int currentSaveState = 0;
SaveStates* SaveStates::instance = 0;
SaveStates* obj1 = obj1->getInstance();
bool canDisplayMSG = false;

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
	return;
}

void SaveStates::getCameraInfo() {
	this->slots[currentSaveState].cam = CameraData;
	this->slots[currentSaveState].camposX = CameraData.Position.x;
	this->slots[currentSaveState].camposY = CameraData.Position.y;
	this->slots[currentSaveState].camposZ = CameraData.Position.z;

	return;
}

void SaveStates::getObjectsState() {

	for (int i = 0; i < ObjectLists_Length; ++i)
	{
		if (ObjectLists[i]) {
			this->slots[currentSaveState].objlist[i] = ObjectLists[i];
		}
	}


	return;
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
	return;
}

void SaveStates::restoreCameraInfo() {

	CameraData = this->slots[currentSaveState].cam;
	CameraData.Position.x = this->slots[currentSaveState].camposX;
	CameraData.Position.y = this->slots[currentSaveState].camposY;
	CameraData.Position.z = this->slots[currentSaveState].camposZ;

	return;
}


void SaveStates::restoreObjectState() {

	for (int i = 0; i < ObjectLists_Length; ++i)
	{
		if (ObjectLists[i]) {
			ObjectLists[i] = this->slots[currentSaveState].objlist[i];
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

	if (!MainCharObj1[0]) {

		this->timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO SAVE ON SLOT %d";
		return;
	}


	this->timerMessage = 60;
	this->getGameInfo();
	this->getPlayerInfo();
	this->getCameraInfo();
	this->getObjectsState();
	SetDebugFontColor(0xFF1dcf01);
	this->message = "Saved on slot: %d";

	return;
}

void SaveStates::loadSlot() {

	if (!MainCharObj1[0] || CurrentLevel != this->slots[currentSaveState].level) {
		timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO LOAD SAVE ON SLOT: %d";
		return;
	}


	this->timerMessage = 60;
	this->restoreGameInfo();
	this->restorePlayerInfo();
	this->restoreCameraInfo();
	this->restoreObjectState();
	SetDebugFontColor(0xFF29c8e1);
	this->message = "Loaded Save State on slot %d";

	return;
}

void SaveStates::changeSlot() {


	if (this->timerSlotDelay != 0)
	{
		timerSlotDelay--;
	}
	else {
		SetDebugFontColor(0xFFBFBFBF);
		timerMessage = 60;

		if (currentSaveState < slot_count) {
			currentSaveState++;
			timerSlotDelay = 5;
			this->message = "Current Slot %d";
			return;
		}
		else {
			currentSaveState = 0;
			timerSlotDelay = 5;
			this->message = "Current Slot %d";
			return;
		}
	}
}


void SavestatesCheckInput() {

	if (!obj1 || Controllers[0].on & Buttons_Y)
		return;

	if (Controllers[0].on & Buttons_Left) {
		obj1->saveOnSlot();
	}

	if (Controllers[0].on & Buttons_Right) {
		obj1->loadSlot();
	}

	if (Controllers[0].on & Buttons_Up) {
		obj1->changeSlot();
	}


	obj1->displaySaveText();
}
