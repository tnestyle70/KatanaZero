#pragma once
#include "Define.h"

class CBackBuffer
{
public:
	CBackBuffer() : m_hMemDC(nullptr), m_hBitmap(nullptr), m_hOldBitmap(nullptr),
		m_iWidth(0), m_iHeight(0) {};
	~CBackBuffer();
public:
	bool Initialize(HWND hWnd, int iWidth, int iHeight);
	void Release();
public:
	HDC GetMemDC() { return m_hMemDC; }
	void Clear(COLORREF color);
	void Present(HDC hDest);
private:
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_iWidth;
	int m_iHeight;
};
