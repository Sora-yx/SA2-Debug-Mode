#pragma once

#define CURRENT_MATRIX _nj_current_matrix_ptr_
constexpr auto MaxPage = 4;

//void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawPolygon(int numPoints, int readAlpha_q, const PolygonPoint* pointArray);

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void FreeMDL(ModelInfo* pointer);

void SetMaterialColorOffset(float r, float g, float b, float a);
void ResetMaterialColorOffset();
void njRotateZXY(Rotation* rot);

signed char GetCharacterLevel();
bool isBossLevel();
ModelInfo* LoadDZMDL(const char* name, std::string level);