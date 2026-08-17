#pragma once

enum curPage {
	None,
	pPlayerInfo,
	pGameInfo,
	pCharacterInfo,
	pCameraInfo
};

constexpr float hudRefWidth = 640.0f;
constexpr float hudRefHeight = 480.0f;
constexpr float hudCenterX = hudRefWidth / 2.0f;
constexpr float debugFontRefSize = 14.0f;
constexpr float panelPadX = 6.0f;
constexpr float panelPadY = 6.0f;

struct hudRect
{
	float x;
	float y;
	float w;
	float h;
};

float GetHudScale();
int GetDebugFontSize();
float PixelToHudX(float pixelX);
float PixelToHudY(float pixelY);

void PrintPanelText(int col, int row, const char* format, ...);
void ResetTextExtents();
void AddTextExtents(int col, int row, int length);
bool GetTextPanelRect(hudRect* out);
bool GetTextExtentsCells(int* minCol, int* maxCol, int* minRow, int* maxRow);
