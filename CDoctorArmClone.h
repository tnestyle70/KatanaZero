#pragma once
#include "CEnemy.h"
#include "Define.h"

enum class eArmState
{
	SPAWN, STRIKE, HOLD, RETRACT
};

class CDoctorArmClone : public CEnemy
{
public:
	CDoctorArmClone();
	virtual ~CDoctorArmClone();
public:
    void Initialize() override;
    int  Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void OnHit()override;
    void UpdatePhase(float fDeltaTime);
private:
    eArmState m_eArmState;
    float m_fSpawnTime;
    float m_fStrikeTime;
    float m_fHoldTime;
    float m_fRetractTime;
};