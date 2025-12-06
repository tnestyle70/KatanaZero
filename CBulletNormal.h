#pragma once
#include "Define.h"
#include "CObj.h"

class CBulletNormal : public CObj
{
public:
	CBulletNormal();
	virtual ~CBulletNormal();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Release() override;
	void Render(HDC hDC) override;
public:
	void OnParried();
public:
	void SetOwner(CObj* pOwner) { m_pOwner = pOwner; }
	CObj* GetOwner() { return m_pOwner; }
private:
	CObj* m_pOwner;
	float m_fInitAngle;
	bool m_bStart;
};
