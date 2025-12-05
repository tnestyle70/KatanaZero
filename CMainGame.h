#pragma once
#include "Define.h"
#include "CObj.h"
#include "CBackBuffer.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();
public:
	void Initialize(HWND m_hWnd);
	void Update();
	void Late_Update();
	void Render();
	void Release();
private:
	HWND m_hWnd;
	//list<CObj*> m_listObj[OBJ_END];
	CBackBuffer m_BackBuffer;
};