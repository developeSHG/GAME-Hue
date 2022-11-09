#include "stdafx.h"
#include "Hand.h"

#include "Export_Function.h"
#include "Player.h"

USING(Engine)

CHand::CHand(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pCameraObserver(CCameraObserver::GetInstance())
	, m_pCubeMgr(CCubeMgr::GetInstance())
{

}

CHand::~CHand()
{
	Release();
}

HRESULT CHand::Initialize(void)
{
	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_HandIdle";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	//m_pInfoCom->m_vPos = { 0.f, -900.f, 0.f };
	//m_pInfoCom->m_vScale = { 1280.f, 286.f, 0.f };
	//m_fOriginY = -435.f;
	m_pInfoCom->m_vPos = { 0.f, -1000.f, 0.f };
	m_pInfoCom->m_vScale = { 1260.f, 200.f, 0.f };
	m_fOriginY = -355.f;

	return S_OK;
}

void CHand::Update(void)
{
	if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		if (FALSE == *m_pCameraObserver->GetNarrativeMoving())
		{
			/************************ (SongSong) ************************/
			/*  손 등장  */
			if (m_fOriginY <= m_pInfoCom->m_vPos.y)
				m_pInfoCom->m_vPos.y = m_fOriginY;
			else
				m_pInfoCom->m_vPos.y += 1300.f * GET_TIME;

			/************************ (SongSong) ************************/
			/*  왼손오른손 컬러액티브  */
			if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_RBUTTON))
				m_eNextState = HAND_STATE(++m_iSelectColorActive % 2 + 1);
		}
	}

	StateCheck();
	Animation();

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CHand::LateUpdate(void)
{

}

void CHand::Render(void)
{
	if (NARRATIVE_VIEW::FIRST_PERSON != *m_pCameraObserver->GetNarrativeView()
		|| TRUE == *m_pCameraObserver->GetNarrativeMoving())
	{
		m_pInfoCom->m_vPos = { 0.f, -1000.f, 0.f };
		return;
	}

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pTextureCom[m_eCurState]->SetTexture(DWORD(m_tFrame.fCurFrame));
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pBufferCom->Render();

}

void CHand::Release(void)
{
//	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CHand::SetTransform(void)
{
	D3DXMATRIX		matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);

		if (m_pConvertVtx[i].vPos.z < 1.f)
			m_pConvertVtx[i].vPos.z = 1.f;

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &matProj);
	}
}

void CHand::Animation()
{
	if (HAND_STATE::IDLE == m_eCurState)
	{
		if (m_tFrame.iMaxFrame >= m_tFrame.fCurFrame)
			m_tFrame.fCurFrame += m_tFrame.fFrameSpeed * GET_CONSTANTTIME;
		else
			m_tFrame.fCurFrame = 0.f;
	}
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_RBUTTON))
	{
		if (HAND_STATE::LEFT == m_eCurState)
		{
			if (20.f >= m_tFrame.fCurFrame)
				m_tFrame.fCurFrame += m_tFrame.fFrameSpeed * GET_CONSTANTTIME;
		}
		else if (HAND_STATE::RIGHT == m_eCurState)
		{
			if (14.f >= m_tFrame.fCurFrame)
				m_tFrame.fCurFrame += m_tFrame.fFrameSpeed * GET_CONSTANTTIME;
		}
	}
	else if (HAND_STATE::IDLE != m_eCurState
		&& FALSE == *m_pCameraObserver->GetColorCircleActive())
	{
		m_tFrame.fCurFrame += m_tFrame.fFrameSpeed * GET_CONSTANTTIME;
		if (m_tFrame.iMaxFrame < m_tFrame.fCurFrame)
		{
			m_tFrame.fCurFrame = 0.f;
			m_eNextState = HAND_STATE::IDLE;
		}
	}

}

void CHand::StateCheck()
{
	if (m_eNextState == m_eCurState)
		return;

	m_eCurState = m_eNextState;
	m_tFrame.fCurFrame = 0.f;

	switch (m_eCurState)
	{
	case HAND_STATE::IDLE:
		m_tFrame.iMaxFrame = 20;
		m_tFrame.fFrameSpeed = 20.f;
		break;
	case HAND_STATE::LEFT:
		m_tFrame.iMaxFrame = 41;
		m_tFrame.fFrameSpeed = 60.f;
		break;
	case HAND_STATE::RIGHT:
		m_tFrame.iMaxFrame = 27;
		m_tFrame.fFrameSpeed = 40.f;
		break;
	}
}

HRESULT CHand::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);

	// Texture
	TCHAR* pTexName[HAND_STATE::STATE_END] = { L"Texture_HandIdle", L"Texture_HandLeft", L"Texture_HandRight" };
	for (int i = 0; i < HAND_STATE::STATE_END; ++i)
	{
		pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, pTexName[i]);
		m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
		m_mapComponent.emplace(pTexName[i], m_pTextureCom[i]);
	}

	// Transform
	pComponent = Engine::CTransform::Create(g_vLook);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

CHand* CHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHand*	pInstance = new CHand(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
