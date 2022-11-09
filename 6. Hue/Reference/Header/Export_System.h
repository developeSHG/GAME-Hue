#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "SoundMgr.h"
BEGIN(Engine)

inline CGraphicDev*		Get_GraphicDev(void);
inline CTimeMgr*		Get_TimeMgr(void);
inline CFrameMgr*		Get_FrameMgr(void);
inline CSoundMgr*		Get_SoundMgr(void);

#include "Export_System.inl"

END
#endif // Export_System_h__
