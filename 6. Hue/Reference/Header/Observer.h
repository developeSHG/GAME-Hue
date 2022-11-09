#ifndef Observer_h__
#define Observer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CObserver
{
protected:
	CObserver(void);
public:
	virtual ~CObserver(void);

public:
	virtual void Update(int iMessage)PURE;

};

END

#endif // Observer_h__
