#include "stdafx.h"
#include "save-state.h"

int currentSaveState = 0;
SaveStates* SaveStates::instance = 0;
SaveStates* obj1 = obj1->getInstance();
bool canDisplayMSG = false;
Trampoline* MechEggman_chkDmg_t = nullptr;
Trampoline* GamePlayerMissed_t = nullptr;
const char slot_count = 7;


void SaveStates::getGameInfo() {

	this->slots[currentSaveState].level = CurrentLevel;
	this->slots[currentSaveState].character = CurrentCharacter;
	this->slots[currentSaveState].lives = Life_Count[0];
	this->slots[currentSaveState].rings = RingCount[0];
	this->slots[currentSaveState].score = ScoreP1;

	this->slots[currentSaveState].timerCart = CartTimer;
	this->slots[currentSaveState].timeF = TimerFrames;
	this->slots[currentSaveState].timeS = TimerSeconds;
	this->slots[currentSaveState].timeM = TimerMinutes;
	this->slots[currentSaveState].timerStopped = TimerStopped;
	this->slots[currentSaveState].gameState = GameState;
	this->slots[currentSaveState].pauseValue[0] = dword_1A558BC;
	this->slots[currentSaveState].pauseValue[1] = dword_1A558B8;
	this->slots[currentSaveState].pauseValue[2] = dword_1AEE5AC;
	this->slots[currentSaveState].playerPaused = PlayerPaused;
	this->slots[currentSaveState].pauseDisabled = PauseDisabled;

	return;
}

bool isCartSaved = false;
bool isEmePtrSaved = false;

void SaveStates::getPlayerInfo() {

	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];

	if (!data)
		return;

	memcpy(&this->slots[currentSaveState].charData.player, MainCharacter[0], sizeof(ObjectMaster));
	memcpy(&this->slots[currentSaveState].charData.data, MainCharacter[0]->Data1.Entity, sizeof(EntityData1));
	memcpy(&this->slots[currentSaveState].charData.col, MainCharacter[0]->Data1.Entity->Collision, sizeof(CollisionInfo));

	SaveCartPointer();

	if (!co2 || getCartPointer())
		return;

	memcpy(&this->slots[currentSaveState].charData.data2, MainCharData2[0], sizeof(EntityData2));

	if (co2->CharID <= Characters_Shadow)
	{
		memcpy(&this->slots[currentSaveState].charData.sonicCO2, MainCharacter[0]->Data2.Character, sizeof(SonicCharObj2));
	}

	if (co2->CharID == Characters_Tails)
	{
		memcpy(&this->slots[currentSaveState].charData.tailsCO2, MainCharacter[0]->Data2.Character, sizeof(TailsCharObj2));
	}

	if (co2->CharID == Characters_Eggman)
	{
		memcpy(&this->slots[currentSaveState].charData.eggmanCO2, MainCharacter[0]->Data2.Character, sizeof(EggmanCharObj2));
	}

	if (co2->CharID == Characters_MechEggman || co2->CharID == Characters_MechTails)
	{
		memcpy(&this->slots[currentSaveState].charData.mechCO2, MainCharacter[0]->Data2.Character, sizeof(MechEggmanCharObj2));
	}

	if (co2->CharID == Characters_Knuckles || co2->CharID == Characters_Rouge)
	{
		memcpy(&this->slots[currentSaveState].charData.knuxCO2, MainCharacter[0]->Data2.Character, sizeof(KnucklesCharObj2));
	}

	this->slots[currentSaveState].grv = Gravity;
	return;
}

//credit onvar
INT_PTR camPast = 0x1A5A234;
INT_PTR camRot = 0x1a5a238;
INT_PTR camConstPastPos = 0x19f1740;
INT_PTR camPastPosIDX = 0x19f173c;

DataPointer(char, CampastPosIDX, 0x19f173c);

