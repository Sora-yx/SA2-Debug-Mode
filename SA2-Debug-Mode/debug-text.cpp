#include "stdafx.h"
#include <cstdarg>
#include <cstdio>

int currentPage = None;
int texPosY = 4;
std::string debugText = "";
int DebugMessageTimer = 0;

static void ScaleDebugFont()
{
	HelperFunctionsGlobal.SetDebugFontSize((float)GetDebugFontSize());

}

// Formats a line, records where it lands so DrawDebugRectangle can size the
// background around it, then queues it.
void PrintPanelText(int col, int row, const char* format, ...)
{
	char text[128];

	va_list args;
	va_start(args, format);
	int length = vsnprintf(text, sizeof(text), format, args);
	va_end(args);

	if (length < 0)
		return;

	if (length >= (int)sizeof(text))
		length = (int)sizeof(text) - 1;

	AddTextExtents(col, row, length);
	HelperFunctionsGlobal.DisplayDebugString(NJM_LOCATION(col, row), text);
}

void setTexPosY() {

	const int rowOffset = (CurrentLevel == LevelIDs_Route101280 || CurrentLevel == LevelIDs_KartRace) ? 6 : 4;

	if (rowOffset != texPosY)
	{
		texPosY = rowOffset;
		ResetTextExtents(); // the whole block moved, measure it again
	}

	return;
}

