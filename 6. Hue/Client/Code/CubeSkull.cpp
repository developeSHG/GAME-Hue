#include "stdafx.h"
#include "CubeSkull.h"

#include "Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "Player.h"
#include "SkullDust.h"

#include "CollisionMgr.h"
USING(Engine)


CCubeSkull::CCubeSkull(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeSkull::~CCubeSkull(void)
{
	Release();
}

HRESULT CCubeSkull::Initialize(void)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

	m_iRotTime = 0;

	m_bToRight = false;
	m_eCurState = SKULLSTATE::SKULLSTATE_END;
	m_eCurState = SKULLSTATE::SKULL_IDLE;
	

	m_bIsReadyToFall = true;

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_fCrashTime = 0.f;

	m_pTexName = m_tCubeInfo.tTex.szTex;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();
	
	m_vecOriginPos = m_pInfoCom->m_vPos;

	m_pInfoCom->m_vScale = D3DXVECTOR3(0.8f, 0.8f, 1.f);

	return S_OK;
}

void CCubeSkull::Update(void)
{
	if (!m_bIsMovingCube)
	{
		switch (m_eCurState)
		{
		case SKULLSTATE::SKULL_IDLE:
			break;
		case SKULLSTATE::SKULL_READY:
			ReadyUpdate();
			break;
		case SKULLSTATE::SKULL_FALL:
			FallUpdate();
			break;
		case SKULLSTATE::SKULL_CRASH:
			CrashUpdate();
			break;
		case SKULLSTATE::SKULL_UP:
			UpUpdate();
			break;
		case SKULLSTATE::SKULLSTATE_END:
			break;
		default:
			break;
		}
	}

	if (m_bIsMovingCube)
		FirstMovingUpdate();

	//1. 만약 플레이어가 밑에 가면
	//해당 횟수만큼 떨고

	//2. 해당 횟수가 되면 떨어진다

	//3. 충돌처리.....

	//4. 충돌되면 다시 원래 자리로 돌아간다
	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;
	float fDistX = fabsf(fPlayerX - m_pInfoCom->m_vPos.x);
	float fDistY = fabsf(fPlayerY - m_pInfoCom->m_vPos.y);

	if (fDistX < 0.7f && fDistY < 4.f && m_bIsReadyToFall && fPlayerY < m_pInfoCom->m_vPos.y)
	{
		m_eNextState = SKULLSTATE::SKULL_READY;
		m_bIsReadyToFall = false;
	}


	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		if (1.f < m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 1.f;
	}


	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		if (1.5f > m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z += 0.45f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 1.5f;
	}


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	auto iter_begin = m_plistSkullDust.begin();
	auto iter_end = m_plistSkullDust.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->DustUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_plistSkullDust.erase(iter_begin);
		}
		else
			++iter_begin;
	}


	//SetTransform();
	StateChange();
}

void CCubeSkull::LateUpdate(void)
{
}

void CCubeSkull::Render(void)
{

	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
											
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */

	m_pTextureCom->SetTexture(0);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pFirstBufferCom->Render_NoIndices();
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pBufferCom->Render_NoIndices();


	for (auto& MyDust : m_plistSkullDust)
		MyDust->Render();

	//Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.f, 0);
}

void CCubeSkull::RenderInsideWorld(void)
{
	if (m_tCubeInfo.tColor != Engine::CUBE_COLOR::CUBE_COLOR_END)
		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
	else
		m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);
													
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	m_pTextureCom->SetTexture(0);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pFirstBufferCom->Render_NoIndices();
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pBufferCom->Render_NoIndices();

	for (auto& MyDust : m_plistSkullDust)
		MyDust->Render();

	//Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.f, 0);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}

void CCubeSkull::RenderWhite(void)
{
	//m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	///************************ (SongSong) ************************/
	///*  카메라 시점에 따른 클리핑  */
	//if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	//m_pBufferCom->Render_NoIndices();
}

void CCubeSkull::Release(void)
{
	for (auto& MyDust : m_plistSkullDust)
		Engine::Safe_Delete(MyDust);
}

