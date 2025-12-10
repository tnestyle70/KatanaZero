#pragma once
#include "Vec2.h"
#include "CObj.h"
#include "CScreenSplit.h"
//싱글톤으로 카메라 클래스 생성
//핵심 - 게임 오브젝트 좌표는 월드 좌표
//랜더할 경우에만 screen = world - camera
class CCamera
{
private:
	CCamera();
	CCamera(const CCamera& rhs) = delete; //복사 생성 막기
	CCamera& operator=(CCamera& rhs) = delete; //대입 연산자 막기
	~CCamera();
public:
	void Initialize(float fWinX, float fWinY);
	void SetWorldSize(float fWorldX, float fWorldY);
	void ClampToWorld();
	void Update(); //즉시 플레이어 위치로 스냅

	float WorldToScreenX(float fWolrdX) { return fWolrdX - m_fCamX; }
	float WorldToScreenY(float fWolrdY) { return fWolrdY - m_fCamY; }

	float GetCamX() { return m_fCamX; }
	float GetCamY() { return m_fCamY; }

	void SetAnchor(float fAnchorX, float fAnchorY) { m_fAnchorX = fAnchorX; m_fAnchorY = fAnchorY; }
public:
	void SetSplitMode(bool IsSplit);
	void SetScreenSpilt(CScreenSplit* pSceen) { m_pScreen = pSceen; }
	void SetQuadrant(int iQuad) { m_iQuadrant = iQuad; }
	Vec2 WorldToScreen(const Vec2& world) const;
public:
	static CCamera* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCamera;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	//플레이어 타겟 설정
	CObj* m_pTarget;
	//카메라 월드 좌상단 - 월드맵의 좌상단을 기준으로 삼아서 카메라의 위치를 설정한다.
	float m_fCamX;
	float m_fCamY;
	//원본 윈도우 스크린 좌표
	float m_fBaseWinX;
	float m_fBaseWinY;
	//스플릿 윈도우 스크린 좌표
	float m_fActiveWinX;
	float m_fActiveWinY;
	bool m_bSplitMode;
	//월드 좌표
	float m_fWorldX;
	float m_fWorldY;
	//앵커 
	float m_fAnchorX;
	float m_fAnchorY;
	//데드존
	float m_fDeadLeft;
	float m_fDeadRight;
private:
	static CCamera* m_pInstance;
	CScreenSplit* m_pScreen;
	int m_iQuadrant;
};

