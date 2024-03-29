#include "stdafx.h"
#include "UsercallFunctionHandler.h"

//Free Cam: Credit PKR and Speeps (SADX Debug Mode)

int FreeCamMode = 0;
float FreeCamSpeed = 2.0f;
long double FreeCamMoveX;
long double FreeCamMoveY;
POINT MouseCursorPosition;
bool FreeCamLockCheck = false;
bool FreeCamEnabled = false;
int delayCam = 0;

//hud stuff
bool isGamePaused = false;
bool isCheatDisableHud = false;
bool isCheatDisableExtraHud = false;

auto DoSomethingWithCam = GenerateUsercallWrapper<void (*)(int a1, int a2, int a3)>(noret, 0x4EBCD0, rEAX, rECX, rEDI);

enum FreeCamModes
{
	Camera_None = 0,
	Camera_Look = 1,
	Camera_Move = 2,
	Camera_Zoom = 3,
	Camera_Lock = 4
};

void FreeCam_OnInput()
{
	// Stop if free camera mode is disabled
	if (!FreeCamEnabled)
		return;

	while (ShowCursor(false) >= 0); // Decrease cursor view value until it disappears


	// Toggle camera lock
	if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_LSHIFT) & 0x8000)
	{
		if (!FreeCamLockCheck)
		{
			if (FreeCamMode != Camera_Lock)
			{
				FreeCamMode = Camera_Lock;
				//SendDebugMessage("FREE CAM: LOCKED");
			}
			else
			{
				FreeCamMode = Camera_Look;
				//SendDebugMessage("FREE CAM: UNLOCKED");
			}
			FreeCamLockCheck = true;
		}
	}
	else
		FreeCamLockCheck = false;

	// If camera is locked, stop there
	if (FreeCamMode == Camera_Lock)
		return;

	// Set camera mode
	if (GetKeyState(VK_CONTROL) & 0x8000)
		FreeCamMode = Camera_Zoom;
	else if (GetKeyState(VK_LSHIFT) & 0x8000)
		FreeCamMode = Camera_Move;
	else {
		FreeCamMode = Camera_Look;
	}

	// Get screen center and cursor delta
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	GetCursorPos(&MouseCursorPosition);

	// Set delta and center cursor
	int deltaX = MouseCursorPosition.x - w / 2;
	int deltaY = MouseCursorPosition.y - h / 2;
	FreeCamMoveX = (double)(deltaX * 4);
	FreeCamMoveY = (double)(deltaY * 4);
	SetCursorPos(w / 2, h / 2);

	// Calculate camera movement speed and angle
	float movementSpeed_CurX = FreeCamSpeed * abs(FreeCamMoveX) / 40.0f;
	float movementSpeed_CurY = FreeCamSpeed * abs(FreeCamMoveY) / 40.0f;
	Angle angle_hz_move = -CamEventAngleY - (unsigned __int64)(atan2(0, FreeCamMoveX) * 65536.0 * -0.1591549762031479);
	Angle angle_hz_zoom = -CamEventAngleY - (unsigned __int64)(atan2(FreeCamMoveY, 0) * 65536.0 * -0.1591549762031479);
	Angle angle_vt_zoom = CamEventAngleZ - (unsigned __int64)(atan2(FreeCamMoveY, 0) * 65536.0 * -0.1591549762031479);

	int number = 50;

	// Perform movement
	switch (FreeCamMode)
	{
	case Camera_Look:

		if (FreeCamMoveY > 0) {
			CamEventAngleZ = (int)(CamEventAngleZ - (movementSpeed_CurY * number));
		}
		if (FreeCamMoveY < 0) {
			CamEventAngleZ = (int)(CamEventAngleZ + (movementSpeed_CurY * number));
		}
		if (FreeCamMoveX > 0) {
			CamEventAngleY = (int)(CamEventAngleY - (movementSpeed_CurX * number));
		}
		if (FreeCamMoveX < 0) {
			CamEventAngleY = (int)(CamEventAngleY + (movementSpeed_CurX * number));
		}
		
		//move cam with the stick (by Exant)
		if (isGamePaused) {
			if (MenuButtons_Held[0] & Buttons_Up)
			{
				CamEventPos.x -= njSin(CamEventAngleY) * 2;
				CamEventPos.z -= njCos(CamEventAngleY) * 2;

				CamEventPos.y += njSin(CamEventAngleZ) * 2;
			}
			if (MenuButtons_Held[0] & Buttons_Down)
			{
				CamEventPos.x += njSin(CamEventAngleY) * 2;
				CamEventPos.z += njCos(CamEventAngleY) * 2;

				CamEventPos.y += njSin(-CamEventAngleZ) * 2;
			}
			if (MenuButtons_Held[0] & Buttons_Left)
			{
				CamEventPos.x -= njCos(-CamEventAngleY) * 2;
				CamEventPos.z -= njSin(-CamEventAngleY) * 2;
			}
			if (MenuButtons_Held[0] & Buttons_Right)
			{
				CamEventPos.x += njCos(-CamEventAngleY) * 2;
				CamEventPos.z += njSin(-CamEventAngleY) * 2;
			}
		}

		break;
	case Camera_Move:
		if (FreeCamMoveX < 0)
		{
			CamEventPos.x -= njCos(angle_hz_move) * (movementSpeed_CurX);
			CamEventPos.z -= njSin(angle_hz_move) * (movementSpeed_CurX);
		}
		if (FreeCamMoveX > 0)
		{
			CamEventPos.x += njCos(angle_hz_move) * (movementSpeed_CurX);
			CamEventPos.z += njSin(angle_hz_move) * (movementSpeed_CurX);
		}
		if (FreeCamMoveY > 0)
			CamEventPos.y += movementSpeed_CurY;

		if (FreeCamMoveY < 0)
			CamEventPos.y -= movementSpeed_CurY;
		break;
	case Camera_Zoom:
		if (FreeCamMoveY < 0)
		{
			CamEventPos.x += njCos(angle_hz_zoom) * (movementSpeed_CurY);
			CamEventPos.z += njSin(angle_hz_zoom) * (movementSpeed_CurY);
			CamEventPos.y += njCos(angle_vt_zoom) * (movementSpeed_CurY);
		}
		if (FreeCamMoveY > 0)
		{
			CamEventPos.x -= njSin(angle_hz_zoom) * (movementSpeed_CurY);
			CamEventPos.z += njCos(angle_hz_zoom) * (movementSpeed_CurY);
			CamEventPos.y -= njSin(angle_vt_zoom) * (movementSpeed_CurY);
		}
		break;
	}
}


