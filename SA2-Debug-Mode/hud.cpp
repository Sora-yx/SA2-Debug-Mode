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
	DeleteKartPointer();
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
	case pCharacterInfo:
		x = 2.40f;
		y = 0.70f;
		break;
	default:
		x = 1.80f;
		y = 0.70f;
		break;
	}

	if (HorizontalResolution >= 1920)
		SA2_HUD_SPRITE.p.x = HorizontalStretch - 10.0f;

	if (HorizontalResolution == 1280)
	{
		SA2_HUD_SPRITE.p.x = HorizontalStretch - 30.0f;
	}

	if (HorizontalResolution < 1280 && HorizontalResolution > 800)
	{
		SA2_HUD_SPRITE.p.x = HorizontalStretch + 32.0 + 20;
	}


	if (HorizontalResolution <= 800)
		SA2_HUD_SPRITE.p.x = HorizontalStretch + 32.0 + 50.0;


	if (CurrentLevel == LevelIDs_KartRace || CurrentLevel == LevelIDs_Route101280) {
		if (HorizontalResolution >= 1920)
			SA2_HUD_SPRITE.p.x = SA2_HUD_SPRITE.p.x + 40.0f;

		SA2_HUD_SPRITE.p.y = VerticalStretch + 120.0f;
	}
	else {
		SA2_HUD_SPRITE.p.y = VerticalStretch + 100.0f;

	}


	if (CurrentLevel == LevelIDs_KartRace || CurrentLevel == LevelIDs_Route101280) {

		SA2_HUD_SPRITE.sx = { x * FontScale * 1.70f };
		SA2_HUD_SPRITE.sy = { y * FontScale * 8.5f };
	}
	else {

		SA2_HUD_SPRITE.sx = { x * FontScale * 1.70f };
		SA2_HUD_SPRITE.sy = { y * FontScale * 10.0f };
	}


	if (HorizontalResolution <= 800)
	{
		SA2_HUD_SPRITE.sx = SA2_HUD_SPRITE.sx + 0.3f;
	}

	if (HorizontalResolution == 800)
	{
		SA2_HUD_SPRITE.p.y = SA2_HUD_SPRITE.p.y - 15.0;
	}

	if (HorizontalResolution < 800)
		SA2_HUD_SPRITE.sy = SA2_HUD_SPRITE.sy + 0.8f;


	njDrawSprite2D(&SA2_HUD_SPRITE, 1, 1, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
}

void DrawDebugRectangle(ObjectMaster* obj)
{
	if (!currentPage)
		return;

	SetMaterial(1, 1, 1, 1);
	njSetTexture(&RecBG_TEXLIST);

	float FontScale;

	if ((float)HorizontalResolution / (float)VerticalResolution > 1.33f)
		FontScale = floor((float)VerticalResolution / 480.0f);
	else
		FontScale = floor((float)HorizontalResolution / 640.0f);

	if (FontScale <= 1.0)
		FontScale *= 1.5;

	if (HorizontalResolution <= 800)
		FontScale += 0.3;

	CalcAndDrawRec(SA2_HUD_SPRITE, FontScale);
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
}