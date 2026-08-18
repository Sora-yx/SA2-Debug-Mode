#include "stdafx.h"
#include "basic-to-chunk.h"

static bool renderDZ = false;

// The game's death zones are Basic models (NJS_MODEL) and SA2 has no Basic renderer,
// so we convert each model to a Chunk model the first time it is draw.


static const BasicToChunkOptions DeathZoneChunkOptions = []
{
	BasicToChunkOptions opt;
	opt.forceUseAlpha = true;
	opt.forceIgnoreLight = true;
	opt.forceDoubleSide = true;
	opt.forceBlend = true;
	opt.forceNoTexture = true;
	return opt;
}();

void Delete_DeathZones()
{
	ReleaseAllChunkConversions();
}

static int LevelsWithLessAlpha[] =
{
	LevelIDs_MetalHarbor, LevelIDs_SkyRail
};

void DeathZone_Display(ObjectMaster* obj)
{
	if (!renderDZ || !CurrentDeathZones)
		return;

	njSetTexture(texlist_objtex_common);
	SaveControl3D();
	OnControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL | NJD_CONTROL_3D_ENABLE_ALPHA);

	float alpha = 0.4f;

	for (int i = 0; i < LengthOfArray(LevelsWithLessAlpha); i++)
	{
		if (CurrentLevel == LevelsWithLessAlpha[i])
		{
			alpha = 0.8f;
			break;
		}
	}
	SetMaterial(alpha, 1.0f, 0.0f, 0.0f);

	// we browse all the death zone models loaded in the current level
	// then convert them to chunk format to draw them
	for (DeathZone* zone = CurrentDeathZones; zone->Model; zone++)
	{
		if ((Sint32)zone->Flags < 0)
			continue;

		NJS_OBJECT* chunk = GetChunkObject(zone->Model, DeathZoneChunkOptions);

		if (!chunk)
			continue;

		njPushMatrix(_nj_current_matrix_ptr_);
		ResetRenderSpace();
		ROTATEZ(0, chunk->ang[2]);
		ROTATEX(0, chunk->ang[0]);
		ROTATEY(0, chunk->ang[1]);
		njScale(0, chunk->scl[0], chunk->scl[1], chunk->scl[2]);
	
		
		ProcessChunkModelsWithCallback(chunk, ProcessChunkModel);
		njPopMatrix(1u);
	}

	LoadControl3D();
	ResetMaterial();
}

void DeathZoneRender_Manager(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	switch (data->Action)
	{
	case 0:
		obj->DisplaySub = DeathZone_Display;
		data->Action++;
		break;
	case 1:
		if (GetKeyState('D') & 0x8000)
		{
			bool isActive = GetActiveWindow();

			if (!isActive)
				return;

			renderDZ = !renderDZ;
			SendTimedDebugMessage(renderDZ ? "SHOW DEATH ZONES: ON" : "SHOW DEATH ZONES: OFF", 40);
			data->Action++;
		}
		break;
	case 2:
		if (++data->Timer == 30)
		{
			data->Timer = 0;
			data->Action--;
		}
		break;
	}
}

void LoadDeathZoneObj()
{
	LoadObject(2, "DeathZoneRender", DeathZoneRender_Manager, LoadObj_Data1);
}