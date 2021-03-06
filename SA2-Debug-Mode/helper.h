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

void initializeDebugText();
void DisplayDebugStringFormatted(Int loc, const char* Format, ...);
void DisplayPlayerInformation();
void SetDebugFontColor(int color);
void GetGoalRing();
void init_CartHack();
cartStruct* getCartPointer();
void DeleteKartPointer();
char* getHintText_r(void* a1, char hintID);
void init_DebuggingObjHack();
void LoadDeathZoneObj();
void Delete_DeathZones();
void DeleteColModels();