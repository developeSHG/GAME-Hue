#include "stdafx.h"
#include "Stage3.h"

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
/************************ (SongSong) ************************/
/*  GameObject  */
#include "Player.h"
#include "Cube.h"
#include "CubeFirecracker.h"
#include "CubeFireparent.h"
/************************ (SongSong) ************************/
/*  UI  */
#include "StaticCamera.h"
#include "ColorCircle.h"
#include "CubeMgr.h"


CStage3::CStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{
}

CStage3::~CStage3(void)
{
	Release();
}

HRESULT CStage3::Initialize(void)
{
	m_fMakeKingTime = 0.f;
	m_bMoreKingCracker = true;
	m_iMoreKingCracker = 0;
	m_bIsPlayerEndingSound = false;
	m_bLateInit = false;
	m_fMakeFireTime = 0.f;
	m_iMakeFireTime = 0;
	m_iExlodeTime = 0;
	/************************ (SongSong) ************************/
	/*  Cube 로드  */
	CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_STAGE3);
	CCubeMgr::GetInstance()->Set_CurCubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);

	FAILED_CHECK_RETURN(Add_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI(), E_FAIL);

	m_bIsKingFire = false;
	Engine::Get_SoundMgr()->StopAll();
	/*Engine::Get_SoundMgr()->PlayBGM(L"Stage3BGM.wav");*/

	CCubeMgr::GetInstance()->Set_IsRender(FALSE);

	return S_OK;
}

void CStage3::Update(void)
{
	if (m_iMoreKingCracker > 4)
		m_bMoreKingCracker = false;



	if (g_bIsRenderEnding && !m_bIsPlayerEndingSound)
	{
		CCubeMgr::GetInstance()->Set_CurCubeColor(Engine::CUBE_COLOR::CUBE_RED);
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::BGM);
		Engine::Get_SoundMgr()->PlayBGM(L"Ending.mp3");
		m_bIsPlayerEndingSound = true;
	}


	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;
	float fPlayerZ = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.z;

	if ((fPlayerX > -20.f && fPlayerX < -15.f) && (fPlayerY > -40.f && fPlayerY < -30.f) && (fPlayerZ > 28.f && fPlayerZ < 35.f))
		g_bIsRenderEnding = true;
	Engine::CScene::Update();

	CCubeMgr::GetInstance()->Update();
	
	auto iter_begin = m_listFireparent.begin();
	auto iter_end = m_listFireparent.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->UpdateFireparent();

		if (0 == iEvent)
		{
			if (!(*iter_begin)->GetIsKing())
				CreateFirecracker((*iter_begin)->Get_InfoCom()->m_vPos, false);
			else
				CreateKingcracker((*iter_begin)->Get_InfoCom()->m_vPos);
			m_iExlodeTime++;
			if(m_iExlodeTime % 3 ==0)
				Engine::Get_SoundMgr()->MyPlaySound(L"FireworkExplode.mp3", Engine::CHANNELID::FIREEXPLODE);
			delete (*iter_begin);
			iter_begin = m_listFireparent.erase(iter_begin);

		}
		else
			++iter_begin;
	}

	auto iter_anotherbegin = m_listFirecracker.begin();
	auto iter_anotherend = m_listFirecracker.end();

	for (; iter_anotherbegin != iter_anotherend; )
	{
		int iEvent = (*iter_anotherbegin)->UpdateFirework();

		if (0 == iEvent)
		{
			if (m_bMoreKingCracker)
			{
				if ((*iter_anotherbegin)->GetIsKing())
				{
					int iRand = rand() % 2000;
					if (iRand < 30)
					{
						CreateFirecracker((*iter_anotherbegin)->Get_InfoCom()->m_vPos, true);
					}

				}
			}
			delete (*iter_anotherbegin);
			iter_anotherbegin = m_listFirecracker.erase(iter_anotherbegin);
		}
		else
			++iter_anotherbegin;
	}
	if (g_bIsRenderEnding)
		m_fMakeFireTime += 3.f * GET_TIME;

	if (m_fMakeFireTime > 4.2f && g_bIsRenderEnding && !m_bIsKingFire)
	{
		m_iMakeFireTime += 4;
		
		CreateFireparent();
		Engine::Get_SoundMgr()->MyPlaySound(L"FireworkFlying.wav", Engine::CHANNELID::FIREWORK);
		CreateFireparent();
		CreateFireparent();
		CreateFireparent();
		//Engine::Get_SoundMgr()->MyPlaySound(L"FireworkFlying.wav", Engine::CHANNELID::FIREWORK);
		m_fMakeFireTime = 0.f;
	}

	if (m_iMakeFireTime > 33 && !m_bIsKingFire)
	{
		m_fMakeKingTime += 1.f * GET_TIME;
		if (m_fMakeKingTime > 5.f)
			m_bIsKingFire = true;

		if (m_bIsKingFire)
		{
			MakeKingFire();

			//킹폭탄 날라가는 소리
			Engine::Get_SoundMgr()->MyPlaySound(L"FireworkFlying.wav", Engine::CHANNELID::FIREWORK);
			Engine::Get_SoundMgr()->MyPlaySound(L"FireworkFlying.wav", Engine::CHANNELID::FIREWORK);
			Engine::Get_SoundMgr()->MyPlaySound(L"FireworkFlying.wav", Engine::CHANNELID::FIREWORK);
		}
	}

	

	
}

