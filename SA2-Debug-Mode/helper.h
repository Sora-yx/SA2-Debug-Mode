#pragma once


#pragma pack(push, 8)
struct DebugStringInfo
{
    __int16 column;
    __int16 row;
    __int16 fontsize;
    int color;
    const char* text;
};
#pragma pack(pop)



//void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawGameText(const char* str_in, const NJS_VECTOR* pos, float scale, const NJS_COLOR* color);
void DrawPolygon(int numPoints, int readAlpha_q, const PolygonPoint* pointArray);

FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);

float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest);
bool IsPointInsideSphere(NJS_VECTOR* center, NJS_VECTOR* pos, float radius);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void FreeMDL(ModelInfo* pointer);
void __cdecl DrawDebugCollision(ObjectMaster* a1);
void __cdecl DrawCol(CollisionData* Col, EntityData1* a2);