void SaveStates::getCameraInfo() {

	//general values (target, camera col, pos etc.)
	memcpy(&this->slots[currentSaveState].CameraUnit.cameraBackup, (void*)0x1dcff00, 0x2518);

	//cam struct and array (fix most of the random cam issues)
	memcpy(&this->slots[currentSaveState].CameraUnit.cameraPastPosBackup, (void*)camPast, 0xC000);
	memcpy(&this->slots[currentSaveState].CameraUnit.cameraPastRotBackup, (void*)camRot, 0xC000);
	this->slots[currentSaveState].CameraUnit.PosRotBufferIndex = PosRotBufferIndex[0];

	memcpy(&this->slots[currentSaveState].CameraUnit.camConstpastPos, (void*)camConstPastPos, 0xC000);
	this->slots[currentSaveState].CameraUnit.camConstPastPosIDX = CampastPosIDX;


	return;
}


static const void* const loc_43ADA9 = (void*)0x43ADA9;
void SaveStates::restoreGameInfo() {

	DeathZoneDebug = 1;

	if (GameState == GameStates_Pause && this->slots[currentSaveState].gameState == GameStates_Ingame)
	{
		Menu_Unknown_13(); //restore music and sound effect
		dword_17472BC = 1;
		dword_17483FC = 1;
	}
	else if (GameState == GameStates_Ingame && this->slots[currentSaveState].gameState == GameStates_Pause)
	{
		PauseSound(1);
	}

	dword_1A558BC = this->slots[currentSaveState].pauseValue[0];
	dword_1A558B8 = this->slots[currentSaveState].pauseValue[1];
	dword_1AEE5AC = this->slots[currentSaveState].pauseValue[2];
	PlayerPaused = this->slots[currentSaveState].playerPaused;
	GameState = this->slots[currentSaveState].gameState;

	if (GameState == GameStates_Ingame)
		PauseDisabled = 0;
	else
		PauseDisabled = this->slots[currentSaveState].pauseDisabled;

	Life_Count[0] = this->slots[currentSaveState].lives;
	RingCount[0] = this->slots[currentSaveState].rings;
	ScoreP1 = this->slots[currentSaveState].score;

	TimerFrames = this->slots[currentSaveState].timeF;
	TimerSeconds = this->slots[currentSaveState].timeS;
	TimerMinutes = this->slots[currentSaveState].timeM;
	CartTimer = this->slots[currentSaveState].timerCart;

	TimerStopped = this->slots[currentSaveState].timerStopped;

	return;
}


void SaveStates::restorePlayerInfo() {

	EntityData1* data = MainCharObj1[0];
	CharObj2Base* co2 = MainCharObj2[0];

	if (!data)
		return;

	if (getCartPointer())
	{
		isCartSaved = true;
		return;
	}

	co2->Powerups &= ~Powerups_Dead;
	Gravity = this->slots[currentSaveState].grv;

	memcpy(MainCharData2[0], &this->slots[currentSaveState].charData.data2, sizeof(EntityData2));

	if (co2->CharID <= Characters_Shadow)
	{
		memcpy(MainCharacter[0]->Data2.Character, &this->slots[currentSaveState].charData.sonicCO2, sizeof(SonicCharObj2));
	}

	if (co2->CharID == Characters_Tails)
	{
		memcpy(MainCharacter[0]->Data2.Character, &this->slots[currentSaveState].charData.tailsCO2, sizeof(TailsCharObj2));
	}

	if (co2->CharID == Characters_Eggman)
	{
		memcpy(MainCharacter[0]->Data2.Character, &this->slots[currentSaveState].charData.eggmanCO2, sizeof(EggmanCharObj2));
	}

	if (co2->CharID == Characters_MechEggman || co2->CharID == Characters_MechTails)
	{
		memcpy(MainCharacter[0]->Data2.Character, &this->slots[currentSaveState].charData.mechCO2, sizeof(MechEggmanCharObj2));
	}

	if (co2->CharID == Characters_Knuckles || co2->CharID == Characters_Rouge)
	{
		memcpy(MainCharacter[0]->Data2.Character, &this->slots[currentSaveState].charData.knuxCO2, sizeof(KnucklesCharObj2));
	}

	if (TimeTotal % 1 == 0) //wait 1 frame to re load collision
	{
		memcpy(MainCharacter[0]->Data1.Entity, &this->slots[currentSaveState].charData.data, sizeof(EntityData1));
		memcpy(MainCharacter[0]->Data1.Entity->Collision, &this->slots[currentSaveState].charData.col, sizeof(CollisionInfo));
	}

	return;
}

