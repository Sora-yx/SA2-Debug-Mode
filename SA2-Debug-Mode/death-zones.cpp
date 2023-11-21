#include "stdafx.h"

ModelInfo* DZObj[50];
bool renderDZ = false;
uint8_t DZCount = 0;

struct DeathZoneExtern
{
	uint8_t CurLevel;
	const char* levelName;
	uint8_t count;
};

DeathZoneExtern DeathZoneRenderArray[5] = {
	{ LevelIDs_CosmicWall, "CosmicWall", 17},
	{ LevelIDs_CrazyGadget, "CrazyGadget", 47},
	{ LevelIDs_FinalRush, "FinalRush", 18},
	{ LevelIDs_FinalChase, "FinalChase", 6},
	{ LevelIDs_EternalEngine, "EternalEngine", 5}
};

void Delete_DeathZones()
{
	for (uint8_t i = 0; i < DZCount; i++)
	{
		if (DZObj[i])
		{
			FreeMDL(DZObj[i]);
		}
	}
}

void DeathZone_Display(ObjectMaster* obj)
{
	if (!renderDZ || CurrentLevel == LevelIDs_CrazyGadget) //CG DZ really doesn't render properly it's not worth.
		return;

	EntityData1* data = obj->Data1.Entity;

	for (uint8_t i = 0; i < data->Index; i++) {

		njSetTexture(texlist_objtex_common);
		njPushMatrix(_nj_current_matrix_ptr_);
		njControl3D_Backup();
		njControl3D_Add(NJD_CONTROL_3D_CONSTANT_MATERIAL | NJD_CONTROL_3D_ENABLE_ALPHA | NJD_CONTROL_3D_CONSTANT_ATTR);
		SetMaterial(0.4f, 1.0f, 0, 0);

		sub_42D340();

		if (DZObj[i]) 
			ProcessChunkModelsWithCallback(DZObj[i]->getmodel(), ProcessChunkModel);

		njPopMatrix(1u);
		njControl3D_Restore();
		ResetMaterial();
	}
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
		if (++data->field_6 == 30)
		{
			data->field_6 = 0;
			data->Action--;
		}
		break;
	}

}

void LoadDeathZonesModels()
{

	DZCount = 0;

	for (uint8_t i = 0; i < LengthOfArray(DeathZoneRenderArray); i++)
	{
		if (CurrentLevel == DeathZoneRenderArray[i].CurLevel)
		{
			DZCount = DeathZoneRenderArray[i].count;

			for (Uint8 j = 0; j < DZCount; ++j) {
				std::string str = std::to_string(j);
				DZObj[j] = LoadDZMDL(str.c_str(), DeathZoneRenderArray[i].levelName);
			}
		}
	}

	if (DZCount) {
		ObjectMaster* DZManager = LoadObject(2, "DeathZoneRender", DeathZoneRender_Manager, LoadObj_Data1);
		DZManager->Data1.Entity->Index = DZCount;
	}
}


void LoadDeathZoneObj()
{
	//WriteData<5>((int*)0x0612E79, 0x90);
	LoadDeathZonesModels();
}