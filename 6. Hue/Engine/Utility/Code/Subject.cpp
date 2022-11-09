#include "Subject.h"

USING(Engine)

Engine::CSubject::CSubject(void)
{

}

Engine::CSubject::~CSubject(void)
{
	Release();
}

void Engine::CSubject::Subscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	m_ObserverList.push_back(pObserver);
}

void Engine::CSubject::UnSubscribe(CObserver* pObserver)
{
	auto	iter		= m_ObserverList.begin();
	auto	iter_end	= m_ObserverList.end();

	for (; iter != iter_end; )
	{
		if ((*iter) == pObserver)
		{
			m_ObserverList.erase(iter);
			return;
		}
		else
			++iter;
	}

}

void Engine::CSubject::Notify(int iMessage)
{
	for (auto& iter : m_ObserverList)
		iter->Update(iMessage);
}

void Engine::CSubject::Release(void)
{
	m_ObserverList.clear();
}

