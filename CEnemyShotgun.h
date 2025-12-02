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
};