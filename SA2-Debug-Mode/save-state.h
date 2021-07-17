#pragma once

#define slot_count 8

struct save_struct {
    unsigned short level;
    unsigned char character;
    int lives;
    int score;
    int rings;
    char timeF;
    char timeS;
    char timeM;
    int action;
    int anim;
    __int16 Status;
    __int16 Powerups;
    short hoverFrames;
    NJS_VECTOR pos;
    Rotation rot;
    NJS_POINT3 acc;
    NJS_VECTOR spd;
    NJS_VECTOR grv;
    ObjectMaster* HeldObject;
    ObjectMaster* HoldTarget;
    CameraInfo cam;
    Float camposX;
    Float camposY;
    Float camposZ;
    ObjectMaster* objlist[8];
    ObjectMaster* SetPtr;
};


class SaveStates {
private:
    SaveStates() {}
    static SaveStates* instance;
    save_struct slots[slot_count];
    void getGameInfo();
    void getPlayerInfo();
    void getCameraInfo();
    void getObjectsState();
    void restoreGameInfo();
    void restorePlayerInfo();
    void restoreCameraInfo();
    void restoreObjectState();
    int timerSlotDelay;
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
    void loadSlot();
    void changeSlot();
    void displaySaveText();

};
