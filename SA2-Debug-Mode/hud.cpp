#include "stdafx.h"
#include "hud.h"

static NJS_TEXNAME RecBG[2]{};
static NJS_TEXLIST RecBG_TEXLIST = { arrayptrandlength(RecBG, Uint32) };

ObjectMaster* DrawBGObj = nullptr;
extern int currentPage;

NJS_TEXANIM	SA2_HUD_TEXANIM[]{
	{ 0x30, 0x10, 0x10, 0, 0, 0, 0x100, 0x100, 0, 0x20 },
	{ 0x30, 0x10, 0x10, 0, 0, 0, 0x100, 0x100, 1, 0x20 },
};

NJS_SPRITE SA2_HUD_SPRITE = { { 0, 0, 0 }, 1.0, 1.0, 0, &RecBG_TEXLIST , SA2_HUD_TEXANIM };

void DeleteDebugManager(ObjectMaster* obj) {
	DrawBGObj = nullptr;
}


void CalcAndDrawRec(NJS_SPRITE sp, float FontScale) {

	float x;
	float y;

	switch (currentPage) {

	case pPlayerInfo:
		x = 1.80f;
		y = 0.90f;
		break;
	case pGameInfo:
		x = 1.90f;
		y = 0.70f;
		break;
	case pHuntingInfo:
		x = 2.90f;
		y = 0.70f;
		break;
	default:
		x = 1.80f;
		y = 0.70f;
		break;
	}


	SA2_HUD_SPRITE.p = { 1.75f * FontScale * 1.5f,  0.75f * FontScale * 45.0f, 0 };
	SA2_HUD_SPRITE.sx = { x * FontScale * 1.80f };
	SA2_HUD_SPRITE.sy = { y * FontScale * 10.0f };
	njDrawSprite2D(&SA2_HUD_SPRITE, 1, 1, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);

}
void DrawDebugRectangle(ObjectMaster* obj)
{

	if (!currentPage)
		return;

	SetMaterial(1, 1, 1, 1);

	float FontScale;

	if ((float)HorizontalResolution / (float)VerticalResolution > 1.33f)
		FontScale = floor((float)VerticalResolution / 480.0f);
	else
		FontScale = floor((float)HorizontalResolution / 640.0f);


	CalcAndDrawRec(SA2_HUD_SPRITE, FontScale);
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
}