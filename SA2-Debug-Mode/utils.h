#pragma once

#define CURRENT_MATRIX _nj_current_matrix_ptr_
#define MaxPage 4

//void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawPolygon(int numPoints, int readAlpha_q, const PolygonPoint* pointArray);

FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void FreeMDL(ModelInfo* pointer);

DataPointer(float, GlobalMatColorR, 0x25EFFD4);
DataPointer(float, GlobalMatColorG, 0x25EFFD8);
DataPointer(float, GlobalMatColorB, 0x25EFFDC);
DataPointer(float, GlobalMatColorA, 0x25EFFD0);

DataPointer(int, DeathZoneDebug, 0x174B002);
void SetMaterialColorOffset(float r, float g, float b, float a);
void ResetMaterialColorOffset();
void njRotateZXY(Rotation* rot);

static const void* const DrawChunkModelPtr = (void*)0x42E6C0;
static inline void DrawChunkModel(NJS_CNK_MODEL* a1)
{
	__asm
	{
		mov eax, [a1]
		call DrawChunkModelPtr
	}
}

signed char GetCharacterLevel();