void DisplayPlayerInformation() {

	if (currentPage != pPlayerInfo)
		return;

	if (MainCharObj1[0] == nullptr || CurrentLevel == LevelIDs_KartRace)
	{
		SetDebugFontColor(0xFFFF0000);
		PrintPanelText(2, 7 + texPosY, "- PLAYER INFO UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(3, 7 + texPosY, "- PLAYER INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	PrintPanelText(3, 9 + texPosY, "POS X: %.2f", MainCharObj1[0]->Position.x);
	PrintPanelText(3, 10 + texPosY, "POS Y: %.2f", MainCharObj1[0]->Position.y);
	PrintPanelText(3, 11 + texPosY, "POS Z: %.2f", MainCharObj1[0]->Position.z);

	cartStruct* cartPointer = getCartPointer();

	if (!cartPointer)
	{
		PrintPanelText(3, 13 + texPosY, "ACTION: %d", MainCharObj1[0]->Action);
		PrintPanelText(3, 14 + texPosY, "NEXT ACTION: %d", MainCharObj1[0]->NextAction);
	}
	else {

		float spdX = fabs(cartPointer[1].SpeedX);
		PrintPanelText(3, 13 + texPosY, "ACTION: %d", cartPointer->KartAction);
		PrintPanelText(3, 14 + texPosY, "CART SPEED X: %.2f", spdX);
		PrintPanelText(3, 15 + texPosY, "CART SPEED Y: %.2f", cartPointer[1].SpeedY);
		return;
	}

	if (MainCharObj2[0] == nullptr)
		return;

	PrintPanelText(3, 16 + texPosY, "HOVER FRAMES: %d", MainCharObj2[0]->field_12);

	PrintPanelText(3, 17 + texPosY, "NEXT ANIM: %d", MainCharObj2[0]->AnimInfo.Next);
	PrintPanelText(3, 18 + texPosY, "CURRENT ANIM: %d", MainCharObj2[0]->AnimInfo.Current);

	PrintPanelText(3, 20 + texPosY, "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	PrintPanelText(3, 21 + texPosY, "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	PrintPanelText(3, 22 + texPosY, "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);

	return;
}

void DisplayGameInfo()
{
	if (currentPage != pGameInfo)
		return;

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(5, 7 + texPosY, "- GAME STATS -");
	SetDebugFontColor(0xFFBFBFBF);

	PrintPanelText(3, 9 + texPosY, "FRAME: %08d", FrameCount);
	PrintPanelText(3, 10 + texPosY, "FRAME LVL: %08d", FrameCountIngame);

	PrintPanelText(3, 12 + texPosY, "GAME MODE: %02d", GameMode);
	PrintPanelText(3, 13 + texPosY, "GAME STATE: %02d", GameState);

	PrintPanelText(3, 15 + texPosY, "CHARACTER: %01d", CurrentCharacter);
	PrintPanelText(3, 16 + texPosY, "LEVEL: %02d", CurrentLevel);
	PrintPanelText(3, 17 + texPosY, "CHAO AREA: %02d", CurrentChaoArea);
	return;
}

void DrawHintText(char* text, char pID, char count) {

	PrintPanelText(3, count + texPosY, "P%d: %.18s", pID +1, text);
}

void GetNextEmeraldPosition() {

	if (!EmeraldManagerObj2)
		return;

	PrintPanelText(3, 9 + texPosY, "Piece(s) Left: %d", EmeraldManagerObj2->byte5);

	if (!EmeraldManagerObj2->byte5)
		return;

	char* text;

	EmeManThing piece = EmeraldManagerObj2->byte2C[0];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {

		text = (char*)getHintText_r(NULL, 0);
		DrawHintText(text, 0, 15);
		PrintPanelText(3, 11 + texPosY, "P1 Distance: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[0].v, &MainCharObj1[0]->Position));
	}

	piece = EmeraldManagerObj2->byte2C[1];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {

		text = (char*)getHintText_r(NULL, 1);
		DrawHintText(text, 1, 16);
		PrintPanelText(3, 12 + texPosY, "P2 Distance: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[1].v, &MainCharObj1[0]->Position));
	}

	piece = EmeraldManagerObj2->byte2C[2];

	if (piece.byte1 > 0 || piece.byte0 >= 0) {
		text = (char*)getHintText_r(NULL, 2);

		DrawHintText(text, 2, 17);
		PrintPanelText(3, 13 + texPosY, "P3 Distance: %.2f", CheckDistance(&EmeraldManagerObj2->byte2C[2].v, &MainCharObj1[0]->Position));
	}
}

void DisplayTreasureHuntingInfo()
{
	if (GetCharacterLevel() != Characters_Knuckles && GetCharacterLevel() != Characters_Rouge || !EmeraldManagerObj2)
	{
		SetDebugFontColor(0xFFFF0000);
		PrintPanelText(2, 7 + texPosY, "- HUNTING UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(5, 7 + texPosY, "- HUNTING -");
	SetDebugFontColor(0xFFBFBFBF);

	GetNextEmeraldPosition();
	return;
}

void DisplaySpeedCharInfo() {

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(5, 7 + texPosY, "- SPEED CHAR INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	SonicCharObj2* sonicCO2 = (SonicCharObj2*)MainCharacter[0]->Data2.Undefined;

	PrintPanelText(3, 9 + texPosY, "SOMERSAULT TIMER: %d", sonicCO2->SomersaultTime);

	PrintPanelText(3, 10 + texPosY, "SPIN DASH COUNTER: %d", sonicCO2->SpindashCounter);
	PrintPanelText(3, 16 + texPosY, "HOVER FRAMES: %d", MainCharObj2[0]->field_12);	

	PrintPanelText(3, 12 + texPosY, "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	PrintPanelText(3, 13 + texPosY, "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	PrintPanelText(3, 14 + texPosY, "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
}

void DisplayMechCharInfo() {

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(5, 7 + texPosY, "- MECH CHAR INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	auto mechCO2 = (MechEggmanCharObj2*)MainCharacter[0]->Data2.Undefined;

	PrintPanelText(3, 9 + texPosY, "MECH HP: %.2f", mechCO2->base.MechHP);

	PrintPanelText(3, 12 + texPosY, "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	PrintPanelText(3, 13 + texPosY, "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	PrintPanelText(3, 14 + texPosY, "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
}

void DisplayCharacterInfo() {

	if (currentPage != pCharacterInfo)
		return;

	if (!MainCharObj1[0] || getCartPointer() || CurrentLevel == LevelIDs_KartRace) {
		currentPage++;
		return;
	}

	DisplayGoalRingPos(17, texPosY);

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

extern int FreeCamMode;
void DisplayCameraInfo()
{
	if (currentPage != pCameraInfo)
		return;

	if (!MainCharObj1[0])
	{
		SetDebugFontColor(0xFFFF0000);
		PrintPanelText(2, 7 + texPosY, "- CAM UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	PrintPanelText(3, 7 + texPosY, "- CAMERA INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	PrintPanelText(3, 9 + texPosY, "POS X: %.2f", CameraData[0].location.pos.x);
	PrintPanelText(3, 10 + texPosY, "POS Y: %.2f", CameraData[0].location.pos.y);
	PrintPanelText(3, 11 + texPosY, "POS Z: %.2f", CameraData[0].location.pos.z);
	
	PrintPanelText(3, 13 + texPosY, "ANG X: %d", (Uint16)CameraData[0].location.ang.x, (360.0f / 65535.0f) * (Uint16)CameraData[0].location.ang.x);
	PrintPanelText(3, 14 + texPosY, "ANG Y: %d", (Uint16)CameraData[0].location.ang.y, (360.0f / 65535.0f) * (Uint16)CameraData[0].location.ang.y);

	PrintPanelText(3, 16 + texPosY, "FREECAM MODE: %d", FreeCamMode);

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

void DisplayTimed_DebugMessage_OnFrames()
{
	if (DebugMessageTimer && debugText != "")
	{
		SetDebugFontColor(0xFFBFBFBF);
		HelperFunctionsGlobal.DisplayDebugString(NJM_LOCATION(25, 10), debugText.c_str());
		SetDebugFontColor(0xFFBFBFBF);
		DebugMessageTimer--;
	}
}

void SendTimedDebugMessage(std::string msg, int timer)
{
	DebugMessageTimer = timer;
	debugText = msg;
}

void DisplayDebugTextInfo() {

	ScaleDebugFont();
	setTexPosY();
	DisplayPlayerInformation();
	DisplayGameInfo();
	DisplayCharacterInfo();
	DisplayCameraInfo();
	DisplayTimed_DebugMessage_OnFrames();
}

void initializeDebugText() {

	SetDebugFontColor(0xFFBFBFBF);
	ScaleDebugFont();
}
