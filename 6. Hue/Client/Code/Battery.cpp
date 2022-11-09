#include "stdafx.h"
#include "Battery.h"

#include "KeyEffect.h"
#include "CubeMgr.h"
#include "Player.h"
USING(Engine)

CBattery::CBattery(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
	: CObjBasic(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CBattery::~CBattery(void)
{
	Release();
}

HRESULT CBattery::Initialize(void)
{
	m_fMakeEffectTime = 0.f;

	m_pCameraObserver = CCameraObserver::GetInstance();

	m_tMtrl.Ambient = RED;
	m_tMtrl.Diffuse = RED;
	m_tMtrl.Specular = RED;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;


	m_bIsDead = false;
	m_bIsGetKey = false;

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Object_Battery";
	//m_pBufferName = L"Buffer_RcTex";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos.x = m_tObjInfo.vPos.x;
	m_pInfoCom->m_vPos.y = m_tObjInfo.vPos.y;
	m_pInfoCom->m_vPos.z = -0.3f;
	m_pInfoCom->m_vScale = { 0.2f, 0.35f, 0.1f };

	m_vecOriginPos = m_pInfoCom->m_vPos;
	return S_OK;
}

void CBattery::Update(void)
{
	m_tMtrl.Ambient = RED;
	m_tMtrl.Diffuse = RED;
	m_tMtrl.Specular = RED;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
	m_pInfoCom->m_fAngle[ANGLE_Y] += D3DXToRadian(120.f) * GET_TIME;

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.15f)
	{
		CKeyEffect* pEffect = CKeyEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, false, true);
		m_listKeyEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listKeyEffect.begin();
	auto iter_end = m_listKeyEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->KeyEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listKeyEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}


}

void CBattery::LateUpdate(void)
{

}

int CBattery::ObjectUpdate(void)
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

	float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
	float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
	float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);

	if (fDistance < 1.f && CCubeMgr::GetInstance()->Get_CurCubeColor() != m_tObjInfo.tColor
		&& m_bIsInteraction)
	{
		Engine::Get_SoundMgr()->MyPlaySound(L"BatteryCharging.wav", Engine::CHANNELID::EFFECT);
		m_bIsInteraction = false;
		CCubeMgr::GetInstance()->m_bGetBattery = true;
	}

	m_pInfoCom->m_fAngle[ANGLE_Y] -= D3DXToRadian(120.f) * GET_TIME;

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.125f)
	{
		CKeyEffect* pEffect = CKeyEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, false, true);
		int iR = rand() % 230;
		int iG = rand() % 230;
		pEffect->SetMaterial(0, iG, iR);
		m_listKeyEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listKeyEffect.begin();
	auto iter_end = m_listKeyEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->KeyEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listKeyEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}

	return 1;
}



void CBattery::Render(void)
{
	if (m_bIsInteraction)
	{
		m_pGraphicDev->SetMaterial(&m_tMtrl);

		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();

		for (auto& MyEffect : m_listKeyEffect)
			MyEffect->Render();
	}
}


void CBattery::Release(void)
{
	for (auto& MyEffect : m_listKeyEffect)
		Engine::Safe_Delete(MyEffect);
	//if (m_pVertex)
	//	Engine::Safe_Delete_Array(m_pVertex);
	//if (m_pConvertVtx)
	//	Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CBattery::RenderInsideWorld(void)
{
	if (m_bIsInteraction)
	{
		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
												   //m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();

		for (auto& MyEffect : m_listKeyEffect)
			MyEffect->Render();
	}
}

void CBattery::ResetObject(void)
{
	m_pInfoCom->m_vPos = m_vecOriginPos;
	m_bIsInteraction = true;
}

void CBattery::SetTransform(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
}

HRESULT CBattery::AddComponent(void)
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

CBattery* CBattery::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
{
	CBattery* pInstance = new CBattery(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}

