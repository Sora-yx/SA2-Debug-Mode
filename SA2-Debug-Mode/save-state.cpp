#include "stdafx.h"
#include "save-state.h"

const unsigned int slot_count = 8;
unsigned int currentSaveState = 0;
SaveStates* SaveStates::instance = 0;
SaveStates* obj1 = obj1->getInstance();

void SaveStates::saveOnSlot() {

	if (!MainCharObj1[0]) {
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7), "- ERROR, FAILED TO SAVE. -");
		return;
	}

	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];


	this->slots[currentSaveState].lives = Life_Count[0];
	this->slots[currentSaveState].rings = RingCount[0];
	this->slots[currentSaveState].score = ScoreP1;
	this->slots[currentSaveState].timeF = TimerFrames;
	this->slots[currentSaveState].timeS = TimerSeconds;
	this->slots[currentSaveState].timeM = TimerMinutes;

	this->slots[currentSaveState].posX = data->Position.x;
	this->slots[currentSaveState].posY = data->Position.y;
	this->slots[currentSaveState].posZ = data->Position.z;
	this->slots[currentSaveState].rotY = data->Rotation.y;
	this->slots[currentSaveState].spdX = co2->Speed.x;
	this->slots[currentSaveState].spdY = co2->Speed.y;
	this->slots[currentSaveState].spdZ = co2->Speed.z;
	this->slots[currentSaveState].grvX = Gravity.x;
	this->slots[currentSaveState].grvY = Gravity.y;
	this->slots[currentSaveState].grvZ = Gravity.z;

	this->slots[currentSaveState].camPosX = CameraData.Position.x;
	this->slots[currentSaveState].camPosY = CameraData.Position.y;
	this->slots[currentSaveState].camPosZ = CameraData.Position.z;

	this->slots[currentSaveState].action = data->Action;
	this->slots[currentSaveState].anim = co2->AnimInfo.Current;
	DisplayDebugStringFormatted(NJM_LOCATION(10, 9), "Save State on slot: %d", currentSaveState);
};

void SaveStates::loadSlot() {

	if (!MainCharObj1[0]) {
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7), "- ERROR, FAILED TO LOAD. -");
		return;
	}

	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];

	Life_Count[0] = this->slots[currentSaveState].lives;
	RingCount[0] = this->slots[currentSaveState].rings;
	ScoreP1 = this->slots[currentSaveState].score;
	TimerFrames = this->slots[currentSaveState].timeF;
	TimerSeconds = this->slots[currentSaveState].timeS;
	TimerMinutes = this->slots[currentSaveState].timeM;

	data->Action = this->slots[currentSaveState].action;
	co2->AnimInfo.Next = this->slots[currentSaveState].anim;

	data->Position.x = this->slots[currentSaveState].posX;
	data->Position.y = this->slots[currentSaveState].posY;
	data->Position.z = this->slots[currentSaveState].posZ;
	data->Rotation.y = this->slots[currentSaveState].rotY;
	co2->Speed.x = this->slots[currentSaveState].spdX;
	co2->Speed.y = this->slots[currentSaveState].spdY;
	co2->Speed.z = this->slots[currentSaveState].spdZ;
	Gravity.x = this->slots[currentSaveState].grvX;
	Gravity.y = this->slots[currentSaveState].grvY;
	Gravity.z = this->slots[currentSaveState].grvZ;

	CameraData.Position.x = this->slots[currentSaveState].camPosX;
	CameraData.Position.y = this->slots[currentSaveState].camPosY;
	CameraData.Position.z = this->slots[currentSaveState].camPosZ;
	DisplayDebugStringFormatted(NJM_LOCATION(10, 9), "Loaded Save State on slot: %d", currentSaveState);
};

void SaveStates::clearSlot() {

};

void SavestatesCheckInput() {

	if (!obj1)
		return;

	if (Controllers[0].on & Buttons_Left) {
		obj1->saveOnSlot();
	}

	if (Controllers[0].on & Buttons_Right) {
		obj1->loadSlot();
	}

}
