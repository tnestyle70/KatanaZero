#include "pch.h"
#include "CBullet.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 1500.f; 
	m_bStart = false;
}

int CBullet::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	if (!m_bStart)
	{
		float fDX = m_pTarget->GetInfo()->fX - m_tInfo.fX;
		float fDY = m_pTarget->GetInfo()->fY - m_tInfo.fY;
		//빗변 구하기
		float fDistance = sqrtf(fDX * fDX + fDY * fDY);

		float fCeta = acosf(fDX / fDistance);
		float fDegree = fCeta * (180 / PI);

		if (fDY <= 0)
		{
			fDegree = 360 - fDegree;
		}
		m_fInitAngle = fDegree;
		m_bStart = true;
	}
	//구한 세타값을 기준으로 이동
	m_tInfo.fX += m_fSpeed * fDeltaTime * cosf((m_fInitAngle + m_fAngle)* (PI / 180));
	m_tInfo.fY += m_fSpeed * fDeltaTime * sinf((m_fInitAngle + m_fAngle) * (PI / 180));

	__super::Update_Rect();

	return NOEVENT;
}

void CBullet::Late_Update(float fDeltaTime)
{
}

void CBullet::Release()
{
}

void CBullet::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

std::unique_ptr<ISnapshot> CBullet::SaveSnapshot() const
{
	auto snap = std::make_unique<BulletSnapshot>();

	snap->fX = m_tInfo.fX;
	snap->fY = m_tInfo.fY;
	snap->fAngle = m_fAngle;
	snap->fSpeed = m_fSpeed;
	snap->bDead = m_bDead;

	return snap;
}

void CBullet::LoadSnapshot(const ISnapshot& snapshot)
{
	auto& snap = static_cast<const BulletSnapshot&>(snapshot);

	m_tInfo.fX = snap.fX;
	m_tInfo.fY = snap.fY;
	m_fAngle = snap.fAngle;
	m_bDead = snap.bDead;
}

void CBullet::OnParried()
{
}
