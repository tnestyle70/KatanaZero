#pragma once
#include "CObj.h"

class CBullet : public CObj
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
};