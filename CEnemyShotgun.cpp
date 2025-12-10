#include "pch.h"
#include "Define.h"
#include "CBullet.h"
#include "CObjMgr.h"
#include "CEnemyShotgun.h"
#include "CAbstractFactory.h"

#include "Vec2.h"
#include "CCamera.h"

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
	m_iHP = 1;
}

int CEnemyShotgun::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	if (m_iHP <= 0)
	{
		m_bDead = true;
		return DEAD;
	}
	
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
	//월드 -> 화면(사분면 미러 포함)
	Vec2 vScreen = CCamera::Get_Instance()->WorldToScreen({ m_tInfo.fX, m_tInfo.fY });
	//화면 기준으로 임시 렉트 계산
	float fLeft = vScreen.fX - m_tInfo.fCX * 0.5f;
	float fTop = vScreen.fY - m_tInfo.fCY * 0.5f;
	float fRight = fLeft + m_tInfo.fCX;
	float fBottom = fTop + m_tInfo.fCY;

	Rectangle(hDC, (int)fLeft, (int)fTop, (int)fRight, (int)fBottom);
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
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

void CEnemyShotgun::OnHit()
{
	//맞았을 때의 처리
	m_iHP -= 1;
}

void CEnemyShotgun::CreateBullet()
{
	//Bullet을 Dir으로 싹 다 리팩토링하기!!
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER);
	//플레이어와 몬스터 사이의 거리를 구하고 빗변으로 나눠서 정규화를 한 Dir을 구한다.
	float fDX = pPlayer->GetInfo()->fX - m_tInfo.fX;
	float fDY = pPlayer->GetInfo()->fY - m_tInfo.fY;
	float fLen = sqrtf(fDX * fDX + fDY * fDY);
	if (fLen < 0.0001f) return;
	//방향 정규화 - 벡터의 방향을 1로 만들기
	float fBaseDirX = fDX / fLen;
	float fBaseDirY = fDY / fLen;

	float fStepAngle = -10.f;

	for (int i = 0; i < BULLET_COUNT; ++i)
	{
		//DirX, Y을 fStepAngle만큼 회전을 시킨다.
		//우선 stepangle을 radian으로 변환
		float fRadian = fStepAngle * (PI / 180);
		//회전 행렬 공식 x' = cos ceta  - sin ceta -> x, y' = sin ceta + cos ceta -> y
		float fDirX = fBaseDirX * cosf(fRadian) - fBaseDirY * sinf(fRadian);
		float fDirY = fBaseDirX * sinf(fRadian) + fBaseDirY * cosf(fRadian);

		CObj* pBullet = CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY);
		pBullet->SetDir(fDirX, fDirY);
		dynamic_cast<CBullet*>(pBullet)->SetOwner(this, eBulletOwner::ENEMY);
		fStepAngle += 5.f;
		//pBullet->SetTarget(CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER));
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
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
