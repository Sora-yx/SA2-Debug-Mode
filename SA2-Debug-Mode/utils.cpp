#include "stdafx.h"

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest)
{
	return sqrtf(powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2));
}

bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius)
{
	return GetDistance(center, pos) <= radius;
}

const char* ModelFormatStrings[]
{
	"collision",
	"chunk",
	"battle"
};

//Load Object File
ModelInfo* LoadMDL(const char* name, ModelFormat format) {
	std::string fullPath;

	if (format == ModelFormat_Chunk)
	{
		fullPath = "resource\\gd_PC\\Models\\";
	}


	if (format == ModelFormat_Basic)
	{
		fullPath = "resource\\gd_PC\\Collisions\\";
	}

	fullPath += name;

	switch (format)
	{
	case ModelFormat_Basic:
		fullPath += ".sa1mdl";
		break;
	case ModelFormat_Chunk:
		fullPath += ".sa2mdl";
		break;
	case ModelFormat_SA2B:
		fullPath += ".sa2bmdl";
		break;
	}

	const char* foo = fullPath.c_str();

	ModelInfo* temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	if (temp->getformat() == format)
	{
		PrintDebug("[SA2 Debug Mode] Loaded %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}
	else {
		PrintDebug("[SA2 Debug Mode] Failed loading %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}

	return temp;
}

//Load DZ File
ModelInfo* LoadDZMDL(const char* name, std::string level) {

	std::string fullPath;

	fullPath = "resource\\gd_PC\\deathzones\\" + level + "\\";
	fullPath += name;
	fullPath += ".sa2mdl";

	const char* foo = fullPath.c_str();

	ModelInfo* temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	if (temp->getformat() == ModelFormat_Chunk)
	{
		PrintDebug("[SA2 Debug Mode] Loaded Death Zones %s model: %s.", ModelFormatStrings[(int)ModelFormat_Chunk - 1], name);
	}
	else {
		PrintDebug("[SA2 Debug Mode] Failed loading Death Zone %s model: %s.", ModelFormatStrings[(int)ModelFormat_Chunk - 1], name);
	}

	return temp;
}

void FreeMDL(ModelInfo* pointer)
{
	if (pointer) 
		delete(pointer);
}

void FreeAnim(AnimationFile* pointer)
{
	if (pointer) delete pointer;
}

void SetMaterialColorOffset(float r, float g, float b, float a)
{
	GlobalMatColorR = r;
	GlobalMatColorG = g;
	GlobalMatColorB = b;
	GlobalMatColorA = a;
	return;
}

void ResetMaterialColorOffset()
{
	GlobalMatColorR = 0.0f;
	GlobalMatColorG = 0.0f;
	GlobalMatColorB = 0.0f;
	GlobalMatColorA = 0.0f;
	return;
}

void njRotateZXY(Rotation* rot)
{
	njRotateZ(CURRENT_MATRIX, rot->z);
	njRotateX(CURRENT_MATRIX, rot->x);
	njRotateY(CURRENT_MATRIX, rot->y);
}

signed char GetCharacterLevel() {
	for (int i = 0; i < 33; i++)
	{
		if (CurrentLevel == StageSelectLevels[i].Level)
		{
			return StageSelectLevels[i].Character;
		}
	}

	return -1;
}


//just so I don't have to type "HelperFunctions.X"
void DisplayDebugStringFormatted(Int loc, const char* Format, ...) {
	return HelperFunctionsGlobal.DisplayDebugStringFormatted(loc, Format);
}

void SetDebugFontColor(int color) {
	return HelperFunctionsGlobal.SetDebugFontColor(color);
}


bool isBossLevel()
{
	return CurrentLevel >= LevelIDs_BigFoot && CurrentLevel <= LevelIDs_EggGolemE ||
		CurrentLevel == LevelIDs_SonicVsShadow1 || CurrentLevel == LevelIDs_TailsVsEggman1
		|| CurrentLevel == LevelIDs_TailsVsEggman2 || CurrentLevel == LevelIDs_KnucklesVsRouge || CurrentLevel == LevelIDs_SonicVsShadow2;
}

void CharColliOff(taskwk* twp)
{
	auto cwp = twp->cwp;
	auto nbinfo = cwp->nbInfo;
	auto info = cwp->info;

	if (nbinfo)
	{
		auto attr = &info->attr;
		do
		{
			*attr |= 0x10u;
			attr += 12;
			--nbinfo;
		} while (nbinfo);
	}
}

void CharColliOn(taskwk* a1)
{

	auto col = a1->cwp;
	auto nbINfo = col->nbInfo;
	auto colInfo = col->info;

	if (nbINfo)
	{
		auto attr = &colInfo->attr;
		do
		{
			*attr &= 0xFFFFFFEF;
			attr += 12;
			--nbINfo;
		} while (nbINfo);
	}
}