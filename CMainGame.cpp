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
	//dynamic_cast<CEnemyShotgun*>(pEnemyShotgun)->SetBulletList(&m_listObj[OBJ_BULLET]);
	
	//Boss Enemy 생성 후 BulletList 할당
	CObj* pEnemyDoctorBoss = CAbstractFactory<CDoctorBoss>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_BOSS, pEnemyDoctorBoss);
	//m_listObj[OBJ_BOSS].push_back(pEnemyDoctorBoss);
	pEnemyDoctorBoss->SetTarget(pPlayer);
	//dynamic_cast<CEnemyBoss*>(pEnemyDoctorBoss)->SetBulletList(&m_listObj[OBJ_BOSS_BULLET]);
	
	//Parry Object 생성 후 플레이어 할당
	CObj* pParry = CAbstractFactory<CParry>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ_PARRY, pParry);
	//m_listObj[OBJ_PARRY].push_back(pParry);
	pParry->SetTarget(pPlayer);
	
	//InputManager 초기화
	CInputManager::GetInst()->Initialize();
}

void CMainGame::Update()
{
	//Time Update
	CTimeManager::GetInst()->Update();
	CInputManager::GetInst()->Update();

	//Shift키 입력에 따른 TimeManager의 TimeScale 조정
	//bool bShiftPress = CInputManager::GetInst()->KeyPress(TIME_SLOW);
	//CTimeManager::GetInst()->SetSlowMotion(bShiftPress, 0.15f);

	//Shift 보정한 시간값 사용
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();
	/*
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_listObj[i].begin();
			iter != m_listObj[i].end();)
		{
			int iResult = (*iter)->Update(fDeltaTime);
			if (iResult == DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				iter++;
		}
	}
	*/
	CObjMgr::Get_Instance()->Update(fDeltaTime);
	//CCollisionMgr::RectCollide(m_listObj[OBJ_BULLET], m_listObj[OBJ_PLAYER]);
}

void CMainGame::Late_Update()
{
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();
	/*
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto pObj : m_listObj[i])
		{
			pObj->Late_Update(fDeltaTime);
		}
	}
	*/
	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);
}

void CMainGame::Render()
{
	m_BackBuffer.Clear(RGB(0, 0, 0));

	HDC HMemDC = m_BackBuffer.GetMemDC();
	/*
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto pObj : m_listObj[i])
		{
			pObj->Render(HMemDC);
		}
	}
	*/
	CObjMgr::Get_Instance()->Render(HMemDC);

	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();
	WCHAR cBuf[64];
	swprintf_s(cBuf, L"플레이어 남은 체력 : %2.f", fDeltaTime);
	TextOutW(HMemDC, 10, 40, cBuf, lstrlenW(cBuf));

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
	/*
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			Safe_Delete<CObj*>(pObj);
		}
		m_listObj[i].clear();
	}
	*/
}