void CCubeSkull::StateChange()
{
	if (m_eCurState != m_eNextState)
	{
		m_eCurState = m_eNextState;

		switch (m_eCurState)
		{
		case SKULLSTATE::SKULL_IDLE:
			break;
		case SKULLSTATE::SKULL_READY:
			if ((Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
			{
				Engine::Get_SoundMgr()->MyPlaySound(L"SkullFallReady.wav", Engine::CHANNELID::SKULL);
				Engine::Get_SoundMgr()->MyPlaySound(L"SkullFallReady.wav", Engine::CHANNELID::SKULL);
			}
			break;
		case SKULLSTATE::SKULL_FALL:
			break;
		case SKULLSTATE::SKULL_CRASH:
			if ((Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
			{
				Engine::Get_SoundMgr()->MyPlaySound(L"SkullFallCrash.wav", Engine::CHANNELID::SKULL);
				Engine::Get_SoundMgr()->MyPlaySound(L"SkullFallCrash.wav", Engine::CHANNELID::SKULL);
			}
			break;
		case SKULLSTATE::SKULL_UP:
			break;
		case SKULLSTATE::SKULLSTATE_END:
			break;
		default:
			break;
		}
	}
}

void CCubeSkull::SetTransform(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		if (1.f < m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 1.f;
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

void CCubeSkull::ReadyUpdate()
{
	int a = 5;

	if (m_iRotTime > 50)
	{
		m_iRotTime = 0;
		m_pInfoCom->m_fAngle[ANGLE_Z] = 0.f;
		m_bToRight = true;
		m_eNextState = SKULL_FALL;
		return;
	}

	if (m_bToRight)
	{
		m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(180.f) * GET_TIME;
		if (D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Z]) > 2.25f)
		{
			m_bToRight = false;
			m_iRotTime++;
			return;
		}
	}
	else
	{
		m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(180.f) * GET_TIME;
		if (D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Z]) < -2.25f)
		{
			m_bToRight = true;
			m_iRotTime++;
			return;
		}
	}
}


void CCubeSkull::MakeSkullDust(void)
{
	Engine::CUBE_COLOR eColor = CCubeMgr::GetInstance()->Get_CurCubeColor();
	CSkullDust* pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_TWO, false);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_TWO, true);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_THREE, false);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_THREE, true);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_ONE, false);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_ONE, true);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_FOUR, false);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);

	pDust = CSkullDust::Create(m_pGraphicDev);
	pDust->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, SKULLDUSTEFFECTID::SKULLDUST_FOUR, true);
	pDust->SetMaterial(eColor);
	m_plistSkullDust.push_back(pDust);
}

void CCubeSkull::FallUpdate()
{
	m_pInfoCom->m_vPos.y -= 5.5f * GET_TIME;

	//충돌!
	if (CCollisionMgr::GetInstance()->IsCollisionSkullToCube(this))
	{
		m_eNextState = SKULL_CRASH;
		MakeSkullDust();
	}

}

void CCubeSkull::CrashUpdate()
{
	m_fCrashTime += 1.f * GET_TIME;

	if (m_fCrashTime > 2.f)
	{
		m_fCrashTime = 0.f;
		m_eNextState = SKULL_UP;
	}
}

void CCubeSkull::UpUpdate()
{
	m_pInfoCom->m_vPos.y += 4.5f * GET_TIME;

	if (m_pInfoCom->m_vPos.y > m_vecOriginPos.y)
	{
		m_pInfoCom->m_vPos.y = m_vecOriginPos.y;
		m_bIsReadyToFall = true;
		m_eNextState = SKULL_IDLE;
	}

}

HRESULT CCubeSkull::AddComponent(void)
{
	switch (m_tCubeInfo.eType)
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
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR:
		m_pBufferName = L"Buffer_CubeCullLR";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB:
		m_pBufferName = L"Buffer_CubeCullBackAndB";
		break;
	}
	m_pBufferName = L"Buffer_CubeCullB";
	m_pTexName = L"Texture_CubeObstacle_Skull";
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);
	
	m_pBufferName = L"Buffer_CubeCullBackAndB";
	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pFirstBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"BufferSecond", m_pFirstBufferCom);

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


CCubeSkull * CCubeSkull::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeSkull* pInstance = new CCubeSkull(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
