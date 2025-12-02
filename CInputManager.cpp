#include "pch.h"
#include "CInputManager.h"

//클래스의 싱글턴 포인터를 프로그램에서 한 번만 선언
CInputManager* CInputManager::m_pInst = nullptr;

CInputManager::CInputManager()
{
	memset(m_bPrevKey, 0, sizeof(m_bPrevKey));
	memset(m_bCurrentKey, 0, sizeof(m_bCurrentKey));
}

CInputManager::~CInputManager()
{
}

CInputManager* CInputManager::GetInst()
{
	if (m_pInst == nullptr)
	{
		m_pInst = new CInputManager;
	}
	return m_pInst;
}

void CInputManager::DestoryInst()
{
	if (m_pInst)
	{
		delete m_pInst;
		m_pInst = nullptr;
	}
}

void CInputManager::Initialize()
{
	//논리키와 실제키 매핑
	m_iVKCode[eKey::UP] = 'W';
	m_iVKCode[eKey::DOWN] = 'S';
	m_iVKCode[eKey::LEFT] = 'A';
	m_iVKCode[eKey::RIGHT] = 'D';
	m_iVKCode[eKey::ATTACK_LEFT] = VK_LBUTTON;
	m_iVKCode[eKey::ATTACK_RIGHT] = VK_RBUTTON;
}

void CInputManager::Update()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);
	//이전 프레임 상태 백업
	memcpy(m_bPrevKey, m_bCurrentKey, sizeof(m_bPrevKey));
	//이번 프레임 상태 갱신
	for (int i = 0; i < eKey::KEY_END; ++i)
	{
		int iVK = m_iVKCode[i];
		SHORT sState = GetAsyncKeyState(iVK);
		//최상위 비트가 눌렸는지 여부 확인
		m_bCurrentKey[i] = (sState & 0x8000) != 0;
	}
}
//누르고 있는 동안
bool CInputManager::KeyPress(eKey eKey) const
{
	int iIndex = eKey;
	return m_bCurrentKey[iIndex];
}
//한 프레임 눌린 순간
bool CInputManager::KeyDown(eKey eKey) const
{
	int iIndex = eKey;
	return m_bCurrentKey[iIndex] && !m_bPrevKey[iIndex];
}
//뗀 순간
bool CInputManager::KeyUp(eKey eKey) const
{
	int iIndex = eKey;
	return !m_bCurrentKey[iIndex] && m_bPrevKey[iIndex];
}
