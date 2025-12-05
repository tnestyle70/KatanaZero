#pragma once
#include "CObj.h"
class CParryBullet :
    public CObj
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
};

