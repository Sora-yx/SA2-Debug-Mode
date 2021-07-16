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


void FreeMDL(ModelInfo* pointer)
{
	if (pointer) delete(pointer);
}

void FreeAnim(AnimationFile* pointer)
{
	if (pointer) delete pointer;
}



NJS_COLOR DebugFontColor = { 0xffffffff };
char* DebugTextBuffer;
DebugStringInfo* DebugMessages;

Trampoline* deathzoneobj_t;


struct RenderInfoThing
{
	char gap0[8];
	int texparplus4;
	int Thing;
	int unknown2;
	int texparbuf;
	int unknown3;
	int unknown4;
	NJS_TEXLIST* CurrentTexlist;
	int unknown;
	int CurrentTexid;
};

DataPointer(RenderInfoThing*, RenderInfo, 0x2670544);

PolygonPoint testSquare[4] = { 0 };

static const void* const DrawGameText_ptr = (void*)0x667410;
static inline void DrawGameTextASM(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color)
{
	__asm
	{
		push[color]
		push[scale]
		mov eax, [pos]
		mov ecx, [str_in]
		call DrawGameText_ptr
		add esp, 0x8
	}
}


void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color) {
	return DrawGameTextASM(str_in, pos, scale, color);
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