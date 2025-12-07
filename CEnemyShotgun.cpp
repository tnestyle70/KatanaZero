#include "pch.h"
#include "Define.h"
#include "CBullet.h"
#include "CObjMgr.h"
#include "CEnemyShotgun.h"
#include "CAbstractFactory.h"

CEnemyShotgun::CEnemyShotgun()
{
}

CEnemyShotgun::~CEnemyShotgun()
{
}

void CEnemyShotgun::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY - 50.f, 100.f, 100.f };
	//처음 바라보는 방향 180도, 오른쪽으로 이동하면 0도
	m_fAngle = 180.f;
	m_fSpeed = 300.f;
	m_fAttackCoolTime = 2.f;
	m_fStep = 0.f;
	m_fBulletAngle = 0.f;
}

int CEnemyShotgun::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	
	if (m_fAttackCoolTime > 0.f)
	{
		m_fAttackCoolTime -= fDeltaTime;
		if (m_fAttackCoolTime < 0.f)
		{
			CreateBullet();
			m_fAttackCoolTime = 2.f;
		}
	}

	m_tInfo.fX += m_fSpeed * fDeltaTime * cosf(m_fAngle * (PI / 180)); 
	m_tInfo.fY -= m_fSpeed * fDeltaTime * sinf(m_fAngle * (PI / 180));

	ResolveCollision();

	__super::Update_Rect();

	return NOEVENT;
}

void CEnemyShotgun::Late_Update(float fDeltaTime)
{

}

void CEnemyShotgun::Release()
{
	
}

void CEnemyShotgun::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

std::unique_ptr<ISnapshot> CEnemyShotgun::SaveSnapshot() const
{
	auto snap = std::make_unique<EnemyShotgunSnapshot>();

	snap->fX = m_tInfo.fX;
	snap->fY = m_tInfo.fY;
	snap->fCX = m_tInfo.fCX;
	snap->fCY = m_tInfo.fCY;
	snap->fSpeed = m_fSpeed;
	snap->fAngle = m_fAngle;
	snap->bDead = m_bDead;

	return snap;
}

void CEnemyShotgun::LoadSnapshot(const ISnapshot& snapshot)
{
	auto& snap = static_cast<const EnemyShotgunSnapshot&>(snapshot);
	
	m_tInfo.fX = snap.fX;
	m_tInfo.fY = snap.fY;
	m_tInfo.fCX = snap.fCX;
	m_tInfo.fCY = snap.fCY;
	m_fSpeed = snap.fSpeed;
	m_fAngle = snap.fAngle;
	m_bDead = snap.bDead;
}

void CEnemyShotgun::OnHit(int iDamage)
{
	//맞았을 때의 처리
}

void CEnemyShotgun::CreateBullet()
{
	float fStepAngle = -10.f;
	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		CObj* pBullet = CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY);
		pBullet->SetAngle(fStepAngle);
		fStepAngle += 5.f;
		pBullet->SetTarget(CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
		//m_pBulletList->push_back(pBullet);
	}
	return;
}

void CEnemyShotgun::ResolveCollision()
{
	//충돌 처리 
	if (m_tRect.left <= 50.f)
	{
		m_tInfo.fX += m_tInfo.fCX * 0.5f; 
		m_fAngle = 180.f - m_fAngle;
	}
	if (m_tRect.right >= WINCX - 50.f)
	{
		m_tInfo.fX -= m_tInfo.fCX * 0.5f;
		m_fAngle = 180.f - m_fAngle;
	}
}

void CEnemyShotgun::SetBulletDir()
{
	//적이 바라보는 방향 기준으로 5개의 총알이 발사되도록 구현
	
}
