#include "pch.h"
#include "CBackBuffer.h"

CBackBuffer::~CBackBuffer()
{
	Release();
}

bool CBackBuffer::Initialize(HWND hWnd, int iWidth, int iHeight)
{
	Release();

	HDC hWndDC = GetDC(hWnd);
	if (!hWndDC) return false;

	m_hMemDC = CreateCompatibleDC(hWndDC);
	m_hBitmap = CreateCompatibleBitmap(hWndDC, iWidth, iHeight);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	ReleaseDC(hWnd, hWndDC);
	return (m_hMemDC && m_hBitmap);
}

void CBackBuffer::Release()
{
	if (m_hMemDC)
	{
		SelectObject(m_hMemDC, m_hOldBitmap);
		DeleteObject(m_hBitmap);
		DeleteDC(m_hMemDC);

		m_hMemDC = nullptr;
		m_hBitmap = nullptr;
		m_hOldBitmap = nullptr;
	}
}

void CBackBuffer::Clear(COLORREF color)
{
	if (!m_hMemDC) return;
	RECT rc = { 0, 0, m_iWidth, m_iHeight };
	HBRUSH hBrush = CreateSolidBrush(color);
	FillRect(m_hMemDC, &rc, hBrush);
	DeleteObject(hBrush);
}

void CBackBuffer::Present(HDC hDest)
{
	if (!m_hMemDC || !hDest) return;
	BitBlt(hDest, 0, 0, m_iWidth, m_iHeight, m_hMemDC, 0, 0, SRCCOPY);
}
