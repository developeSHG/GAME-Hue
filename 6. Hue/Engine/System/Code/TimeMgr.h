#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
public:
	DECLARE_SINGLETON(CTimeMgr)
private:
	CTimeMgr(void);
	~CTimeMgr(void);

public:
	void SetDivideTime(float fTime) { m_fDivideTime = fTime; }

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float		m_fTime;
	float		m_fDivideTime;

public:
	float GetTime(void);
	float GetConstantTime(void);

public:
	HRESULT Initialize(void);
	void Update(void);
};

END
#endif // TimeMgr_h__

