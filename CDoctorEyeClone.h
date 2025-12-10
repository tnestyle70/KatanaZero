#pragma once
#include "CEnemy.h"
#include "Define.h"

enum class eEyeState
{
    APPEAR, KILL, VANISH
};

class CDoctorEyeClone : public CEnemy
{
public:
	CDoctorEyeClone();
	virtual ~CDoctorEyeClone();
public:
    void Initialize() override;
    int  Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void OnHit()override;
    void UpdateState(float fDeltaTime);
private:
    eEyeState m_eState;
    float m_fAppearDuration;
    float m_fKillDuration;
    float m_fVanishDuration;
};