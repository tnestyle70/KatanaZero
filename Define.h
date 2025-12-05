#pragma once
#define WINCX 1280
#define WINCY 720
#define TILE_SIZE 32
#define PURE = 0
#define PI 3.141592
#define NOEVENT 0
#define DEAD 1

//대쉬 상수 
const float DASH_COOMTIME = 0.36f;
const float DASH_SPEED = 1500.f;
const float DASH_DURATION = 0.18f;
const float DASH_INFINITE_FRAME_TIME = 0.16f;
//공격 상수
const float ATTACK_COOLTIME = 0.1f;
const float ATTACK_DURATION = 0.09f;
//공격시에 살짝 앞으로 끌어당김
const float ATTACK_LUNGE_SPEED = 1000.f;

//Enemy 상수 
//Shotgun enemy 상수 
const int BULLET_COUNT = 5;
const float SPREAD = 20.f;

extern HWND g_hWnd;

enum eObjID
{
	OBJ_PLAYER = 0, OBJ_ENEMY ,OBJ_ENEMY_SHOTGUN, OBJ_BULLET,
	OBJ_PARRY, OBJ_DOCTOR_BOSS, OBJ_BOSS_BULLET, OBJ_END
};

typedef struct Info
{
	float fX, fY;
	float fCX, fCY;
}INFO;

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}