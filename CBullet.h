#pragma once
#include "Define.h"
#include "CObj.h"
#include "IMemento.h"

enum class eBulletOwner
{
	PLAYER, ENEMY, BOSS, NEUTRAL
};

struct BulletSnapshot : ISnapshot
{
	float fX, fY;
	float fAngle;
	float fSpeed;
	bool bDead;
	eBulletOwner eOwner;
};

class CBullet : public CObj, public IMemento
{
public:
	CBullet();
	virtual ~CBullet();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Release() override;
	void Render(HDC hDC) override;
public:
	std::unique_ptr<ISnapshot> SaveSnapshot() const override;
	void LoadSnapshot(const ISnapshot& snapshot) override;
public:
	void OnParried();
	void SetBulletDir(float fDirX, float fDirY) { m_fDirX = fDirX; m_fDirY = fDirY; };
public:
	void SetOwner(CObj* pOwner, eBulletOwner eOwner) { m_pOwner = pOwner; m_eOwner = eOwner; }
	CObj* GetOwner() { return m_pOwner; }
private:
	CObj* m_pOwner;
	eBulletOwner m_eOwner;
	float m_fInitAngle;
	bool m_bStart;
};