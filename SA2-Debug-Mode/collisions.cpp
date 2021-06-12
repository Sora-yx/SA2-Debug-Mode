#include "stdafx.h"

static ModelInfo* Cube;
static ModelInfo* Sphere;
static ModelInfo* Cylinder;
static ModelInfo* HalfSphere;
static ModelInfo* Capsule;

Trampoline* addCol_t;

bool isColDebug = false;

//set color according to the type of col (blue = not solid/player detection, green = solid, red = hurt)
void CheckAndSetColColor(CollisionInfo* Col) {


	if ((Col->Id == 3)) {
		SetMaterialColorOffset(1.0, 0, 0, 0.0); //red
		return;
	}
	else {
		SetMaterialColorOffset(0.0, 1, 0.0, 0.0); //green
	}
}

void __cdecl DrawCollisionInfo(CollisionInfo* ColInfo)
{
	CollisionData* Col; // esi
	ObjectMaster* Obj; // eax
	EntityData1* data; // edi
	int v4; // ebx

	if (ColInfo)
	{
		Col = ColInfo->CollisionArray;
		Obj = ColInfo->Object;
		if (ColInfo->Count)
		{
			if (Col)
			{
				if (Obj)
				{
					data = Obj->Data1.Entity;
					if (data)
					{
						if (ColInfo->Count)
						{
							v4 = ColInfo->Count;
							do
							{
								DrawCol(Col++, data);
								--v4;
							} while (v4);
						}
					}
				}
			}
		}
	}
}


void __cdecl DrawDebugCollision(ObjectMaster* a1)
{

	if (a1->Data1.Entity->Collision == nullptr || !isColDebug)
		return;

	DrawCollisionInfo(a1->Data1.Entity->Collision);

}

void CheckAndDisplayCol(ObjectMaster* a1) {

	if (a1->DisplaySub_Delayed3 == nullptr) {
		a1->DisplaySub_Delayed3 = DrawDebugCollision;
	}
}

//Trampoline "AddToCollisionList" to add a new display on the object so it can draw collision.

#pragma region AddCollision_UserCall
void AddToCollisionlist_original(ObjectMaster* object) {
	const auto colli_ptr = addCol_t->Target();

	__asm
	{
		mov esi, [object]
		call colli_ptr
	}
}

void AddCollision_ori(ObjectMaster* obj) {
	AddToCollisionlist_original(obj);
	CheckAndDisplayCol(obj);
}


static void __declspec(naked) AddCollisionDataASM()
{
	__asm
	{
		push esi // a1
		call AddCollision_ori
		pop esi // a1
		retn
	}
}
#pragma endregion

//the following functions came from SADX
void njTranslateCol(CollisionData* Col, EntityData1* data)
{
	Uint32 Flag; // ebx
	Angle RotY; // eax
	Angle RotZ; // eax
	Angle RotX; // eax
	Angle RotZ2; // eax
	Angle RotX2; // eax
	Angle RotY2; // eax

	Flag = Col->attr;
	if ((Flag & 0x20) == 0)
	{
		njTranslateV(0, &data->Position);
		if ((Flag & 0x8000) == 0)
		{
			if ((Flag & 0x200) != 0)
			{
				RotY = data->Rotation.y;
				if (RotY)
				{
					njRotateY(0, (unsigned __int16)RotY);
				}
				RotZ = data->Rotation.z;
				if (RotZ)
				{
					njRotateZ(0, (unsigned __int16)RotZ);
				}
				RotX = data->Rotation.x;
				if (RotX)
				{
					njRotateX(0, (unsigned __int16)RotX);
					njTranslateV(0, &Col->center);
					return;
				}
			}
			else
			{
				RotZ2 = data->Rotation.z;
				if (RotZ2)
				{
					njRotateZ(0, (unsigned __int16)RotZ2);
				}
				RotX2 = data->Rotation.x;
				if (RotX2)
				{
					njRotateX(0, (unsigned __int16)RotX2);
				}
				RotY2 = data->Rotation.y;
				if (RotY2)
				{
					njRotateY(0, (unsigned __int16)RotY2);
				}
			}
		}
	}
	njTranslateV(0, &Col->center);
}

