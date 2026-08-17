#include "stdafx.h"
#include "hud.h"

static NJS_TEXNAME RecBG[2]{};
static NJS_TEXLIST RecBG_TEXLIST = { arrayptrandlength(RecBG, Uint32) };

ObjectMaster* DrawBGObj = nullptr;
extern int currentPage;

static const int hudTexAnimID = 1;
constexpr Sint16 panelU1 = 11;	// 44 / 1024 * 256
constexpr Sint16 panelV1 = 21;	// 84 / 1024 * 256
constexpr Sint16 panelU2 = 244;	// 976 / 1024 * 256
constexpr Sint16 panelV2 = 235;	// 940 / 1024 * 256

NJS_TEXANIM	SA2_HUD_TEXANIM[]{
	{ 0x30, 0x10, 0, 0, 0, 0, 0x100, 0x100, 0, 0x20 },
	{ 0x30, 0x10, 0, 0, panelU1, panelV1, panelU2, panelV2, 1, 0x20 },
};

NJS_SPRITE SA2_HUD_SPRITE = { { 0, 0, 0 }, 1.0, 1.0, 0, &RecBG_TEXLIST , SA2_HUD_TEXANIM };


struct textExtentsData
{
	int page;
	int minCol;
	int maxCol; // one past the last character
	int minRow;
	int maxRow;
	bool valid;
};

textExtentsData textExtents;

float GetHudScale()
{
	if (VerticalResolution < 1.0f)
		return 1.0f;

	return VerticalResolution / hudRefHeight;
}

int GetDebugFontSize()
{
	const int size = (int)(debugFontRefSize * GetHudScale() + 0.5f);

	return size < 8 ? 8 : size;
}

float PixelToHudX(float pixelX)
{
	return (pixelX - HorizontalResolution * 0.5f) / GetHudScale() + hudCenterX;
}

float PixelToHudY(float pixelY)
{
	return pixelY / GetHudScale();
}

void ResetTextExtents()
{
	textExtents.valid = false;
}

void AddTextExtents(int col, int row, int length)
{
	if (textExtents.page != currentPage)
	{
		textExtents.page = currentPage;
		textExtents.valid = false;
	}

	if (!textExtents.valid)
	{
		textExtents.minCol = col;
		textExtents.maxCol = col + length;
		textExtents.minRow = row;
		textExtents.maxRow = row;
		textExtents.valid = true;
		return;
	}

	if (col < textExtents.minCol)
		textExtents.minCol = col;

	if (col + length > textExtents.maxCol)
		textExtents.maxCol = col + length;

	if (row < textExtents.minRow)
		textExtents.minRow = row;

	if (row > textExtents.maxRow)
		textExtents.maxRow = row;
}

bool GetTextExtentsCells(int* minCol, int* maxCol, int* minRow, int* maxRow)
{
	if (!textExtents.valid)
		return false;

	*minCol = textExtents.minCol;
	*maxCol = textExtents.maxCol;
	*minRow = textExtents.minRow;
	*maxRow = textExtents.maxRow;

	return true;
}

bool GetTextPanelRect(hudRect* out)
{
	if (!textExtents.valid || textExtents.page != currentPage)
		return false;

	const float font = (float)GetDebugFontSize();
	const float left = textExtents.minCol * font;
	const float top = textExtents.minRow * font;
	const float width = (textExtents.maxCol - textExtents.minCol) * font;
	const float height = (textExtents.maxRow + 1 - textExtents.minRow) * font;

	const float scale = GetHudScale();

	out->x = PixelToHudX(left) - panelPadX;
	out->y = PixelToHudY(top) - panelPadY;
	out->w = width / scale + panelPadX * 2.0f;
	out->h = height / scale + panelPadY * 2.0f;

	return true;
}

void DeleteDebugManager(ObjectMaster* obj) {
	DrawBGObj = nullptr;
	DeleteKartPointer();
}

void DrawDebugRectangle(ObjectMaster* obj)
{
	if (!currentPage)
		return;

	hudRect rect;

	if (!GetTextPanelRect(&rect))
		return;

	SetMaterial(1, 1, 1, 1);
	njSetTexture(&RecBG_TEXLIST);


	SA2_HUD_TEXANIM[hudTexAnimID].sx = (Sint16)(rect.w + 0.5f);
	SA2_HUD_TEXANIM[hudTexAnimID].sy = (Sint16)(rect.h + 0.5f);

	SA2_HUD_SPRITE.p.x = rect.x;
	SA2_HUD_SPRITE.p.y = rect.y;
	SA2_HUD_SPRITE.sx = 1.0f;
	SA2_HUD_SPRITE.sy = 1.0f;

	njDrawSprite2D(&SA2_HUD_SPRITE, hudTexAnimID, 1.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	ResetMaterial();
}

void DrawDebugObjMain(ObjectMaster* obj) {

	if (GameState != GameStates_LoadFinished)
		return;

	if (obj->Data1.Entity->Action == 0) {
		obj->DeleteSub = DeleteDebugManager;
		obj->DisplaySub_Delayed3 = DrawDebugRectangle;
		obj->Data1.Entity->Action = 1;
	}
}

void LoadObjDrawBG() {
	if (!DrawBGObj) {
		LoadTextureList("BlueRec", &RecBG_TEXLIST);
		DrawBGObj = LoadObject(0, "DrawBG", DrawDebugObjMain, LoadObj_Data1 | LoadObj_Data2);
	}

	if (!ShowHud && !isCheatDisableHud)
		isCheatDisableHud = true;

	if (!HudSpecialEnabled && !isCheatDisableExtraHud)
		isCheatDisableExtraHud = true;
}