void CStage3::LateUpdate(void)
{
	Engine::CScene::LateUpdate();

	CCubeMgr::GetInstance()->LateUpdate();

}

void CStage3::Render(void)
{
	CCubeMgr::GetInstance()->RenderOutsideWorld();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (auto& MyParent : m_listFireparent)
		MyParent->Render();
	for (auto& MyFireCracker : m_listFirecracker)
		MyFireCracker->Render();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//CObjectMgr::GetInstance()->RenderOutsideWorld();
	Engine::CScene::Render();

}

void CStage3::Release(void)
{
	for (auto& MyFireParent : m_listFireparent)
		Engine::Safe_Delete(MyFireParent);

	for (auto& MyFireCracker : m_listFirecracker)
		Engine::Safe_Delete(MyFireCracker);

	Engine::CScene::Release();
}

void CStage3::CreateFirecracker(D3DXVECTOR3 vecParentPos, bool IsKing)
{
	static bool IsSound = false;

	if (IsKing)
	{
		m_iMoreKingCracker++;

		if (!IsSound)
		{
			//킹폭탄 터지는 소리
			Engine::Get_SoundMgr()->MyPlaySound(L"FireworkExplode.mp3", Engine::CHANNELID::FIREEXPLODE);
			Engine::Get_SoundMgr()->MyPlaySound(L"FireworkExplode.mp3", Engine::CHANNELID::FIREEXPLODE);
			IsSound = true;
		}
	}
	
	for (int i = 0; i < 150; i++)
	{
		int iRot = rand() % 360;

		D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		iRot = rand() % 360;
		D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		iRot = rand() % 360;
		D3DXMatrixRotationY(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		Engine::CUBE_INFO tInfo;
		tInfo.vPos = vecParentPos;
		tInfo.vScale = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
		tInfo.tColor = Engine::CUBE_COLOR::CUBE_BLUE;

		CCubeFirecracker*	pFire = CCubeFirecracker::Create(m_pGraphicDev, tInfo);

		int iRandomColor = rand() % 7;
		switch (iRandomColor)
		{
		case 0:
			pFire->SetMaterial(BLUE);
			break;
		case 1:
			pFire->SetMaterial(RED);
			break;
		case 2:
			pFire->SetMaterial(PINK);
			break;
		case 3:
			pFire->SetMaterial(PURPLE);
			break;
		case 4:
			pFire->SetMaterial(GREEN);
			break;
		case 5:
			pFire->SetMaterial(ORANGE);
			break;
		case 6:
			pFire->SetMaterial(YELLOW);
			break;
		default:
			break;
		}
		pFire->SetDir(vecDir);
		if (IsKing)
			pFire->SetIsKing();
		m_listFirecracker.emplace_back(pFire);
	}
}

void CStage3::CreateKingcracker(D3DXVECTOR3 m_parentPos)
{
	static bool IsSound = false;
	if (!IsSound)
	{
		//킹폭탄 터지는 소리
		Engine::Get_SoundMgr()->MyPlaySound(L"FireworkExplode.mp3", Engine::CHANNELID::FIREEXPLODE);
		Engine::Get_SoundMgr()->MyPlaySound(L"FireworkExplode.mp3", Engine::CHANNELID::FIREEXPLODE);
		IsSound = true;
	}

	for (int i = 0; i < 2000; i++)
	{
		int iRot = rand() % 360;

		D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		iRot = rand() % 360;
		D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		iRot = rand() % 360;
		D3DXMatrixRotationY(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		Engine::CUBE_INFO tInfo;
		tInfo.vPos = m_parentPos;
		tInfo.vScale = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
		tInfo.tColor = Engine::CUBE_COLOR::CUBE_BLUE;

		CCubeFirecracker*	pFire = CCubeFirecracker::Create(m_pGraphicDev, tInfo);

		int iRandomColor = rand() % 7;
		switch (iRandomColor)
		{
		case 0:
			pFire->SetMaterial(BLUE);
			break;
		case 1:
			pFire->SetMaterial(RED);
			break;
		case 2:
			pFire->SetMaterial(PINK);
			break;
		case 3:
			pFire->SetMaterial(PURPLE);
			break;
		case 4:
			pFire->SetMaterial(GREEN);
			break;
		case 5:
			pFire->SetMaterial(ORANGE);
			break;
		case 6:
			pFire->SetMaterial(YELLOW);
			break;
		default:
			break;
		}
		pFire->SetDir(vecDir);
		pFire->SetIsKing();
		m_listFirecracker.emplace_back(pFire);
	}
}

void CStage3::CreateFireparent()
{

	D3DXVECTOR3 vecPos;
	vecPos = D3DXVECTOR3(15.f, -30.f, 40.f);
	vecPos.x = rand() % 6 + 8.f;
	vecPos.y = ((rand() % 15) - 45.f);
	vecPos.z = rand() % 18 + 26.f;
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 3.f, -2.f);
	//x축 회전 30도

	int iRot = rand() % 60;

	D3DXMATRIX mRot;
	D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
	D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	int iRand = rand() % 2;
	if (iRand == 0)
	{
		D3DXMATRIX mRot;
		iRot = rand() % 20;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	}
	else
	{
		D3DXMATRIX mRot;
		iRot = rand() % 20;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(-iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	}

	Engine::CUBE_INFO tInfo;
	tInfo.vPos = vecPos;
	tInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
	CCubeFireparent* pFireparent = CCubeFireparent::Create(m_pGraphicDev, tInfo);
	pFireparent->SetDir(vecDir);
	pFireparent->SetMaterial(BLACK);
	m_listFireparent.emplace_back(pFireparent);
	
}


HRESULT CStage3::Add_Environment(void)
{
	
	return S_OK;
}

HRESULT CStage3::Add_GameObject(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Player", pGameObject);

	CCubeMgr::GetInstance()->SetPlayer(m_pPlayer);

	m_mapLayer.emplace(LAYER_GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CStage3::Add_UI(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	auto	iter = m_mapLayer.find(LAYER_GAMEOBJECT);
	if (iter == m_mapLayer.end())
		return E_FAIL;

	Engine::CGameObject* pPlayer = nullptr;
	pPlayer = iter->second->FindObject(L"Player");
	m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	const Engine::CComponent*	pTransform = pPlayer->FindComponent(L"Transform");
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	pGameObject = CStaticCamera::Create(m_pGraphicDev, dynamic_cast<const Engine::CTransform*>(pTransform));
	dynamic_cast<CStaticCamera*>(pGameObject)->SetPlayer(m_pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	pGameObject = CColorCircle::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ColorCircle", pGameObject);


	m_mapLayer.emplace(LAYER_UI, pLayer);
	return S_OK;
}

void CStage3::MakeKingFire()
{
	D3DXVECTOR3 vecPos;
	vecPos = D3DXVECTOR3(15.f, -30.f, 40.f);
	vecPos.x = 12.f;
	vecPos.y = -32.f;
	vecPos.z = 35.f;
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
	//x축 회전 30도


	Engine::CUBE_INFO tInfo;
	tInfo.vPos = vecPos;
	tInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
	CCubeFireparent* pFireparent = CCubeFireparent::Create(m_pGraphicDev, tInfo);
	pFireparent->SetDir(vecDir);
	pFireparent->SetMaterial(BLACK);
	pFireparent->SetIsKing();
	m_listFireparent.emplace_back(pFireparent);
}

CStage3* CStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage3*		pInstance = new CStage3(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}