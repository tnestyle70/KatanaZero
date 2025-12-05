#include "pch.h"
#include "CCollisionMgr.h"
#include "CBullet.h"

void CCollisionMgr::RectCollide(list<CObj*> EnemyBullet, list<CObj*> Player)
{
	RECT rc = {};
	for (auto& pBullet : EnemyBullet)
	{
		for (auto& pPlayer : Player)
		{
			if (IntersectRect(&rc, pBullet->GetRect(), pPlayer->GetRect()))
			{
				pBullet->SetDead();
				//pPlayer->SetDead();
			}
		}
	}
}

void CCollisionMgr::CircleCollide(list<CObj*> Dst, list<CObj*> Src)
{
	//원충돌 - 반지름 + 반지름 < 중심 사이의 거리
	for (auto& pDst : Dst)
	{
		for (auto& pSrc : Src)
		{
			if (CheckCircle(pDst, pSrc))
			{
				pDst->SetDead();
				pSrc->SetDead();
			}
		}
	}
}

bool CCollisionMgr::CheckCircle(CObj* pDst, CObj* pSrc)
{
	float fDX = pDst->GetInfo()->fX - pSrc->GetInfo()->fX;
	float fDY = pDst->GetInfo()->fY - pSrc->GetInfo()->fY;
	float fDistance = sqrtf(fDX * fDX + fDY * fDY);
	float fRadiusSum = pDst->GetInfo()->fCX * 0.5f + pSrc->GetInfo()->fCY * 0.5f;
	
	return fDistance <= fRadiusSum;
}

void CCollisionMgr::ParryBullet(list<CObj*> listParry, list<CObj*> listEnemyBullet)
{
	RECT rc = {};
	for (CObj* pParry : listParry)
	{
		for (CObj* pEnemyBullet : listEnemyBullet)
		{
			if (IntersectRect(&rc, pParry->GetRect(), pEnemyBullet->GetRect()))
			{
				dynamic_cast<CBullet*>(pEnemyBullet)->OnParried();
				pEnemyBullet->SetDead();
			}
		}
	}
}
