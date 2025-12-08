#pragma once
#include "IMemento.h"
#include <deque>
#include <memory>
#include <unordered_map>
#include "CObjMgr.h"
using Frame = unordered_map<CObj*, unique_ptr<ISnapshot>>;

class CRewindMgr
{
private:
	CRewindMgr();
	CRewindMgr(const CRewindMgr& rhs) = delete;
	CRewindMgr& operator=(CRewindMgr& rObj) = delete;
	~CRewindMgr() {};
public:
	void SetRewinding(bool bRewind) { m_bRewinding = bRewind; }
	bool IsRewinding() { return m_bRewinding; }

	void StoreFrame(CObjMgr& mgr);
	void RewindFrame(CObjMgr& mgr);

	deque<Frame>& GetRewindFrames() { return m_dRewindFrames; }
public:
	static CRewindMgr* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CRewindMgr;
		}
		return m_pInstance;
	}
	static void DestoryInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static CRewindMgr* m_pInstance;
	bool m_bRewinding;
	deque<Frame> m_dRewindFrames;
};