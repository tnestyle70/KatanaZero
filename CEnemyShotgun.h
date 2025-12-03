#pragma once
#include "CObj.h"

class CEnemyShotgun : public CObj
{
public:
	CEnemyShotgun();
	~CEnemyShotgun();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Release() override;
	void Render(HDC hDC) override;
public:
	void SetBulletList(list<CObj*>* pBulletList) {m_pBulletList = pBulletList;};
	void SetShotGunEnemyDir();
	void ResolveCollision();
	void SetBulletDir();
private:
	list<CObj*>* m_pBulletList;
};