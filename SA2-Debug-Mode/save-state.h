#pragma once

struct save_struct {
    int lives;
    int score;
    int rings;
    char timeF;
    char timeS;
    char timeM;
    int action;
    int anim;
    Float posX;
    Float posY;
    Float posZ;
    Angle rotY;
    Float spdX;
    Float spdY;
    Float spdZ;
    Float grvX;
    Float grvY;
    Float grvZ;
    Float camPosX;
    Float camPosY;
    Float camPosZ;
};


class SaveStates {

    SaveStates() {}
    static SaveStates* instance;
    save_struct slots[8];

public:
    static SaveStates* getInstance() {
        if (!instance) {
            instance = new SaveStates;
            return instance;
        }
    }

    void saveOnSlot();
    void loadSlot();
    void clearSlot();

};
