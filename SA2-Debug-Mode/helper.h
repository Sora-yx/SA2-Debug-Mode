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



void __cdecl DrawDebugCollision(ObjectMaster* a1);
void __cdecl DrawCol(CollisionData* Col, EntityData1* a2);