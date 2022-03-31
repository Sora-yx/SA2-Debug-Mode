#include "stdafx.h"

Trampoline* GoalRing_t = nullptr;
Trampoline* Ring_t = nullptr;
NJS_VECTOR GoalRingPosition = { -1, -1, -1 };

bool isJungleLevel()
{
	return CurrentLevel == LevelIDs_GreenForest || CurrentLevel == LevelIDs_WhiteJungle;
}

void DisplayGoalRingPos(int texFirst, int texPosY)
{
	if (isBossLevel() || CurrentLevel >= LevelIDs_Route101280 || (GoalRingPosition.x == -1 && GoalRingPosition.y == -1 && GoalRingPosition.z == -1))
		return;

	if (MissionNum == 2)
		DisplayDebugStringFormatted(NJM_LOCATION(3, texFirst + texPosY), "CHAO DISTANCE: %.2f", CheckDistance(&GoalRingPosition, &MainCharObj1[0]->Position));
	else if (!EmeraldManagerObj2)
		DisplayDebugStringFormatted(NJM_LOCATION(3, texFirst + texPosY), "GOAL DISTANCE: %.2f", CheckDistance(&GoalRingPosition, &MainCharObj1[0]->Position));
}


void GetGoalRing()
{
	if (!CurrentObjectList || !SETEntries)
		return;

	int listSize = CurrentObjectList->Count;

	SETEntry* Set = SETEntries;
	int setSize = SETFile_->Count;

	if (!setSize)
		return;

	int id;

	for (int i = 0; i < setSize; i++)
	{
		id = Set[i].ID & 0x7FFF;

		if (!id)
			continue;

		if (CurrentObjectList->List[id].Function == GoalRing_Main)
		{
			CurrentObjectList->List[id].DistanceMaybe *= 120; //increase goal ring display distance

			if (Set[i].XRot != 1 && MissionNum == 2) { //if it's a lost chao mission, don't take the regular goal ring pos
				continue;
			}

			GoalRingPosition = Set[i].Position;
			return;
		}
	}

	GoalRingPosition = { -1, -1, -1 };
}

void MissionStartVariableSetup_r()
{
	GetGoalRing();
	return MissionStartVariableSetup();
}

void init_GoalRingHack()
{
	WriteCall((void*)0x43CB80, MissionStartVariableSetup_r);
}