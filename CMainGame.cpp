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
#include "CRewindMgr.h"

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

	//CScreenSplit 초기화
	m_ScreenSplit.Initialize(WINCX, WINCY);
	//카메라 ScreenSplit 연결
	CCamera::Get_Instance()->SetScreenSpilt(&m_ScreenSplit);
}

void CMainGame::Update()
{
	//Time Update
	CTimeManager::GetInst()->Update();
	CInputManager::GetInst()->Update();

	//Shift키 입력에 따른 TimeManager의 TimeScale 조정
	if (CInputManager::GetInst()->KeyPress(eKey::TIME_SLOW))
		CTimeManager::GetInst()->SetSlowMotion(0.15f);

	//Shift 보정한 시간값 사용
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();

	auto* pObjList = CObjMgr::Get_Instance();
	auto* pRewindMgr = CRewindMgr::GetInstance();

	//되감기 Memeto 호출
	if (CInputManager::GetInst()->KeyPress(eKey::REWIND))
	{
		if (pRewindMgr->GetRewindFrames().empty())
		{
			pRewindMgr->SetRewinding(false);
			return;
		}
		pRewindMgr->SetRewinding(true);
		pRewindMgr->RewindFrame(*pObjList);
		//Rect만 업데이트해서 Rewinding되는 상태 반영
		CObjMgr::Get_Instance()->Update_Rect();
	}
	else
	{
		CObjMgr::Get_Instance()->Update(fDeltaTime);
		pRewindMgr->SetRewinding(false);
		pRewindMgr->StoreFrame(*pObjList);
	}
	
	//스플릿 모드 On/Off 체크
	auto* pBossObj = CObjMgr::Get_Instance()->Get_Object(OBJ_DOCTOR_BOSS);
	if (dynamic_cast<CDoctorBoss*>(pBossObj)->IsEyeSplitPhase())
		CCamera::Get_Instance()->SetSplitMode(true);
	else
		CCamera::Get_Instance()->SetSplitMode(false);
		
	//Camera
	CCamera::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	auto* pRewindMgr = CRewindMgr::GetInstance();

	if (pRewindMgr->IsRewinding())
	{
		return;
	}
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();

	CObjMgr::Get_Instance()->Update_Rect();

	CObjMgr::Get_Instance()->Late_Update(fDeltaTime);
}

void CMainGame::Render()
{
	m_BackBuffer.Clear(RGB(0, 0, 0));

	HDC HMemDC = m_BackBuffer.GetMemDC();

	//보스 상태 
	auto* pBossObj = CObjMgr::Get_Instance()->Get_Object(OBJ_DOCTOR_BOSS);
	auto* pBoss = dynamic_cast<CDoctorBoss*>(pBossObj);
	
	if (pBoss && pBoss->IsEyeSplitPhase())
	{
		for (int i = 0; i < 4; ++i)
		{
			const RECT& rc = m_ScreenSplit.GetQuad(i);
			HRGN rgn = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
			SelectClipRgn(HMemDC, rgn);

			//카메라에 Quadrant 전달
			CCamera::Get_Instance()->SetQuadrant(i);

			//현재 사분면 인덱스를 전역/렌더 컨텍스트로 전달
			CObjMgr::Get_Instance()->Render(HMemDC);

			SelectClipRgn(HMemDC, nullptr);
			DeleteObject(rgn);
		}
		CCamera::Get_Instance()->SetQuadrant(-1);
	}
	else
	{
		CCamera::Get_Instance()->SetQuadrant(-1);
		CObjMgr::Get_Instance()->Render(HMemDC);
	}
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
