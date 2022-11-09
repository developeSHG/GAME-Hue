#ifndef InfoSubject_h__
#define InfoSubject_h__

#include "Subject.h"

BEGIN(Engine)

typedef list<void*>		DATALIST;


class ENGINE_DLL CInfoSubject : public CSubject
{
	DECLARE_SINGLETON(CInfoSubject)

private:
	CInfoSubject(void);
	virtual ~CInfoSubject(void);

public:
	DATALIST*		GetDataList(int iMessage);

public:
	void	AddData(int iMessage, void* pData);
	void	RemoveData(int iMessage, void* pData);

private:
	void	Release(void);

private:
	map<int, DATALIST>		m_MapDataList;
};

END
#endif // InfoSubject_h__
