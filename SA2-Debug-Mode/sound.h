#pragma pack(push, 1)


enum SoundFlags : __int16
{
    SndFlag_Unknown1 = 0x1,
    SndFlag_Unknown2 = 0x2,
    SndFlag_Remove = 0x4,
    SndFlag_Unknown8 = 0x8,
    SndFlag_AutoVolume = 0x10,
    SndFlag_ForceVolume = 0x20,
    SndFlag_AutoPanning = 0x40,
    SndFlag_ForcePanning = 0x80,
    SndFlag_AutoDist = 0x100,
    SndFlag_ForceDist = 0x200,
    SndFlag_Unknown400 = 0x400,
    SndFlag_Unknown800 = 0x800,
    SndFlag_Unknown1000 = 0x1000,
    SndFlag_Unknown2000 = 0x2000,
    SndFlag_SecondaryTimer = 0x4000
};

struct SoundEntry
{
    __int8 unk1;
    __int8 unk2;
    __int8 Bank;
    __int8 Panning;
    __int8 Volume3D;
    __int8 Volume;
    __int8 MaxVolume;
    __int8 unk3;
    SoundFlags Flags;
    __int16 DistanceShort;
    __int16 PlayTime;
    __int16 PlayTime2;
    ObjectMaster* Entity;
    int SoundID;
    NJS_VECTOR* PositionPtr;
    NJS_VECTOR Position;
    float UnknownFloat;
    float DistanceFloat;
};

struct MLTSoundEntry
{
    char bank;
    char id;
    char unk3;
    char unk4;
    char unk5;
    char unk6;
};

struct MLTSoundList
{
    int size;
    MLTSoundEntry* list;
};

struct SoundSystem
{
    int StartIndex;
    int OverflowIndex;
    int unk3;
    int unk4;
    SoundEntry SoundEntries[43];
    void* SomePointers[8];
    MLTSoundList MLTSoundLists[8];
};

#pragma pack(pop)

DataPointer(SoundSystem*, SoundSystemPtr, 0x1A55874);