#include "stdafx.h"
#include "ColorCircle.h"
#include "Include.h"
#include "CameraObserver.h"
#include "Export_Function.h"
#include "CubeMgr.h"
#include "Mouse.h"
#include "Hand.h"
#include "Snow.h"
#include "Rain.h"
#include "Outline.h"
#include "ColorHP.h"
USING(Engine)

CColorCircle::CColorCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfoSubject(Engine::Get_InfoSubject())
{

}

CColorCircle::~CColorCircle()
{
	Release();
}

HRESULT CColorCircle::Initialize(CGameObject* pPlayer)
{
	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_bForSound = true;
	m_pCameraObserver = CCameraObserver::GetInstance();
	m_pOutline = COutline::Create(m_pGraphicDev);

	m_eCurBufferColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
	m_bCanSelectColor = true;
	m_pMouse = CMouse::Create(m_pGraphicDev);
	//m_pHand = CHand::Create(m_pGraphicDev);

	if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
		m_pPlayerTuto = dynamic_cast<CPlayerTuto*>(pPlayer);
	else
		m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);

	m_pBufferName = L"";
	m_pTexName = L"";

	/************************ (SongSong) ************************/
	/*  눈 파티클 생성  */
	m_pSnow = CSnow::Create(m_pGraphicDev);

	m_pRain = CRain::Create(m_pGraphicDev);

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	CreateColorPart();
	m_pInsideCircle = CInsideCircle::Create(m_pGraphicDev);
	if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
		m_pInsideCircle->SetPlayer(m_pPlayerTuto);
	else
		m_pInsideCircle->SetPlayer(m_pPlayer);
	m_pInsideCircle->SetSnow(m_pSnow);
	m_pInsideCircle->SetRain(m_pRain);
	m_pColorHP = CColorHP::Create(m_pGraphicDev);

	Engine::Get_InfoSubject()->AddData(DATA_COLORCIRCLE, this);
	Engine::Get_InfoSubject()->AddData(DATA_COLORCIRCLEACTIVE, &m_bActive);

	return S_OK;
}

void CColorCircle::CreateColorPart(void)
{
	CColorPart*      pColorPart = nullptr;

	for (int i = 0; i < COLORPARTID::PART_END; ++i)
	{
		pColorPart = CColorPart::Create(m_pGraphicDev, (COLORPARTID)i);
		NULL_CHECK_MSG(pColorPart, L"Create ColorPart Failed");
		pColorPart->SetParent(this);
		m_pColorPartArr[i] = pColorPart;
	}
}


HRESULT CColorCircle::AddComponent(void)
{
	Engine::CComponent*      pComponent = nullptr;

	return S_OK;
}


