#pragma once
#include "Define.h"

class CTimeManager
{
public:
	static CTimeManager* GetInst();
	static void DestoryInst();
public:
	void SetDeltaTime(float fDeltaTime) { m_fDeltaTime = fDeltaTime; }
	float GetDeltaTime() const { return m_fDeltaTime; }
private:
	static CTimeManager* m_pInst;
	float m_fDeltaTime;
	//밖에서 생성하지 못하도록 private으로 생성자 가두기
	CTimeManager() {};
	~CTimeManager();
};
