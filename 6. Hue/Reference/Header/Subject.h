#ifndef Subject_h__
#define Subject_h__

#include "Engine_Include.h"
#include "Observer.h"

BEGIN(Engine)

class ENGINE_DLL CSubject
{
protected:
	CSubject(void);
	virtual ~CSubject(void);

public:
	virtual void	Subscribe(CObserver* pObserver);
	virtual void	UnSubscribe(CObserver* pObserver);
	virtual void	Notify(int iMessage);

private:
	void	Release(void);

protected:
	list<CObserver*>			m_ObserverList;

};

END
#endif // Subject_h__