void CColorCircle::KeyInput(void)
{
	Engine::CUBE_COLOR eBuffer = m_eCurBufferColor;
	Engine::CUBE_COLOR ePlayerCol = CCubeMgr::GetInstance()->GetPlayerColColor();

	if (CCubeMgr::GetInstance()->GetPlayerColColor() == m_eCurBufferColor
		&& ePlayerCol != Engine::CUBE_COLOR::CUBE_COLOR_END)
	{
		m_bCanSelectColor = false;
	}
	else
	{
		m_bCanSelectColor = true;
	}
	m_bActive = false;

	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_RBUTTON) && m_bIsReady &&
		CCubeMgr::GetInstance()->Get_IsRender() != true)
	{
		if (m_bForSound)
		{
			Engine::Get_SoundMgr()->MyPlaySound(L"ExpandColor.wav", Engine::CHANNELID::UI);
			m_bForSound = false;
		}
		Engine::Get_TimeMgr()->SetDivideTime(10.f);
		for (int i = 0; i < COLORPARTID::PART_END; ++i)
		{
			m_pColorPartArr[i]->BiggerUpdate();
			m_pInsideCircle->BiggerUpdate();
		}
		m_bActive = true;
		if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
			m_pPlayerTuto->SetPlayerInput(true);
		else
			m_pPlayer->SetPlayerInput(true);
		//ShowCursor(TRUE);
	}
	else
	{
		Engine::Get_TimeMgr()->SetDivideTime(1.f);
		for (int i = 0; i < COLORPARTID::PART_END; ++i)
		{
			m_pColorPartArr[i]->SmallerUpdate();
			m_pInsideCircle->SmallerUpdate();
		}
		if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
			m_pPlayerTuto->SetPlayerInput(false);
		else
			m_pPlayer->SetPlayerInput(false);
	}
	if (Engine::Get_KeyMgr()->KeyUp(Engine::KEY_RBUTTON) && m_eSelectID != COLORPARTID::PART_END)
	{
		/************************ (SongSong) ************************/
		/*  w  */
		if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		{
			SetCursorPos(WINCX / 2, WINCY / 2);
			//ShowCursor(FALSE);
			m_pCameraObserver->GetPlayer()->Set_CenterPointInit();
		}

		m_pColorPartArr[(int)m_eSelectID]->InitFirstPos();
		m_pColorPartArr[(int)m_eSelectID]->SelectColorPart(false);


		Engine::CUBE_COLOR eColor;
		switch (m_eSelectID)
		{
		case PART_RUU:
			eColor = Engine::CUBE_COLOR::CUBE_YELLOW;
			break;
		case PART_RUD:
			eColor = Engine::CUBE_COLOR::CUBE_GREEN;
			break;
		case PART_RDU:
			eColor = Engine::CUBE_COLOR::CUBE_PURPLE;
			break;
		case PART_RDD:
			eColor = Engine::CUBE_COLOR::CUBE_PINK;
			break;
		case PART_LUU:
			eColor = Engine::CUBE_COLOR::CUBE_ORANGE;
			break;
		case PART_LUD:
			eColor = Engine::CUBE_COLOR::CUBE_RED;
			break;
		case PART_LDU:
			eColor = Engine::CUBE_COLOR::CUBE_BLUE;
			break;
		case PART_LDD:
			eColor = Engine::CUBE_COLOR::CUBE_SKYBLUE;
			break;
		case PART_END:
			break;
		default:
			break;
		}

		Engine::Get_SoundMgr()->MyPlaySound(L"ContractColor.wav", Engine::CHANNELID::UI);
		m_bForSound = true;

		Engine::CUBE_COLOR eBuffer = m_eCurBufferColor;
		Engine::CUBE_COLOR ePlayerCol = CCubeMgr::GetInstance()->GetPlayerColColor();

		if (CCubeMgr::GetInstance()->GetPlayerColColor() == m_eCurBufferColor
			&& ePlayerCol != Engine::CUBE_COLOR::CUBE_COLOR_END)
		{
			Engine::CUBE_COLOR eBuffer = m_eCurBufferColor;
			Engine::CUBE_COLOR ePlayerCol = CCubeMgr::GetInstance()->GetPlayerColColor();
			return;
		}

		Engine::Get_Management()->SetBackColor(m_pColorPartArr[(int)m_eSelectID]->GetColor());
		CCubeMgr::GetInstance()->SetBackColor(m_pColorPartArr[(int)m_eSelectID]->GetColor());
		CCubeMgr::GetInstance()->Set_CurCubeColor(eColor);
		m_eCurBufferColor = eColor;
		m_eSelectID = COLORPARTID::PART_END;
		m_pColorHP->MinusCount();

	}


	if (m_bIsSizeMax)
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		pt.x -= (WINCX / 2);
		pt.y -= (WINCY / 2);

		D3DXVECTOR2 vecMouse;
		vecMouse.x = (float)pt.x;
		vecMouse.y = (float)pt.y;
		D3DXVec2Normalize(&vecMouse, &vecMouse);

		float fDot = D3DXVec2Dot(&vecMouse, &D3DXVECTOR2(1.f, 0.f));
		float fTheta = acosf(fDot);
		float fAngle = D3DXToDegree(fTheta);

		if (pt.y > 0)
			fAngle = 360.f - fAngle;

		if (fAngle > 315.f)
			m_eSelectID = COLORPARTID::PART_RDU;
		else if (fAngle > 270.f)
			m_eSelectID = COLORPARTID::PART_RDD;
		else if (fAngle > 225.f)
			m_eSelectID = COLORPARTID::PART_LDD;
		else if (fAngle > 180.f)
			m_eSelectID = COLORPARTID::PART_LDU;
		else if (fAngle > 135.f)
			m_eSelectID = COLORPARTID::PART_LUD;
		else if (fAngle > 90.f)
			m_eSelectID = COLORPARTID::PART_LUU;
		else if (fAngle > 45.f)
			m_eSelectID = COLORPARTID::PART_RUU;
		else if (fAngle > 0.f)
			m_eSelectID = COLORPARTID::PART_RUD;

		for (int i = 0; i < COLORPARTID::PART_END; ++i)
		{
			if (i == m_eSelectID)
				m_pColorPartArr[(COLORPARTID)i]->SelectColorPart(true);
			else
				m_pColorPartArr[(COLORPARTID)i]->SelectColorPart(false);
		}
	}


}

