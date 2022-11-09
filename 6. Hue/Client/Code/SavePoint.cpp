#include "stdafx.h"
#include "SavePoint.h"

#include "CubeMgr.h"
#include "Player.h"
USING(Engine)

CSavePoint::CSavePoint(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
	: CObjBasic(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CSavePoint::~CSavePoint(void)
{
	Release();
}

HRESULT CSavePoint::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::GetInstance();
	m_tMtrl.Power = 5.f;


	m_bIsDead = false;
	m_bIsGetSavePoint = false;

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Object_SavePoint";
	//m_pBufferName = L"Buffer_RcTex";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos.x = m_tObjInfo.vPos.x;
	m_pInfoCom->m_vPos.y = m_tObjInfo.vPos.y;
	m_pInfoCom->m_vPos.z = m_tObjInfo.vPos.z - 0.3f;
	m_pInfoCom->m_vScale = { 0.4f, 0.4f, 0.1f };

	m_vecOriginPos = m_pInfoCom->m_vPos;
	return S_OK;
}

void CSavePoint::Update(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

}

void CSavePoint::LateUpdate(void)
{

}

int CSavePoint::ObjectUpdate(void)
{
	//충돌시
	//삭제 처리.

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		if (0.0f < m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 0.0f;
	}
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		if (0.2f > m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z += 0.85f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 0.2f;
	}


	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;
	float fPlayerZ = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.z;

	float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
	float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
	float fDistZ = fPlayerZ - m_pInfoCom->m_vPos.z;
	float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY + fDistZ * fDistZ);

	if (*CCameraObserver::GetInstance()->GetSceneID() != SCENEID::SC_STAGE3)
	{
		float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
		float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
		float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);
		if (fDistance < 1.f && !m_bIsGetSavePoint)
		{
			Engine::Get_SoundMgr()->MyPlaySound(L"GetSavePoint.wav", Engine::CHANNELID::EFFECT);
			m_bIsGetSavePoint = true;
			CCubeMgr::GetInstance()->SetIsSave(true);
			CCubeMgr::GetInstance()->SetSavePos(m_pInfoCom->m_vPos);
			CCubeMgr::GetInstance()->PlusSaveCount();
		}
	}
	else
	{
		float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
		float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
		float fDistZ = fPlayerZ - m_pInfoCom->m_vPos.z;
		float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY + fDistZ * fDistZ);
		if (fDistance < 2.5f && !m_bIsGetSavePoint)
		{
			Engine::Get_SoundMgr()->MyPlaySound(L"GetSavePoint.wav", Engine::CHANNELID::EFFECT);
			m_bIsGetSavePoint = true;
			CCubeMgr::GetInstance()->SetIsSave(true);
			CCubeMgr::GetInstance()->SetSavePos(m_pInfoCom->m_vPos);
			CCubeMgr::GetInstance()->PlusSaveCount();
		}
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);


	return 1;
}



void CSavePoint::Render(void)
{
	
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);

	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	if (m_bIsGetSavePoint)
		m_pTextureCom->SetTexture(1);
	else
		m_pTextureCom->SetTexture(0);


	m_pBufferCom->Render_NoIndices();

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}


void CSavePoint::Release(void)
{
	
}

void CSavePoint::RenderInsideWorld(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
												//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_bIsGetSavePoint)
		m_pTextureCom->SetTexture(1);
	else
		m_pTextureCom->SetTexture(0);

	m_pBufferCom->Render_NoIndices();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}

void CSavePoint::ResetObject(void)
{
	m_pInfoCom->m_vPos = m_vecOriginPos;
	//m_bIsInteraction = true;
}

void CSavePoint::SetTransform(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
}

HRESULT CSavePoint::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer

	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);

	// Texture
	pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"Texture", m_pTextureCom);

	// Transform
	pComponent = Engine::CTransform::Create(g_vLook);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

CSavePoint* CSavePoint::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
{
	CSavePoint* pInstance = new CSavePoint(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}