void DrawColCube2Model(EntityData1* data, CollisionData* col)
{
	Uint32 v3; // ebx
	Angle v5; // eax
	Angle v6; // eax
	int v7; // eax
	int v8; // eax
	NJS_OBJECT* v9; // esi
	Float sx; // [esp+0h] [ebp-18h]
	Float sy; // [esp+4h] [ebp-14h]
	Float sz; // [esp+8h] [ebp-10h]

	v3 = col->attr;
	njPushMatrix(0);
	njTranslateCol(col, data);
	if ((v3 & 0x20) == 0 && (v3 & 0x8000) == 0)
	{
		v5 = data->Rotation.x;
		if (v5 || data->Rotation.z)
		{
			if ((v3 & 0x200) != 0)
			{
				if (v5)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
			}
			else
			{
				if (data->Rotation.y)
				{
					njRotateY(0, (unsigned __int16)-LOWORD(data->Rotation.y));
				}
				if (data->Rotation.x)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
				v6 = data->Rotation.y;
				if (v6)
				{
					njRotateY(0, (unsigned __int16)v6);
				}
			}
		}
	}
	v7 = col->rotation.y;
	if (v7)
	{
		njRotateY(0, (unsigned __int16)v7);
	}
	v8 = col->rotation.z;
	if (v8)
	{
		njRotateZ(0, (unsigned __int16)v8);
	}
	sz = col->param3 * 0.2;
	sy = col->param2 * 0.2;
	sx = col->param1 * 0.2;
	njScale(0, sx, sy, sz);
	v9 = Cube->getmodel();


	CheckAndSetColColor(data->Collision);
	DrawObject(v9);
	ResetMaterialColorOffset();
	njPopMatrix(1u);
}

void DrawColCylinder2Model(CollisionData* Col, EntityData1* Data)
{
	Uint32 v3; // ebx
	int v4; // eax
	int v5; // eax
	int v6; // eax
	int rotZ; // eax
	int v8; // eax
	int v9; // eax
	NJS_OBJECT* v10; // esi
	float XScale; // [esp+Ch] [ebp-8h]
	float sy; // [esp+10h] [ebp-4h]

	v3 = Col->attr;
	XScale = Col->param1 * 0.1;
	sy = Col->param2 * 0.1;
	njPushMatrix(0);
	njTranslateCol(Col, Data);
	if ((v3 & 0x200) != 0)
	{
		v4 = Col->rotation.y;
		if (v4)
		{
			njRotateY(0, (unsigned __int16)v4);
		}
		v5 = Col->rotation.z;
		if (v5)
		{
			njRotateZ(0, (unsigned __int16)v5);
		}
		v6 = Col->rotation.x;
		if (v6)
		{
			njRotateX(0, (unsigned __int16)v6);
		}
	}
	else
	{
		rotZ = Col->rotation.z;
		if (rotZ)
		{
			njRotateZ(0, (unsigned __int16)rotZ);
		}
		v8 = Col->rotation.x;
		if (v8)
		{
			njRotateX(0, (unsigned __int16)v8);
		}
		v9 = Col->rotation.y;
		if (v9)
		{
			njRotateY(0, (unsigned __int16)v9);
		}
	}
	njScale(0, XScale, sy, XScale);
	v10 = Cylinder->getmodel();

	CheckAndSetColColor(Data->Collision);
	DrawObject(v10);
	ResetMaterialColorOffset();
	njPopMatrix(1u);
}

void DrawColCubeModel(EntityData1* data, CollisionData* col)
{
	Uint32 v3; // ebx
	Angle v5; // eax
	Angle v6; // eax
	int v7; // eax
	NJS_OBJECT* v8; // esi
	Float sx; // [esp+0h] [ebp-18h]
	Float sy; // [esp+4h] [ebp-14h]
	Float sz; // [esp+8h] [ebp-10h]

	v3 = col->attr;
	njPushMatrix(0);
	njTranslateCol(col, data);
	if ((v3 & 0x20) == 0 && (v3 & 0x8000) == 0)
	{
		v5 = data->Rotation.x;
		if (v5 || data->Rotation.z)
		{
			if ((v3 & 0x200) != 0)
			{
				if (v5)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
			}
			else
			{
				if (data->Rotation.y)
				{
					njRotateY(0, (unsigned __int16)-LOWORD(data->Rotation.y));
				}
				if (data->Rotation.x)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
				v6 = data->Rotation.y;
				if (v6)
				{
					njRotateY(0, (unsigned __int16)v6);
				}
			}
		}
	}
	v7 = col->rotation.y;
	if (v7)
	{
		njRotateY(0, (unsigned __int16)v7);
	}
	sz = col->param3 * 0.2;
	sy = col->param2 * 0.2;
	sx = col->param1 * 0.2;
	njScale(0, sx, sy, sz);
	v8 = Cube->getmodel();


	CheckAndSetColColor(data->Collision);
	DrawObject(v8);
	ResetMaterialColorOffset();
	njPopMatrix(1u);
}

