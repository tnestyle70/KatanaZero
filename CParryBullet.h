#pragma once
#include "Define.h"
#include "IMemento.h"
#include "CObj.h"

struct ParryBulletSnapshot : ISnapshot
{
	float fX, fY;
	float fAngle;
	float fSpeed;
	bool bDead;
};

class CParryBullet : public CObj, public IMemento
{
public:
	CParryBullet();
	virtual ~CParryBullet();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;
public:
	std::unique_ptr<ISnapshot> SaveSnapshot() const override;
	void LoadSnapshot(const ISnapshot& snapshot) override;
};

