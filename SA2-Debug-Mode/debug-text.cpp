#include "stdafx.h"

int currentPage = 0;

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

	if (currentPage != 1)
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

	DisplayDebugStringFormatted(NJM_LOCATION(3, 16), "NEXT ANIM: %d", MainCharObj2[0]->AnimInfo.Next);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 17), "CURRENT ANIM: %d", MainCharObj2[0]->AnimInfo.Current);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 19), "SPEED X: %.2f", MainCharObj2[0]->Speed.x);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 20), "SPEED Y: %.2f", MainCharObj2[0]->Speed.y);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 21), "SPEED Z: %.2f", MainCharObj2[0]->Speed.z);
	return;
}

void DisplayGameInfo()
{
	if (currentPage != 2)
		return;

	SetDebugFontColor(0xFF88FFAA);
	//DrawDebugRectangle(1.75f, 0.75f, 22, 21.5f);
	DisplayDebugStringFormatted(NJM_LOCATION(5, 7), "- GAME STATS -");
	SetDebugFontColor(0xFFBFBFBF);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 9), "FRAME: %08d", FrameCount);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 10), "FRAME IN-GAME: %08d", FrameCountIngame);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 12), "GAME MODE: %02d", GameMode);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 13), "GAME STATE: %02d", GameState);

	DisplayDebugStringFormatted(NJM_LOCATION(3, 15), "CHARACTER: %01d", CurrentCharacter);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 16), "LEVEL: %02d", CurrentLevel);
	DisplayDebugStringFormatted(NJM_LOCATION(3, 17), "CHAO AREA: %02d", CurrentChaoArea);
	return;
}


void DebugTextUpdateCurrentPage() {

	for (int i = 0; i < 2; i++) {

		if ((Controllers[i].on & Buttons_Y && Controllers[i].press & Buttons_Left)) {
			if (currentPage > 0) {
				currentPage--;
				return;
			}
			else {
				currentPage = 2;
				return;
			}
		}

		if ((Controllers[i].on & Buttons_Y && Controllers[i].press & Buttons_Right)) {
			if (currentPage < 2) {
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
