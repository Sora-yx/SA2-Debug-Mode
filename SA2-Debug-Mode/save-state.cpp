#include "stdafx.h"
#include "save-state.h"

int currentSaveState = 0;
SaveStates* SaveStates::instance = 0;
SaveStates* obj1 = obj1->getInstance();
bool canDisplayMSG = false;
Trampoline* MechEggman_chkDmg_t;
Trampoline* sub_461F10_t;
const char slot_count = 7;


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

	if (!co2 || !data)
		return;

	this->slots[currentSaveState].pos = data->Position;
	this->slots[currentSaveState].rot = data->Rotation;
	this->slots[currentSaveState].spd = co2->Speed;
	this->slots[currentSaveState].acc = co2->Acceleration;
	this->slots[currentSaveState].grv = Gravity;

	this->slots[currentSaveState].action = data->Action;
	this->slots[currentSaveState].anim = co2->AnimInfo.Current;
	this->slots[currentSaveState].Status = data->Status;
	this->slots[currentSaveState].hoverFrames = MainCharObj2[0]->field_12;

	memcpy(&this->slots[currentSaveState].physics, &co2->PhysData, sizeof(PhysicsData));
	this->slots[currentSaveState].Powerups = co2->Powerups;
	this->slots[currentSaveState].MechHP = co2->MechHP;
	return;
}

//Copy camera stuff credits: OnVar
void SaveStates::getCameraInfo() {
	memcpy(&this->slots[currentSaveState].CameraUnit.camera, (void*)0x1dcff00, 0x2518);
	memcpy(&this->slots[currentSaveState].CameraUnit.camPos, (void*)0x1a5a234, sizeof(byte*));
	memcpy(&this->slots[currentSaveState].CameraUnit.camRot, (void*)0x1a5a238, sizeof(byte*));
	memcpy(&this->slots[currentSaveState].CameraUnit.posRotBuffer, PosRotBufferIndex, 0x2);
	memcpy(&this->slots[currentSaveState].CameraUnit.pastpos, (void*)0x19f1740, 0xc00);
	memcpy(&this->slots[currentSaveState].CameraUnit.idk2, &camConstPastPosIDX, sizeof(byte*));
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

	if (!co2 || !data)
		return;

	data->Action = this->slots[currentSaveState].action;
	co2->AnimInfo.Next = this->slots[currentSaveState].anim;
	co2->field_12 = this->slots[currentSaveState].hoverFrames;

	data->Position = this->slots[currentSaveState].pos;
	data->Rotation = this->slots[currentSaveState].rot;

	data->Status = this->slots[currentSaveState].Status;
	co2->Speed = this->slots[currentSaveState].spd;
	co2->Acceleration = this->slots[currentSaveState].acc;

	Gravity = this->slots[currentSaveState].grv;
	memcpy(&co2->PhysData, &this->slots[currentSaveState].physics, sizeof(PhysicsData));
	co2->Powerups = this->slots[currentSaveState].Powerups;
	co2->MechHP = this->slots[currentSaveState].MechHP;

	return;
}

void SaveStates::restoreCameraInfo() {
	memcpy((void*)0x1dcff00, &this->slots[currentSaveState].CameraUnit.camera, 0x2518);
	memcpy((void*)0x1a5a234, &this->slots[currentSaveState].CameraUnit.camPos, sizeof(byte*));
	memcpy((void*)0x1a5a238, &this->slots[currentSaveState].CameraUnit.camRot, sizeof(byte*));
	memcpy(PosRotBufferIndex, &this->slots[currentSaveState].CameraUnit.posRotBuffer, 0x2);
	memcpy((void*)0x19f1740, &this->slots[currentSaveState].CameraUnit.pastpos, 0xc00);
	memcpy(&camConstPastPosIDX, &this->slots[currentSaveState].CameraUnit.idk2, sizeof(byte*));

	return;
}

int bannedLevel[8] = { LevelIDs_AquaticMine, LevelIDs_HiddenBase, LevelIDs_LostColony, LevelIDs_CosmicWall, LevelIDs_EggQuarters, LevelIDs_IronGate, LevelIDs_FinalChase, LevelIDs_FinalRush };
ObjectFuncPtr bannedObj[2] = { (ObjectFuncPtr)0x6A79E0,(ObjectFuncPtr)0x6F7AF0 };

bool bannedLvlException() {

	for (int i = 0; i < LengthOfArray(bannedLevel); i++)
	{
		if (CurrentLevel == bannedLevel[i])
			return true;
	}

	return false;
}

bool ObjException(ObjectMaster* obj) {

	if (!obj)
		return false;

	for (int i = 0; i < LengthOfArray(bannedObj); i++)
	{
		if (obj->MainSub == bannedObj[i])
			return true;

	}

	return false;
}

void SaveStates::restoreObjectState() {

	if (!objSave)
		return;

	ResetSetDataFlag();

	if (bannedLvlException())
		return;

	for (int i = 2; i < 6; i++)
	{
		ObjectMaster* obj = ObjectLists[i];
		ObjectMaster* obj_orig = obj;

		if (obj)
		{
			while (1)
			{
				ObjectMaster* previous = obj->PrevObject;

				if (ObjException(obj))
					break;

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
		DisplayDebugStringFormatted(NJM_LOCATION(10, 7), this->message, currentSaveState);
	}
	else {
		return;
	}
}

void SaveStates::saveOnSlot() {

	if (!MainCharObj1[0] || CurrentLevel >= LevelIDs_Route101280) {

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

	if (!MainCharObj1[0] || CurrentLevel != this->slots[currentSaveState].level || CurrentLevel >= LevelIDs_Route101280) {
		timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO LOAD SAVE ON SLOT: %d";
		if (obj)
			obj->Data1.Entity->NextAction = 0;

		return;
	}

	this->timerMessage = 60;
	obj1->restoreCameraInfo();
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

	if (input == Buttons_Up) {
		currentSaveState++;
	} 
	else if (input == Buttons_Down) {
		currentSaveState--;
	}

	if (currentSaveState > slot_count)
		currentSaveState = 0;

	if (currentSaveState < 0)
		currentSaveState = slot_count;

	timerMessage = 60;
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

//since object doesn't run when the pause menu is active, we manually allow the player to save when the game is paused.
void Save_Pause() {
	if (GameState != GameStates_Pause)
		return;

	if (Controllers[0].press & Buttons_Left) {
		obj1->saveOnSlot();
	}
}


void init_SaveState() {
	MechEggman_chkDmg_t = new Trampoline((int)0x742C10, (int)0x742C17, MechEggman_ChecksDamage_r);
}