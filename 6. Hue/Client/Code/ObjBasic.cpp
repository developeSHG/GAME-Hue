#include "stdafx.h"
#include "ObjBasic.h"
#include "Include.h"
#include "Export_Function.h"
#include "LightEffect.h"
#include "CubeMgr.h"
#include "Player.h"


USING(Engine)

CObjBasic::CObjBasic(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
	: CObj(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
	m_bIsInteraction = true;

	m_fSoundTime = 10.f;
	m_bSound = true;
}

CObjBasic::~CObjBasic(void)
{
	Release();
}

void CObjBasic::ResetObject(void)
{
}

HRESULT CObjBasic::Initialize(void)
{
	//m_pTexName = L"Player_Dust";
	m_fMakeEffectTime = 0.f;
	m_pTexName = m_tObjInfo.tTex.szTex;
	m_tObjInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;			//  임시생성

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = m_tObjInfo.vPos;
	m_pInfoCom->m_vScale = m_tObjInfo.vScale;

	m_pCameraObserver = CCameraObserver::GetInstance();

	m_dwVtxCnt = 36;
	//m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	//m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	//m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	return S_OK;
}

void CObjBasic::Update(void)
{
	SetTransform();
}

void CObjBasic::LateUpdate(void)
{
}

void CObjBasic::Render(void)
{
	m_tMtrl.Ambient = BLACK;
	m_tMtrl.Diffuse = BLACK;
	m_tMtrl.Specular = BLACK;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
												//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tObjInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();

	for (auto& MyEffect : m_listLightEffect)
		MyEffect->Render();
}

void CObjBasic::RenderInsideWorld(void)
{
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
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tObjInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();

	for (auto& MyEffect : m_listLightEffect)
		MyEffect->RenderInside();
}

int CObjBasic::ObjectUpdate(void)
{
	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;

	float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
	float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
	float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);

	if (GET_TIME < 0.7f)
	{
		m_fSoundTime += 1.25f * GET_TIME;
		if (m_fSoundTime > 7.5f)
			m_bSound = true;
	}



	if (fDistance < 3.6f)
	{
		if (m_bSound)
		{
			Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::FIRE);
			Engine::Get_SoundMgr()->MyPlaySound(L"FireEffect.wav", Engine::CHANNELID::FIRE);
			m_bSound = false;
			m_fSoundTime = 0.f;
		}
	}


	SetTransform();

	switch (m_eDecoID)
	{
	case Engine::OBJDECO_FIREPOT:
		MakeFireEffect();
		break;
	case Engine::OBJDECO_TOPFIRE:
		MakeTopFireEffect();
		break;
	case Engine::OBJDECO_LEFTLANTERN:
		MakeLeftLanternEffect();
		break;
	case Engine::OBJDECO_RIGHTLANTERN:
		MakeRightLanternEffect();
		break;
	case Engine::OBJDECOID_END:
		break;
	default:
		break;
	}
	return 1;
}

void CObjBasic::Release(void)
{
	for (auto& MyEffect : m_listLightEffect)
		Engine::Safe_Delete(MyEffect);

}

void CObjBasic::MakeFireEffect(void)
{
	if (GET_TIME < 0.7f)
		m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.08f)
	{
		CLightEffect* pEffect = CLightEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x - 0.37f, m_pInfoCom->m_vPos.y + 0.55f, m_eDecoID);
		m_listLightEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listLightEffect.begin();
	auto iter_end = m_listLightEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->LightEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listLightEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

void CObjBasic::MakeTopFireEffect(void)
{
	if (GET_TIME < 0.7f)
		m_fMakeEffectTime += 1.35f * GET_TIME;


	if (m_fMakeEffectTime > 0.1f)
	{
		CLightEffect* pEffect = CLightEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y - 0.35f, Engine::OBJDECO_TOPFIRE);
		m_listLightEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listLightEffect.begin();
	auto iter_end = m_listLightEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->LightEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listLightEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

void CObjBasic::MakeLeftLanternEffect(void)
{
	if (GET_TIME < 0.7f)
		m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.04f)
	{
		CLightEffect* pEffect = CLightEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x + 0.65f, m_pInfoCom->m_vPos.y - 1.1f, m_eDecoID);
		m_listLightEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listLightEffect.begin();
	auto iter_end = m_listLightEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->LightEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listLightEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

void CObjBasic::MakeRightLanternEffect(void)
{
	if (GET_TIME < 0.7f)
		m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.08f)
	{
		CLightEffect* pEffect = CLightEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x - 0.37f, m_pInfoCom->m_vPos.y + 0.25f, m_eDecoID);
		m_listLightEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listLightEffect.begin();
	auto iter_end = m_listLightEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->LightEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listLightEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

void CObjBasic::SetTransform(void)
{
	const D3DXMATRIX*		matView = m_pCameraObserver->GetView();
	const D3DXMATRIX*		matProj = m_pCameraObserver->GetProj();

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		if (0.6f < m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 0.6f;
	}
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		if (1.5f > m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z += 0.85f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 1.5f;
	}

	D3DXMATRIX matTrans, matScale;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixTranslation(&matTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z);
	D3DXMatrixScaling(&matScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y, m_pInfoCom->m_vScale.z);
	m_pInfoCom->m_matWorld = matScale * matTrans;


}

HRESULT CObjBasic::AddComponent(void)
{
	switch (m_tObjInfo.eType)
	{
	case Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL:
		m_pBufferName = L"Buffer_CubeNormal";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT:
		m_pBufferName = L"Buffer_CubeFront";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullB:
		m_pBufferName = L"Buffer_CubeCullB";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullL:
		m_pBufferName = L"Buffer_CubeCullL";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullR:
		m_pBufferName = L"Buffer_CubeCullR";
		break;
	}
	m_pBufferName = L"Buffer_CubeFront";
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

CObjBasic * CObjBasic::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
{
	CObjBasic* pInstance = new CObjBasic(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
