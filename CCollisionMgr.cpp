#include "pch.h"
#include "CCollisionMgr.h"

void CCollisionMgr::RectCollide(list<CObj*> Dst, list<CObj*> Src)
{
	RECT rc = {};
	for (auto& pDst : Dst)
	{
		for (auto& pSrc : Src)
		{
			if (IntersectRect(&rc, pDst->GetRect(), pSrc->GetRect()))
			{
				pDst->SetDead();
				pSrc->SetDead();
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
