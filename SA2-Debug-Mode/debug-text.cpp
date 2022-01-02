#include "stdafx.h"

int currentPage = None;

void ScaleDebugFont(int scale)
{
	float FontScale;

	if ((float)HorizontalResolution / (float)VerticalResolution > 1.33f)
		FontScale = floor((float)VerticalResolution / 480.0f);
	else
		FontScale = floor((float)HorizontalResolution / 640.0f);

	HelperFunctionsGlobal.SetDebugFontSize(FontScale * scale);
	return;
}

void DisplayPlayerInformation() {

	if (currentPage != pPlayerInfo)
		return;

	char texPosY = 2.0f;

	if (CurrentLevel == LevelIDs_Route101280 || CurrentLevel == LevelIDs_KartRace)
		texPosY = 6.0f;

	if (MainCharObj1[0] == nullptr || CurrentLevel == LevelIDs_KartRace)
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7 + texPosY), "- PLAYER INFO UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 7 + texPosY), "- PLAYER INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "POS X: %.2f", MainCharObj1[0]->Position.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10 + texPosY), "POS Y: %.2f", MainCharObj1[0]->Position.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 11 + texPosY), "POS Z: %.2f", MainCharObj1[0]->Position.z);

	cartStruct* cartPointer = getCartPointer();

	if (!cartPointer)
	{
		DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "ACTION: %d", MainCharObj1[0]->Action);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 14 + texPosY), "NEXT ACTION: %d", MainCharObj1[0]->NextAction);
	}
	else {

		float spdX = fabs(cartPointer[1].SpeedX);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "ACTION: %d", cartPointer->KartAction);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 14 + texPosY), "CART SPEED X: %.2f", spdX);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 15 + texPosY), "CART SPEED Y: %.2f", cartPointer[1].SpeedY);
		return;
	}

	if (MainCharObj2[0] == nullptr)
		return;

	DisplayDebugStringFormatted(NJM_LOCATION(3, 16 + texPosY), "HOVER FRAMES: %d", MainCharObj2[0]->field_12);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 17 + texPosY), "NEXT ANIM: %d", MainCharObj2[0]->AnimInfo.Next);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 18 + texPosY), "CURRENT ANIM: %d", MainCharObj2[0]->AnimInfo.Current);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 20 + texPosY), "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 21 + texPosY), "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 22 + texPosY), "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
	return;
}

void DisplayGameInfo()
{
	if (currentPage != pGameInfo)
		return;

	char texPosY = 2.0f;

	if (CurrentLevel == LevelIDs_Route101280 || CurrentLevel == LevelIDs_KartRace)
		texPosY = 5.0f;

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7 + texPosY), "- GAME STATS -");
	SetDebugFontColor(0xFFBFBFBF);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "FRAME: %08d", FrameCount);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10 + texPosY), "FRAME LVL: %08d", FrameCountIngame);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 12 + texPosY), "GAME MODE: %02d", GameMode);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "GAME STATE: %02d", GameState);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 15 + texPosY), "CHARACTER: %01d", CurrentCharacter);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 16 + texPosY), "LEVEL: %02d", CurrentLevel);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 17 + texPosY), "CHAO AREA: %02d", CurrentChaoArea);
	return;
}

DataArray(const char*, HintsArray, 0x1AEFF54, 3);

ThiscallFunctionPointer(const char*, getHintText, (), 0x73B880);

DataPointer(char*, somethingAboutHint, 0x1AF016C);
DataPointer(int, curHintFileIdk, 0x01AF0170);



void GetNextEmeraldPosition() {

	if (!EmeraldManagerObj2)
		return;

	char texPosY = 2.0f;

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "PIECE(S) LEFT: %d", EmeraldManagerObj2->byte5);

	if (!EmeraldManagerObj2->byte5)
		return;

	char* text;

	EmeManThing piece = EmeraldManagerObj2->byte2C[0];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {

		text = (char*)getHintText_r(NULL, 0);

		DisplayDebugStringFormatted(NJM_LOCATION(3, 15 + texPosY), "P1: %.15s.", text);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 11 + texPosY), "P1 DISTANCE: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[0].v, &MainCharObj1[0]->Position));
	}

	piece = EmeraldManagerObj2->byte2C[1];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {

		text = (char*)getHintText_r(NULL, 1);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 16 + texPosY), "P2: %.15s.", text);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 12 + texPosY), "P2 DISTANCE: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[1].v, &MainCharObj1[0]->Position));
	}

	piece = EmeraldManagerObj2->byte2C[2];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {
		text = (char*)getHintText_r(NULL, 2);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 17 + texPosY), "P3: %.15s.", text);
		DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "P3 DISTANCE: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[2].v, &MainCharObj1[0]->Position));
	}
}

