#include "pch.h"
#include "CParry.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CParryBullet.h"

CParry::CParry()
	: m_bActive(false), m_pBullet(nullptr), m_pOwner(nullptr)
{
}

CParry::~CParry()
{
}

void CParry::Initialize()
{
	//패링 판정 넓이
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 120.f;
	m_bDead = false;
	m_bActive = true;
	m_fActiveTime = 0.2f;
	m_bSuccessParry = false;
}

int CParry::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	
	if (m_bSuccessParry)
	{
		CreateParryBullet();
		m_bSuccessParry = false;
	}
	
	//플레이어 추적
	FollowPlayer(fDeltaTime);

	if (m_bActive)
	{
		m_fActiveTime -= fDeltaTime;
		if (m_fActiveTime < 0.f)
		{
			m_bActive = false;
			m_fActiveTime = 0.2f;
			m_bDead = true;
			return DEAD;
		}
	}

	__super::Update_Rect();

	return NOEVENT;
}

void CParry::Late_Update(float fDeltaTime)
{
}

void CParry::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CParry::Release()
{
}

void CParry::CreateParryBullet()
{
	CObj* pBullet = CAbstractFactory<CParryBullet>::Create(m_tInfo.fX, m_tInfo.fY);
	pBullet->SetDir(m_fDirX, m_fDirY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_PARRY_BULLET, pBullet);
}

void CParry::FollowPlayer(float fDeltaTime)
{
	m_pOwner = CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER);

	m_tInfo.fX = m_pOwner->GetInfo()->fX + m_fDirX * m_pOwner->GetInfo()->fCX * 0.5f;
	m_tInfo.fY = m_pOwner->GetInfo()->fY + m_fDirY * m_pOwner->GetInfo()->fCX * 0.5f;
}
