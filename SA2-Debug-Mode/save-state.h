#pragma once

struct CharaStruct {
    char data[sizeof(EntityData1)];
    char data2[sizeof(EntityData2)];
    char sonicCO2[sizeof(SonicCharObj2)];
    char tailsCO2[sizeof(TailsCharObj2)];
    char eggmanCO2[sizeof(EggmanCharObj2)];
    char knuxCO2[sizeof(KnucklesCharObj2)];
    char mechCO2[sizeof(MechEggmanCharObj2)];
};

struct CameraUnit {
    byte camera[0x2518];
    byte* camPos;
    byte* camRot;
    char posRotBuffer[2];
    byte pastpos[0xc00];
    char* idk2;
};

struct save_struct {
    unsigned short level;
    unsigned char character;
    int lives;
    int score;
    int rings;
    char timeF;
    char timeS;
    char timeM;
    int timerCart;
    NJS_VECTOR grv;
    CharaStruct charData;
    DynColInfo* dyncol;
    CameraUnit CameraUnit;
    ObjectMaster* ObjectList[8];
};


class SaveStates {
private:
    SaveStates() {}
    static SaveStates* instance;
    save_struct slots[8];
    void getGameInfo();
    void getPlayerInfo();
    void getCameraInfo();
    void getObjectsState();
    void restoreGameInfo();
    void restorePlayerInfo();
    void restoreObjectState();
    int timerMessage;
    const char* message;

public:
    static SaveStates* getInstance() {
        if (!instance) {
            instance = new SaveStates;
            return instance;
        }
    }

    void saveOnSlot();
    void loadSlot(ObjectMaster* obj);
    void changeSlot(Buttons input);
    void displaySaveText();
    void restoreCameraInfo();

};

enum SaveManager {
    InitSaveManager,
    CheckInputs,
    SaveStateMode,
    LoadSaveMode,
    ChangeMode,
    SaveDelay
};
