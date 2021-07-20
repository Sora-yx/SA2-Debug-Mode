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

	if (MainCharObj1[0] == nullptr || MainCharObj2[0] == nullptr)
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 1), "- PLAYER INFO UNAVAILABLE -");
		return;
	}


	SetDebugFontColor(0xFF88FFAA);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 7), "- PLAYER INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "POS X: %.2f", MainCharObj1[0]->Position.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "POS Y: %.2f", MainCharObj1[0]->Position.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 11), "POS Z: %.2f", MainCharObj1[0]->Position.z);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 13), "ACTION: %d", MainCharObj1[0]->Action);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 14), "NEXT ACTION: %d", MainCharObj1[0]->NextAction);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 15), "HOVER FRAMES: %d", MainCharObj2[0]->field_12);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 17), "NEXT ANIM: %d", MainCharObj2[0]->AnimInfo.Next);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 18), "CURRENT ANIM: %d", MainCharObj2[0]->AnimInfo.Current);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 20), "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 21), "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 22), "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
	return;
}

void DisplayGameInfo()
{
	if (currentPage != pGameInfo)
		return;

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7), "- GAME STATS -");
	SetDebugFontColor(0xFFBFBFBF);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "FRAME: %08d", FrameCount);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "FRAME LVL: %08d", FrameCountIngame);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 12), "GAME MODE: %02d", GameMode);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13), "GAME STATE: %02d", GameState);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 15), "CHARACTER: %01d", CurrentCharacter);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 16), "LEVEL: %02d", CurrentLevel);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 17), "CHAO AREA: %02d", CurrentChaoArea);
	return;
}

DataArray(const char*, HintsArray, 0x1AEFF54, 3);

void DisplayTreasureHuntingInfo()
{
	if (currentPage != pHuntingInfo)
		return;

	if (GetCharacterLevel() != Characters_Knuckles && GetCharacterLevel() != Characters_Rouge || !EmeraldManagerObj2)
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 7), "- HUNTING UNAVAILABLE -");
		return;
	}

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7), "- HUNTING -");
	SetDebugFontColor(0xFFBFBFBF);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "Piece Collected: %d", EmeraldManagerObj2->Status);	

	char* hint = (char*)HintsArray[0];
	if (hint) {
		hint = hint + 3;
		DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "Piece Hint 1:  %.12s .", hint);
	}
	hint = (char*)HintsArray[1];
	if (hint) {
		hint = hint + 3;
		DisplayDebugStringFormatted(NJM_LOCATION(3, 11), "Piece Hint 2:  %.12s .", hint);
	}

	hint = (char*)HintsArray[2];
	if (hint) {
		hint = hint + 3;
		DisplayDebugStringFormatted(NJM_LOCATION(3, 12), "Piece Hint 3:  %.12s .", hint);
	}
	//DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "FRAME LVL: %08d", FrameCountIngame);
	return;
}

void DisplayCameraInfo()
{
	if (currentPage != pCameraInfo)
		return;

	if (!MainCharObj1[0])
	{
		SetDebugFontColor(0xFFFF0000);
		DisplayDebugStringFormatted(NJM_LOCATION(2, 1), "- CAM UNAVAILABLE -");
		return;
	}

	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);

	SetDebugFontColor(0xFF88FFAA);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 7), "- CAMERA INFO -");
	SetDebugFontColor(0xFFBFBFBF);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "POS X: %.2f", CameraData.Position.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "POS Y: %.2f", CameraData.Position.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 11), "POS Z: %.2f", CameraData.Position.z);

	return;
}



void DebugTextUpdateCurrentPage() {

	if (!isDebugTxt)
		return;

	for (int i = 0; i < 2; i++) {

		if ((Controllers[i].on & Buttons_Y && Controllers[i].press & Buttons_Down)) {
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
	DisplayTreasureHuntingInfo();
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
	ScaleDebugFont(16);
	return;
}
