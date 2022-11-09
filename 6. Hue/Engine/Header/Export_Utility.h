#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "KeyMgr.h"
#include "InfoSubject.h"
#include "Pipeline.h"
BEGIN(Engine)

inline CManagement*		Get_Management(void);
inline CKeyMgr*			Get_KeyMgr(void);
inline CInfoSubject*	Get_InfoSubject(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
