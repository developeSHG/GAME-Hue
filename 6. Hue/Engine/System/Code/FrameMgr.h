#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)
class ENGINE_DLL CFrameMgr
{
public:
	DECLARE_SINGLETON(CFrameMgr)

private:
	CFrameMgr();
	~CFrameMgr();

public:
	void InitTime();
	bool LimitFrame(float fLimit);
	void RenderFPS();

private:
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CPUTick;

	TCHAR	m_szBuf[MIN_STR] = L"";
	float	m_fTimeCount = 0.f;
	float	m_fLimitCount = 0.f;
	int		m_iFPS_Count = 0;
};

END

#endif		//FrameMgr_h__