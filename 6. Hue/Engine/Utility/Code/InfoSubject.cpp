#include "InfoSubject.h"
USING(Engine)
IMPLEMENT_SINGLETON(CInfoSubject)

Engine::CInfoSubject::CInfoSubject(void)
{

}

Engine::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

DATALIST* Engine::CInfoSubject::GetDataList(int iMessage)
{
	auto	iter = m_MapDataList.find(iMessage);

	if (iter == m_MapDataList.end())
		return nullptr;

	return &iter->second;
}

void Engine::CInfoSubject::AddData(int iMessage, void* pData)
{
	if (nullptr != pData)
	{
		auto	iter = m_MapDataList.find(iMessage);
		
		if (iter == m_MapDataList.end())
		{
			m_MapDataList[iMessage] = DATALIST();
		}		

		m_MapDataList[iMessage].push_back(pData);
		Notify(iMessage);
	}
}

void Engine::CInfoSubject::RemoveData(int iMessage, void* pData)
{
	for (auto& MapIter : m_MapDataList)
	{
		auto	IterList = MapIter.second.begin();
		auto	IterListEnd = MapIter.second.end();

		for (; IterList != IterListEnd; ++IterList)
		{
			if (pData == (*IterList))
			{
				MapIter.second.erase(IterList);
				return;
			}			
		}
	}
}

void Engine::CInfoSubject::Release(void)
{
	for (auto& iter : m_MapDataList)
		iter.second.clear();

	m_MapDataList.clear();
}

