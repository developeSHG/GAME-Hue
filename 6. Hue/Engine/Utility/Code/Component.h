#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected:
	CComponent(void);
public:
	virtual ~CComponent(void) PURE;

public:
	virtual void	Update(void) {};		//Update가 필요없는 컴포넌트들도 있으므로.
	virtual void	LateUpdate(void) {};
	//virtual void	Release(void) {};
};

END
#endif // Component_h__
