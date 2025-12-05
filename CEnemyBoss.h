#pragma once
#include "CObj.h"

enum class eBossState
{
	INTRO, PHASE1, STUNNED 
};

class CEnemyBoss : public CObj
{
public:
	CEnemyBoss() {};
	virtual ~CEnemyBoss() {};
public:
	virtual void Initialize()PURE;
	virtual int Update(float fDeltaTime)PURE;
	virtual void Late_Update(float fDeltaTime)PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;
public:
	void SetBulletList(list<CObj*>* pBulletList) { m_pBulletList = pBulletList; }
protected:
	int m_iHP;
	bool m_bDead;
private:
	eBossState m_eState;
	float m_fStateTime;
	list<CObj*>* m_pBulletList;
};