void SaveStates::restoreCameraInfo() {

	memcpy((void*)0x1dcff00, &this->slots[currentSaveState].CameraUnit.cameraBackup, 0x2518);
	//Crash the game for some reason...
	//memcpy((void*)camPast, &this->slots[currentSaveState].CameraUnit.cameraPastPosBackup, 0xC000);
	//memcpy((void*)camRot, &this->slots[currentSaveState].CameraUnit.cameraPastRotBackup, 0xC0000);
	PosRotBufferIndex[0] = this->slots[currentSaveState].CameraUnit.PosRotBufferIndex;
	memcpy((void*)camConstPastPos, &this->slots[currentSaveState].CameraUnit.camConstpastPos, 0xC000);
	CampastPosIDX = this->slots[currentSaveState].CameraUnit.camConstPastPosIDX;

	return;
}

int bannedLevel[9] = { LevelIDs_PyramidCave, LevelIDs_AquaticMine, LevelIDs_HiddenBase, LevelIDs_LostColony, LevelIDs_CosmicWall,
LevelIDs_EggQuarters, LevelIDs_IronGate, LevelIDs_FinalChase, LevelIDs_FinalRush };
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

	if (!objSave || CurrentLevel == LevelIDs_Route101280)
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

	if (CurrentLevel >= LevelIDs_BigFoot && !getCartPointer())
	{
		return;
	}

	if (!MainCharObj1[0]) {

		this->timerMessage = 60;
		SetDebugFontColor(0xFFFF0000);
		this->message = "ERROR, FAILED TO SAVE ON SLOT %d";
		return;
	}

	this->timerMessage = 60;
	this->getCameraInfo();
	this->getGameInfo();
	this->getPlayerInfo();
	SetDebugFontColor(0xFF1dcf01);
	this->message = "Saved on slot: %d";
	return;
}

void SaveStates::loadSlot(ObjectMaster* obj) {


	if (CurrentLevel >= LevelIDs_BigFoot && !getCartPointer())
	{
		return;
	}

	if (!MainCharObj1[0] || CurrentLevel != this->slots[currentSaveState].level) {
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
	obj1->restoreCameraInfo();
	//this->restoreObjectState();
	SetDebugFontColor(0xFF29c8e1);
	this->message = "Loaded Save State on slot %d";

	if (obj)
		obj->Data1.Entity->NextAction = 1;

	return;
}

void SaveStates::changeSlot(Buttons input) {

	if (CurrentLevel >= LevelIDs_BigFoot && !getCartPointer())
	{
		return;
	}

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

extern bool isFreeMov;
void SaveStateManager(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	switch (data->Action) {
	case InitSaveManager:
		obj->DisplaySub = SaveStateDisplay;
		obj->DeleteSub = DeleteSaveManager;
		data->Action = CheckInputs;
		break;
	case CheckInputs:
		data->Timer = 0;
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

		if (++data->Timer == 8) {
			data->Action = CheckInputs;

			if (!isFreeMov) {
				DeathZoneDebug = 0;
			}
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
	//don't save if the player is restarting / quiting
	if (GameState != GameStates_Pause || PauseSelection == 1 || PauseSelection == 5 || !savestateObj)
		return;

	savestateObj->MainSub(savestateObj);
}

void __cdecl GamePlayerMissed_r(ObjectMaster* obj)
{
	if (savestateObj) {
		if (savestateObj->Data1.Entity->Action >= LoadSaveMode) {

			DeleteObject_(obj);
			return;
		}
	}

	ObjectFunc(origin, GamePlayerMissed_t->Target());
	origin(obj);
}

void init_SaveState() {
	MechEggman_chkDmg_t = new Trampoline((int)0x742C10, (int)0x742C17, MechEggman_ChecksDamage_r);
	GamePlayerMissed_t = new Trampoline((int)GamePlayerMissed, (int)GamePlayerMissed + 0x5, GamePlayerMissed_r);
}