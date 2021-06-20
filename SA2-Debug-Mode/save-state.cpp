#include "stdafx.h"
#include "save-state.h"

const int slot_count = 8;


void SaveStates::saveOnSlot(byte slot) {


};

void SaveStates::loadSlot(byte slot) {

};

void SaveStates::clearSlot(byte slot) {

};


bool SaveStates::isSlotEmpty(byte slot) {

	if (!slot) {
		return false;
	}

	return true;
}