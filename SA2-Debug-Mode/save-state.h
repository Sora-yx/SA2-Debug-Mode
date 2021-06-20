#pragma once


class SaveStates {
public:
    static SaveStates* get();
    void saveOnSlot(byte slot);
    void loadSlot(byte slot);
    void clearSlot(byte slot);
    bool isSlotEmpty(byte slot);

/*private:
    static SaveStates* instance
        save_struct slots[slot_count];*/
};