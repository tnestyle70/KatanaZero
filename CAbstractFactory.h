#pragma once
#include "CObj.h"
//CObj 객체들을 초기화할 때 추상 팩토리로 한 번에 초기화
template <typename T>
class CAbstractFactory
{
	CAbstractFactory() {};
	~CAbstractFactory() {};
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}
	//위치 초기화
	static CObj* Create(float fX, float fY)
	{
		CObj* pObj = new T(fX, fY);
		pObj->Intialize();
		return pObj;
	}
	//크기, 위치 초기화
	static CObj* Create(float fCX, float fCY, float fX, float fY)
	{
		CObj* pObj = new T(fCX, fCY, fX, fY);
		pObj->Intialize();
		return pObj;
	}
};