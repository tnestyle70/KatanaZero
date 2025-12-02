#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CEnemyShotgun.h"
#include "CTimeManager.h"
#include "CInputManager.h"
#include "CCollisionMgr.h"

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
	m_listObj[OBJ_PLAYER].push_back(CAbstractFactory<CPlayer>::Create());
	m_listObj[OBJ_ENEMY_SHOTGUN].push_back(CAbstractFactory<CEnemyShotgun>::Create());
	//InputManager 초기화
	CInputManager::GetInst()->Initialize();
}

void CMainGame::Update()
{
	CInputManager::GetInst()->Update();
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_listObj->begin();
			iter != m_listObj->end();)
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
	//CCollisionMgr::RectCollide(m_listObj[OBJ_PLAYER], m_listObj[OBJ_ENEMY_SHOTGUN]);
}

void CMainGame::Late_Update()
{
	float fDeltaTime = CTimeManager::GetInst()->GetDeltaTime();
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto pObj : m_listObj[i])
		{
			pObj->Late_Update(fDeltaTime);
		}
	}
}

void CMainGame::Render()
{
	m_BackBuffer.Clear(RGB(0, 0, 0));

	HDC HMemDC = m_BackBuffer.GetMemDC();

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto pObj : m_listObj[i])
		{
			pObj->Render(HMemDC);
		}
	}
	//BackBuffer를 실제 화면 DC에 복사
	HDC hWndDC = GetDC(m_hWnd);
	m_BackBuffer.Present(hWndDC);
	ReleaseDC(m_hWnd, hWndDC);
}

void CMainGame::Release()
{
	m_BackBuffer.Release();
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			Safe_Delete<CObj*>(pObj);
		}
		m_listObj[i].clear();
	}
}
