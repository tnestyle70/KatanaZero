#pragma once
#include "Define.h"

class CTimeManager
{
private:
	//밖에서 생성하지 못하도록 private으로 생성자 가두기
	CTimeManager() {};
	~CTimeManager() {};
public:
	void Initialize();
	void Update();
public:
	static CTimeManager* GetInst();
	static void DestoryInst();
public:
	void SetDeltaTime(float fDeltaTime) { m_fDeltaTime = fDeltaTime; }
	float GetDeltaTime() const { return m_fDeltaTime * m_fTimeScale; }
	float GetUnscaleDeltaTime() const { return m_fDeltaTime; }
	void SetSlowMotion(bool bEnable, float fLerpTime = 0.15f);
private:
	static CTimeManager* m_pInst;
	DWORD m_dwPrevTime;
	float m_fDeltaTime;
	float m_fTimeScale;
	float m_fTargetScale;
	float m_fStartScale;
	float m_fLerpDuration;
	float m_fLerpElapsed;
	bool m_bSlowMotion;
};