void FreeCam_CheckInput()
{
	if (GameState != GameStates_Ingame)
		return;

	FreeCam_OnInput();

	if (delayCam > 0) {
		delayCam--;
		return;
	}

	if (GetKeyState('Y') & 0x8000)
	{
		bool isActive = GetActiveWindow();
		
		if (!isActive)
			return;

		FreeCamEnabled = !FreeCamEnabled;

		SendTimedDebugMessage(FreeCamEnabled ? "FREE CAM ENABLED" : "FREE CAM DISABLED", 60);

		while (ShowCursor(true) < 0); // Increase cursor visibility until it shows

		// Center cursor first to avoid jittering after turning it on
		if (FreeCamEnabled)
		{
			int w = GetSystemMetrics(SM_CXSCREEN);
			int h = GetSystemMetrics(SM_CYSCREEN);
			SetCursorPos(w / 2, h / 2);

			//Use Cam Event since the regular cam has issue with angle for some reason
			SetCameraEvent(0, 20);
			DoSomethingWithCam(*(int*)&CameraData.gap1AC[168], 0, 0);
			*(float*)0x1DCFE1C = 1.0f; //Zoom Needed to make angle working
			*(int*)0x1DCFDE0 = 3;
			*(int*)0x1DCFDE4 = 0;
			*(int*)0x1DCFDE8 = 0;
			CamEventPos = CameraData.Position;
			CamEventAngleY = CameraData.Rotation.y;
			CamEventAngleZ = CameraData.Rotation.z;	

			ShowHud = 0;
			HudSpecialEnabled = 0;
			isGamePaused = true;

		}
		else {
			if (CurrentLevel != LevelIDs_ChaoWorld) {

				if (!isCheatDisableHud)
					ShowHud = 1;
			}
			if (!isCheatDisableExtraHud)
				HudSpecialEnabled = 1;

			IsNotPauseHide = 1;
			isGamePaused = false;
			ResetCam(CameraData.gap1AC[168], 0);
		}

		delayCam = 35;
		return;
	}

	//hide UI and stop time
	if (GetKeyState('P') & 0x8000 && !delayCam && FreeCamEnabled)
	{
		if (!isCheatDisableHud) 
			ShowHud = !ShowHud;

		if (!isCheatDisableExtraHud)
			HudSpecialEnabled = !HudSpecialEnabled;

		isGamePaused = !isGamePaused;

		if (!isCheatDisableHud && !isCheatDisableExtraHud)
			SendTimedDebugMessage(ShowHud ? "HUD ENABLED" : "HUD DISABLED", 40);

		delayCam = 60;
	}
}

void RunObjectIndex_r(int index)
{
	if (isGamePaused && FreeCamEnabled && index > 0)
		return;

	RunObjectIndex(index);
}

void init_FreeCamHack()
{
	WriteCall((void*)0x43CF77, RunObjectIndex_r);
}