void CColorCircle::Update(void)
{
	if (CCubeMgr::GetInstance()->m_bGetBattery)
	{
		m_pColorHP->PlusCount();
		CCubeMgr::GetInstance()->m_bGetBattery = false;
	}

	KeyInput();

	if (m_bActive)
		m_pMouse->Update();

	if (SCENEID::SC_STAGE == *m_pCameraObserver->GetSceneID()
		|| SCENEID::SC_STAGE3 == *m_pCameraObserver->GetSceneID())
	{
		m_pSnow->Update();
	}
	else if (SCENEID::SC_STAGE2 == *m_pCameraObserver->GetSceneID())
		m_pRain->Update();
	//m_pHand->Update();
	m_pOutline->Update();
	m_pInfoSubject->Notify(DATA_COLORCIRCLEACTIVE);
	m_pColorHP->Update();
}

void CColorCircle::LateUpdate(void)
{
	if (m_bActive)
		m_pMouse->LateUpdate();

	if (SCENEID::SC_STAGE == *m_pCameraObserver->GetSceneID()
		|| SCENEID::SC_STAGE3 == *m_pCameraObserver->GetSceneID())
	{
		m_pSnow->LateUpdate();
	}
	else if (SCENEID::SC_STAGE2 == *m_pCameraObserver->GetSceneID())
		m_pRain->LateUpdate();
	//m_pHand->LateUpdate();
	m_pOutline->LateUpdate();
	m_pColorHP->LateUpdate();
}

void CColorCircle::Render(void)
{
	/************************ (SongSong) ************************/
	/*  여기서부터 원근투영(바깥세상)에 그려준다  */
	if (SCENEID::SC_STAGE == *m_pCameraObserver->GetSceneID()
		|| SCENEID::SC_STAGE3 == *m_pCameraObserver->GetSceneID())
	{
		m_pSnow->Render();
	}
	else if (SCENEID::SC_STAGE2 == *m_pCameraObserver->GetSceneID())
		m_pRain->Render();

	/************************ (SongSong) ************************/
	/*  여기서부터 직교투영이다  */
	D3DXMATRIX    mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	D3DXMATRIX      matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pHand->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 0.f, 10.f);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	if (SCENEID::SC_TUTORIAL != *m_pCameraObserver->GetSceneID())
		m_pColorHP->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	m_pInsideCircle->Render();

	//m_pSnow->Render();

	/************************ (SongSong) ************************/
	/*  여기까지 직교투영이다  */
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mIdentity);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_bCanSelectColor)
	{
		for (int i = 0; i < COLORPARTID::PART_END; ++i)
			m_pColorPartArr[i]->Render();
	}

	/************************ (SongSong) ************************/
	/*  여기서부터 또 직교  */
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 0.f, 10.f);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	if (m_bActive && *CCameraObserver::GetInstance()->GetNarrativeView() == NARRATIVE_VIEW::FIRST_PERSON)
		m_pMouse->Render_FirstMouse();
	else if (m_bActive && *CCameraObserver::GetInstance()->GetNarrativeView() != NARRATIVE_VIEW::FIRST_PERSON)
		m_pMouse->Render();
	else
		m_pMouse->FirstPersonRender();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);

	m_pOutline->Render();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


}

void CColorCircle::Release(void)
{
	Engine::Safe_Delete(m_pColorHP);
	Engine::Safe_Delete(m_pOutline);
	Engine::Safe_Delete(m_pSnow);
	Engine::Safe_Delete(m_pRain);
	//Engine::Safe_Delete(m_pHand);
	Engine::Safe_Delete(m_pMouse);

	for (int i = 0; i < COLORPARTID::PART_END; ++i)
		Engine::Safe_Delete(m_pColorPartArr[i]);

	Engine::Safe_Delete(m_pInsideCircle);

}

CColorCircle* CColorCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer)
{
	CColorCircle*   pInstance = new CColorCircle(pGraphicDev);

	if (FAILED(pInstance->Initialize(pPlayer)))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}