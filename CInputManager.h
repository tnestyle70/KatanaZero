#pragma once
#include "Define.h"

enum eKey
{
	UP, DOWN, LEFT, RIGHT, DASH_LEFT, DASH_RIGHT, 
	ATTACK_LEFT, ATTACK_RIGHT, KEY_END
};

class CInputManager
{
public:
	static CInputManager* GetInst();
	static void DestoryInst();
public:
	void Initialize();
	void Update();
public:
	float GetMouseX() { return m_ptMouse.x; }
	float GetMouseY() { return m_ptMouse.y; }
public:
	bool KeyPress(eKey eKey) const;
	bool KeyDown(eKey eKey) const;
	bool KeyUp(eKey eKey) const;
private:
	CInputManager();
	~CInputManager();
private:
	POINT m_ptMouse;
	//싱글턴 패턴 - 전역에서 딱 하나 존재하는 input포인터 멤버 변수, 
	//이걸로 다 접근해서 사용
	static CInputManager* m_pInst;
	//이전, 현재 프레임 키
	bool m_bPrevKey[eKey::KEY_END];
	bool m_bCurrentKey[eKey::KEY_END];
	//가상 키와 논리키 매핑
	int m_iVKCode[eKey::KEY_END];
};