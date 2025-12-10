#include "pch.h"
#include "CAttack.h"
#include "CObjMgr.h"
#include "CEnemy.h"
#include "CAbstractFactory.h"

CAttack::CAttack()
	: m_bActive(false), m_pOwner(nullptr)
{
}

CAttack::~CAttack()
{
}

void CAttack::Initialize()
{
	//패링 판정 넓이
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 120.f;
	m_bDead = false;
	m_bActive = true;
	m_fActiveTime = 0.2f;
	m_bSuccessAttack = false;
}

int CAttack::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	if (m_bSuccessAttack)
	{
		m_bSuccessAttack = false;
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

void CAttack::Late_Update(float fDeltaTime)
{
}

void CAttack::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CAttack::Release()
{
}

void CAttack::OnAttack(CObj* pEnemy)
{
	//패링이 되었다는 사실만 Bullet에게 넘겨주고 종료
	if (!pEnemy) return;
	dynamic_cast<CEnemy*>(pEnemy)->OnHit();
}

void CAttack::FollowPlayer(float fDeltaTime)
{
	m_pOwner = CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER);

	m_tInfo.fX = m_pOwner->GetInfo()->fX + m_fDirX * m_pOwner->GetInfo()->fCX * 0.5f;
	m_tInfo.fY = m_pOwner->GetInfo()->fY + m_fDirY * m_pOwner->GetInfo()->fCY * 0.5f;
}
