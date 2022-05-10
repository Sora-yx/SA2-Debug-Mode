#pragma once

struct CharaStruct {
    char player[sizeof(ObjectMaster)];
    char data2[sizeof(EntityData2)];
    char sonicCO2[sizeof(SonicCharObj2)];
    char tailsCO2[sizeof(TailsCharObj2)];
    char eggmanCO2[sizeof(EggmanCharObj2)];
    char knuxCO2[sizeof(KnucklesCharObj2)];
    char mechCO2[sizeof(MechEggmanCharObj2)];
    char data[sizeof(EntityData1)];
    char col[sizeof(CollisionInfo)];
};

struct CameraUnit {
    char cameraBackup[0x2518];
    char cameraPastPosBackup[0xc000];
    char cameraPastRotBackup[0xc000];
    char PosRotBufferIndex;
    char camConstpastPos[0xc000];
    char camConstPastPosIDX;
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
    char timerStopped;
    short gameState;
    char pauseDisabled;
    int pauseValue[3];
    char playerPaused;
    NJS_VECTOR grv;
    CharaStruct charData;
    CameraUnit CameraUnit;
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

void SaveCartPointer();
void SaveEmePtr();