void DisplayTreasureHuntingInfo()
{

	char texPosY = 2.0f;

	if (GetCharacterLevel() != Characters_Knuckles && GetCharacterLevel() != Characters_Rouge || !EmeraldManagerObj2)
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7 + texPosY), "- HUNTING UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7 + texPosY), "- HUNTING -");
	SetDebugFontColor(0xFFBFBFBF);

	GetNextEmeraldPosition();

	return;
}

void DisplaySpeedCharInfo() {

	char texPosY = 2.0f;

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7 + texPosY), "- SPEED CHAR INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	SonicCharObj2* sonicCO2 = (SonicCharObj2*)MainCharacter[0]->Data2.Undefined;

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "SOMERSAULT TIMER: %d", sonicCO2->SomersaultTime);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 10 + texPosY), "SPIN DASH COUNTER: %d", sonicCO2->SpindashCounter);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 16 + texPosY), "HOVER FRAMES: %d", MainCharObj2[0]->field_12);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 12 + texPosY), "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 14 + texPosY), "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
}

void DisplayMechCharInfo() {

	char texPosY = 2.0f;

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7 + texPosY), "- MECH CHAR INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	auto mechCO2 = (MechEggmanCharObj2*)MainCharacter[0]->Data2.Undefined;

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "MECH HP: %.2f", mechCO2->base.MechHP);	


	DisplayDebugStringFormatted(NJM_LOCATION(3, 12 + texPosY), "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13 + texPosY), "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 14 + texPosY), "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
}

void DisplayCharacterInfo() {

	if (currentPage != pCharacterInfo)
		return;

	char texPosY = 2.0f;

	if (!MainCharObj1[0] || getCartPointer() || CurrentLevel == LevelIDs_KartRace) {
		currentPage++;
		return;
	}

	if (MainCharObj2[0]->CharID <= Characters_Shadow && !EmeraldManagerObj2)
	{
		DisplaySpeedCharInfo();
		return;
	}

	if (MainCharObj2[0]->CharID == Characters_MechTails || MainCharObj2[0]->CharID == Characters_MechEggman)
	{
		DisplayMechCharInfo();
		return;
	}

	if (MainCharObj2[0]->CharID == Characters_Knuckles || MainCharObj2[0]->CharID == Characters_Rouge || EmeraldManagerObj2)
	{
		DisplayTreasureHuntingInfo();
		return;
	}

	currentPage++;
	return;
}

void DisplayCameraInfo()
{
	if (currentPage != pCameraInfo)
		return;


	char texPosY = 2.0f;

	if (CurrentLevel == LevelIDs_Route101280 || CurrentLevel == LevelIDs_KartRace)
		texPosY = 5.0f;

	if (!MainCharObj1[0])
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7 + texPosY), "- CAM UNAVAILABLE -");
		return;
	}

	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);

	SetDebugFontColor(0xFF88FFAA);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 7 + texPosY), "- CAMERA INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9 + texPosY), "POS X: %.2f", CameraData.Position.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10 + texPosY), "POS Y: %.2f", CameraData.Position.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 11 + texPosY), "POS Z: %.2f", CameraData.Position.z);

	return;
}

void DebugTextUpdateCurrentPage() {

	if (!isDebugTxt)
		return;

	for (int i = 0; i < 2; i++) {

		if ((ControllerPointers[i]->on & Buttons_Y && ControllerPointers[i]->press & Buttons_Down)) {
			if (currentPage < MaxPage) {
				currentPage++;
				return;
			}
			else {
				currentPage = 0;
				return;
			}
		}
	}
}

void DisplayDebugTextInfo() {

	DisplayPlayerInformation();
	DisplayGameInfo();
	DisplayCharacterInfo();
	DisplayCameraInfo();
	return;
}

//just so I don't have to type "HelperFunctions.X"
void DisplayDebugStringFormatted(Int loc, const char* Format, ...) {
	return HelperFunctionsGlobal.DisplayDebugStringFormatted(loc, Format);
}

void SetDebugFontColor(int color) {
	return HelperFunctionsGlobal.SetDebugFontColor(color);
}

void initializeDebugText() {

	SetDebugFontColor(0xFFBFBFBF);
	ScaleDebugFont(14);

	return;
}
