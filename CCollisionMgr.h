#pragma once
#include "CObj.h"

class CCollisionMgr
{
public:
	static void RectCollide(list<CObj*> EnemyBullet, list<CObj*> Player);
	static void ParryCollide(list<CObj*>& listParry, list<CObj*>& listBullet);
	static void CircleCollide(list<CObj*> Dst, list<CObj*> Src);
	static bool CheckCircle(CObj* pDst, CObj* pSrc);
	static void ParryBullet(list<CObj*> listParry, list<CObj*> listEnemyBullet);
};