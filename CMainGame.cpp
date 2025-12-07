#include "pch.h"
#include "CMainGame.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CEnemyShotgun.h"
#include "CEnemyBoss.h"
#include "CDoctorBoss.h"
#include "CTimeManager.h"
#include "CInputManager.h"
#include "CCollisionMgr.h"
#include "CParry.h"
#include "CCamera.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	//더블 버퍼 초기화 
	if (!m_BackBuffer.Initialize(hWnd, WINCX, WINCY))
	{
		return;
	}
	//TimeManager 초기화 
	CTimeManager::GetInst()->Initialize();
	//플레이어 생성
	CObj* pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);
	
	//Shotgun Enemy 생성 후 BulletList 할당
	CObj* pEnemyShotgun = CAbstractFactory<CEnemyShotgun>::Create(); 
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY_SHOTGUN, pEnemyShotgun);
	pEnemyShotgun->SetTarget(pPlayer);
	
	//Boss Enemy 생성 후 BulletList 할당
	CObj* pEnemyDoctorBoss = CAbstractFactory<CDoctorBoss>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_BOSS, pEnemyDoctorBoss);
	pEnemyDoctorBoss->SetTarget(pPlayer);
	
	//InputManager 초기화
	CInputManager::GetInst()->Initialize();

	//Camera 초기화
	CCamera::Get_Instance()->Initialize(WINCX, WINCY);
	CCamera::Get_Instance()->SetWorldSize(WORLDX, WORLDY);
	CCamera::Get_Instance()->SetAnchor(0.5f, 0.9f);
}

void CMainGame::Update()
{
	//Time Update
	CTimeManager::GetInst()->Update();
	CInputManager::GetInst()->Update();


	//Shift키 입력에 따른 TimeManager의 TimeScale 조정
	if (CInputManager::GetInst()->KeyPress(eKey::TIME_SLOW))
		CTimeManager::GetInst()->SetSlowMotion(0.15f);

	//되감기 Memeto 호출
	//if (CInputManager::GetInst()->KeyPress(eKey::REWIND))
	//	CTimeManager::GetInst()->SetSlowMotion(0.0f);

	//Shift 보정한 시간값 사용
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();

	CObjMgr::Get_Instance()->Update(fDeltaTime);

	//Camera
	CCamera::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();

	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);
}

void CMainGame::Render()
{
	m_BackBuffer.Clear(RGB(0, 0, 0));

	HDC HMemDC = m_BackBuffer.GetMemDC();

	CObjMgr::Get_Instance()->Render(HMemDC);

	//BackBuffer를 실제 화면 DC에 복사
	HDC hWndDC = GetDC(m_hWnd);
	m_BackBuffer.Present(hWndDC);
	ReleaseDC(m_hWnd, hWndDC);
}

void CMainGame::Release()
{
	m_BackBuffer.Release();
	CTimeManager::DestoryInst();
	CObjMgr::Get_Instance()->Release();
}