void DrawColCylinderModel(EntityData1* data, CollisionData* col)
{
	Uint32 v3; // ebx
	Angle rotX; // eax
	NJS_OBJECT* v6; // esi
	float sx; // [esp+Ch] [ebp-8h]
	float sy; // [esp+10h] [ebp-4h]

	v3 = col->attr;
	sx = col->param1 * 0.1;
	sy = col->param2 * 0.1;
	njPushMatrix(0);
	njTranslateCol(col, data);
	if ((v3 & 0x20) == 0 && (v3 & 0x8000) == 0)
	{
		rotX = data->Rotation.x;
		if (rotX || data->Rotation.z)
		{
			if ((v3 & 0x200) != 0)
			{
				if (rotX)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
				if (data->Rotation.y)
				{
					njRotateY(0, (unsigned __int16)-LOWORD(data->Rotation.y));
				}
			}
			else
			{
				if (data->Rotation.y)
				{
					njRotateY(0, (unsigned __int16)-LOWORD(data->Rotation.y));
				}
				if (data->Rotation.x)
				{
					njRotateX(0, (unsigned __int16)-LOWORD(data->Rotation.x));
				}
				if (data->Rotation.z)
				{
					njRotateZ(0, (unsigned __int16)-LOWORD(data->Rotation.z));
				}
			}
		}
	}
	njScale(0, sx, sy, sx);
	v6 = Cylinder->getmodel();


	CheckAndSetColColor(data->Collision);
	DrawObject(v6);
	ResetMaterialColorOffset();
	njPopMatrix(1u);
}

void DrawColSphereModel(CollisionData* Col, EntityData1* Data)
{
	NJS_OBJECT* v3; // esi
	float XScale; // [esp+0h] [ebp-4h]

	XScale = Col->param1 * 0.1;
	njPushMatrix(0);
	njTranslateCol(Col, Data);
	njScale(0, XScale, XScale, XScale);
	v3 = Sphere->getmodel();

	CheckAndSetColColor(Data->Collision);
	DrawObject(v3);
	ResetMaterialColorOffset();
	njPopMatrix(1u);
}

void __cdecl DrawCol(CollisionData* Col, EntityData1* a2)
{
	if (Col)
	{
		if (a2)
		{
			switch (Col->form)
			{
			case 0:
				DrawColSphereModel(Col, a2);
				break;
			case 1:
				DrawColCylinderModel(a2, Col);
				break;
			case 2:
				DrawColCylinder2Model(Col, a2);
				break;
			case 3:
				DrawColCubeModel(a2, Col);
				break;
			case 4:
				DrawColCube2Model(a2, Col);
				break;
			case 6:
				//sub_79ED30(Col, Col, a2);
				break;
			case 9:
				//sub_79F1A0(a2, Col);
				break;
			default:
				return;
			}
		}
	}
}

int inputDelay = 0;
void CheckController_ColDebug() {

	if (inputDelay > 0) {
		inputDelay--;
		return;
	}

	for (int i = 0; i < 2; i++) {

		if (Controllers[i].on & Buttons_L && Controllers[i].on & Buttons_R) {
			isColDebug = !isColDebug;
			inputDelay = 20;
			return;
		}
	}
}


void Collisions_Init()
{
	addCol_t = new Trampoline((int)0x47E750, (int)0x47E757, AddCollisionDataASM);
	Cube = LoadMDL("cube", ModelFormat_Chunk);
	Sphere = LoadMDL("sphere", ModelFormat_Chunk);
	Cylinder = LoadMDL("cylinder", ModelFormat_Chunk);
	HalfSphere = LoadMDL("half-cylinder", ModelFormat_Chunk);
	Capsule = LoadMDL("Capsule", ModelFormat_Chunk);
}