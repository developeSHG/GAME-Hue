#include "stdafx.h"
#include "CameraObserver.h"

#include "Export_Function.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCameraObserver)

CCameraObserver::CCameraObserver(void)
	: m_pInfoSubject(Engine::Get_InfoSubject())
{

}

CCameraObserver::~CCameraObserver(void)
{

}

void CCameraObserver::Update(int iMessage)
{
	list<void*>*	pDataList = m_pInfoSubject->GetDataList(iMessage);
	NULL_CHECK(pDataList);

	switch (iMessage)
	{
	case D3DTS_VIEW:
		m_matView = *((D3DXMATRIX*)pDataList->back());
		break;
	case D3DTS_PROJECTION:
		m_matProj = *((D3DXMATRIX*)pDataList->back());
		break;
	case DATA_NARRATIVE_MOVEING:
		m_bNarrativeMoveing = *((bool*)pDataList->back());
		break;
	case DATA_NARRATIVE_VIEW:
		m_eNarrativeView = *((NARRATIVE_VIEW*)pDataList->back());
		break;
	case DATA_PLAYER:
		m_pPlayer = ((CPlayer*)pDataList->back());
		break;
	case DATA_STATICCAMERA:
		m_pStaticCamera = ((CStaticCamera*)pDataList->back());
		break;
	case DATA_COLORCIRCLEACTIVE:
		m_bColorCircleActive = *((bool*)pDataList->back());
		break;
	case DATA_DOOR:
		m_pDoor = ((CDoor*)pDataList->back());
		break;
	case DATA_SCENEID:
		m_eSceneID = *((SCENEID*)pDataList->back());
		break;
	case DATA_COLORCIRCLE:
		m_pColorCircle = ((CColorCircle*)pDataList->back());
		break;
	case DATA_KEY:
	{
		int iScene = *((SCENEID*)m_pInfoSubject->GetDataList(DATA_SCENEID)->back());
		switch (iScene)
		{
		case SCENEID::SC_STAGE:
			m_pKey = ((CKey*)pDataList->front());
			break;
		case SCENEID::SC_STAGE2:
			m_pKey = ((CKey*)pDataList->back());
			break;
		}

	}
	break;
	case DATA_OUTLINE:
		m_pOutline = ((COutline*)pDataList->back());
		break;
	case DATA_PLAYERTUTO:
		m_pPlayerTuto = ((CPlayerTuto*)pDataList->back());
		break;
	case DATA_PLAYERTUTOPOS:
		m_vPlayerTutoPos = *((D3DXVECTOR3*)pDataList->back());
		break;
	case DATA_TUTOCAMERA:
		m_pTutoCamera = ((CTutoCamera*)pDataList->back());
		break;
	case DATA_LISTTUTOTOBJECT:
		m_listTutoObject = *((list<Engine::CGameObject*>*)pDataList->back());
		break;
	}
}
