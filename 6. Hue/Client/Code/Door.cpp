#include "stdafx.h"
#include "Door.h"

#include "KeyEffect.h"
#include "CubeMgr.h"
#include "Player.h"
#include "Management.h"
#include "SceneSelector.h"
USING(Engine)

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
	: CObjBasic(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CDoor::~CDoor(void)
{
	Release();
}

HRESULT CDoor::Initialize(void)
{
	m_bIsEndDoor = false;
	m_bIsLock = false;
	m_fMakeEffectTime = 0.f;

	m_pCameraObserver = CCameraObserver::GetInstance();

	m_tMtrl.Ambient = RED;
	m_tMtrl.Diffuse = RED;
	m_tMtrl.Specular = RED;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Object_Door";
	//m_pBufferName = L"Buffer_RcTex";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_pInfoCom->m_vPos.x = m_tObjInfo.vPos.x;
	m_pInfoCom->m_vPos.y = m_tObjInfo.vPos.y;
	m_pInfoCom->m_vPos.z = m_tObjInfo.vPos.z;
	m_pInfoCom->m_vPos.z = 0.6f;

	m_pInfoCom->m_vPos.y -= 0.325f;
	m_pInfoCom->m_vScale = { 0.5f, 0.75f, 0.5f };

	Engine::Get_InfoSubject()->AddData(DATA_DOOR, this);
	m_vecOriginPos = m_pInfoCom->m_vPos;
	return S_OK;
}

void CDoor::Update(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 각도  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.05f)
	{
		CKeyEffect* pEffect = CKeyEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z, false);
		
		int iR = rand() % 26 + 230;
		int iG = rand() % 230;
		pEffect->SetMaterial(iR, iG, 0);
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

void CDoor::LateUpdate(void)
{

}

int CDoor::ObjectUpdate(void)
{
	if (m_bIsEndDoor && !m_bIsLock)
	{
		float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
		float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;

		float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
		float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
		float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);

		if (fDistance < 1.f && CCubeMgr::GetInstance()->Get_CurCubeColor() != m_tObjInfo.tColor)
		{
			if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_Q))
			{
				Engine::Get_SoundMgr()->MyPlaySound(L"DoorClose.wav", Engine::CHANNELID::EFFECT);
				CCubeMgr::GetInstance()->Set_IsRender(true);
				CCubeMgr::GetInstance()->Set_IsStageChange(true);
				CCubeMgr::GetInstance()->PlusSaveCount();
				CCubeMgr::GetInstance()->SetIsSave(false);
			}
		}
	}
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 각도  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_fMakeEffectTime += 1.35f * GET_TIME;

	if (m_fMakeEffectTime > 0.05f)
	{
		CKeyEffect* pEffect = CKeyEffect::Create(m_pGraphicDev);
		pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z, false);
		int iR = rand() % 255;
		int iG = rand() % 255;
		int iB = rand() % 255;
		pEffect->SetMaterial(iR, iG, iB);
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



void CDoor::Render(void)
{
	if (m_bIsLock && *CCameraObserver::GetInstance()->GetSceneID() == SCENEID::SC_STAGE3)
		return;
	m_pGraphicDev->SetMaterial(&m_tMtrl);

	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	if (m_bIsLock)
	{
		m_pLockTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	for (auto& MyEffect : m_listKeyEffect)
		MyEffect->Render();
}


void CDoor::Release(void)
{
	for (auto& MyEffect : m_listKeyEffect)
		Engine::Safe_Delete(MyEffect);
}

void CDoor::RenderInsideWorld(void)
{

	if (m_bIsLock && *CCameraObserver::GetInstance()->GetSceneID() == SCENEID::SC_STAGE3)
	return;
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
												//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	if (m_bIsLock)
	{
		m_pLockTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

}

void CDoor::ResetObject(void)
{
	/*if (m_bIsEndDoor)
		m_bIsLock = true;*/
}

void CDoor::SetTransform(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
}

HRESULT CDoor::AddComponent(void)
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

	m_pTexName = L"Texture_Object_DoorLock";

	// Lock Texture
	pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pLockTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"LockTexture", m_pLockTextureCom);


	// Transform
	pComponent = Engine::CTransform::Create(g_vLook);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo)
{
	CDoor* pInstance = new CDoor(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}

