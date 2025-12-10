#include "pch.h"
#include "CCamera.h"
#include "CObjMgr.h"

CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera() 
	: m_pTarget(nullptr)
	, m_fCamX(0.f), m_fCamY(0.f)
	, m_fBaseWinX(0.f), m_fBaseWinY(0.f)
	, m_fWorldX(0.f), m_fWorldY(0.f)
	, m_pScreen(nullptr)
{
}

CCamera::~CCamera()
{
}

void CCamera::Initialize(float fWinX, float fWinY)
{
	m_fBaseWinX = fWinX;
	m_fBaseWinY = fWinY;
	m_fActiveWinX = fWinX;
	m_fActiveWinY = fWinY;
	m_bSplitMode = false;
	m_fAnchorX = 0.5f;
	m_fAnchorY = 0.5f;
	m_fDeadLeft = 300.f;
	m_fDeadRight = 300.f;
	m_iQuadrant = -1;
}

void CCamera::SetWorldSize(float fWorldX, float fWorldY)
{
	m_fWorldX = fWorldX;
	m_fWorldY = fWorldY;
}

void CCamera::ClampToWorld()
{
	float fMaxX = m_fWorldX - m_fActiveWinX;
	float fMaxY = m_fWorldY - m_fActiveWinY;

	if (fMaxX < 0) fMaxX = 0.f;
	if (fMaxY < 0) fMaxY = 0.f;

	if (m_fCamX <= 0) m_fCamX = 0;
	if (m_fCamY <= 0) m_fCamY = 0;

	if (m_fCamX >= fMaxX) m_fCamX = fMaxX;
	if (m_fCamY >= fMaxY) m_fCamY = fMaxY;
}

void CCamera::Update()
{
	m_pTarget = CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER);
	//플레이어가 죽었을 경우 클램프를 하고 return 
	if (!m_pTarget)
	{
		ClampToWorld();
		return;
	}
	float fPlayerX = m_pTarget->GetInfo()->fX;
	float fPlayerY = m_pTarget->GetInfo()->fY;
	//데드존 플레이어 기준 범위 설정
	float fLeftBound = m_fCamX + m_fDeadLeft;
	float fRightBound = m_fCamX + (m_fActiveWinX - m_fDeadRight);
	//플레이어가 데드존을 넘을 경우 
	if (fPlayerX < fLeftBound) m_fCamX -= (fLeftBound - fPlayerX);
	if (fPlayerX > fRightBound) m_fCamX += (fPlayerX - fRightBound);

	//float fTargetX = m_pTarget->GetInfo()->fX - m_fWinX * m_fAnchorX;
	float fTargetY = m_pTarget->GetInfo()->fY - m_fActiveWinY * m_fAnchorY;
	//스무딩 적용
	//아니면 바로 카메라 위치 적용
	//m_fCamX = fTargetX;
	m_fCamY = fTargetY;

	ClampToWorld();
}

void CCamera::SetSplitMode(bool IsSplit)
{
	if (IsSplit)
	{
		m_fActiveWinX = m_fBaseWinX;
		m_fActiveWinY = m_fBaseWinY;
	}
	else
	{
		m_fActiveWinX = m_fBaseWinX;
		m_fActiveWinY = m_fBaseWinY;
	}
}

Vec2 CCamera::WorldToScreen(const Vec2& world) const
{
	Vec2 vScreen;

	vScreen.fX = world.fX - m_fCamX;
	vScreen.fY = world.fY - m_fCamY;

	if (m_pScreen && m_iQuadrant >= 0)
	{
		vScreen = m_pScreen->ApplyMirror(vScreen, m_iQuadrant);
	}

	return vScreen;
}
