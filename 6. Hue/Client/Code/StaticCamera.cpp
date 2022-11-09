#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"
#include "CameraObserver.h"
#include "Include.h"
#include "Transform.h"
#include "KeyMgr.h"
#include "Player.h"
#include "Door.h"
#include "Key.h"
#include "PlayerTuto.h"
#include "SceneSelector.h"

USING(Engine)

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
	, m_pInfoSubject(Engine::Get_InfoSubject())
	, m_pKeyMgr(Engine::Get_KeyMgr())
	, m_pCameraObserver(CCameraObserver::GetInstance())
{

}

CStaticCamera::~CStaticCamera(void)
{
}

HRESULT CStaticCamera::Initialize(void)
{
	/************************ (SongSong) ************************/
	/*  씬체크를 위해 */
	static int iSceneNum = 0;
	m_iScene += iSceneNum;
	++iSceneNum;

	//if (SCENEID::SC_TUTORIAL_THREAD == m_iScene)
	//   ++iSceneNum;

	Engine::Get_InfoSubject()->AddData(D3DTS_VIEW, &m_matView);
	Engine::Get_InfoSubject()->AddData(D3DTS_PROJECTION, &m_matProj);
	Engine::Get_InfoSubject()->AddData(DATA_NARRATIVE_MOVEING, &m_bNarrativeMoveing);
	Engine::Get_InfoSubject()->AddData(DATA_NARRATIVE_VIEW, &m_eNarrativeView);
	Engine::Get_InfoSubject()->AddData(DATA_STATICCAMERA, this);
	Engine::Get_InfoSubject()->AddData(DATA_SCENEID, &m_iScene);

	/************************ (SongSong) ************************/
	/*  로고아닐 때 카메라디폴트 초기화  */
	if (SCENEID::SC_LOGO != m_iScene)
		BasicCameraDefaultInit();

	/************************ (SongSong) ************************/
	/*  스테이지 추가하셈  */
	if (SCENEID::SC_STAGE == m_iScene
		|| SCENEID::SC_STAGE2 == m_iScene)
		m_bDoorGofor = true;

	return S_OK;
}

void CStaticCamera::Update(void)
{
	//if (SCENEID::SC_LOGO == m_iScene)
	KeyInput();

	if (SCENEID::SC_TUTORIAL_THREAD == m_iScene)
	{
		//if (0.7f > GET_TIME)
		//{
		//   if (false == m_bUpDown)
		//   {
		//      m_fPowerY -= 0.4f * GET_TIME;
		//      m_vEye.y -= m_fPowerY * GET_TIME;
		//      m_vAt.y -= m_fPowerY * GET_TIME;
		//   }
		//   else
		//   {
		//      m_fPowerY -= 0.4f * GET_TIME;
		//      m_vEye.y += m_fPowerY * GET_TIME;
		//      m_vAt.y += m_fPowerY * GET_TIME;
		//   }
		//}

		//if (true == m_bUpDown && 0.f >= m_fPowerY)
		//{
		//   m_bUpDown = false;
		//   m_fPowerY = 0.71f;
		//}
		//else if (false == m_bUpDown && 0.f >= m_fPowerY)
		//{
		//   m_bUpDown = true;
		//   m_fPowerY = 0.71f;
		//}

		//m_vEye.x = 0.f;
		//m_vAt.x = 0.f;

		//D3DXVECTOR3 vDir = m_vAt - m_vEye;
		//D3DXVec3Normalize(&vDir, &vDir);

		//if (PLAYERSTATE::STATE_FALL != m_pCameraObserver->GetPlayerTuto()->Get_CurState()
		//   && PLAYERSTATE::STATE_JUMP != m_pCameraObserver->GetPlayerTuto()->Get_CurState())
		//{
		//   //m_vEye.y = m_pTargetInfo->m_vPos.y;
		//}

		//CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));

		if (m_pTargetInfo)
		{
			D3DXVECTOR3 vDir = m_pTargetInfo->m_vPos - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			//m_vEye.x += vDir.x * 18.f * GET_TIME;
			//m_vEye.y += vDir.y * 12.f  * GET_TIME;

			m_vEye.x += vDir.x * 11.f * GET_TIME;
			m_vEye.y += vDir.y * 5.f  * GET_TIME;

			m_vAt = m_pTargetInfo->m_vPos;

			//m_vEye.x = m_pTargetInfo->m_vPos.x;
			//m_vEye.y = m_pTargetInfo->m_vPos.y;

			CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		}
	}
	if (/*SCENEID::SC_TUTORIAL_THREAD == m_iScene
		|| */SCENEID::SC_TUTORIAL == m_iScene)
		TutoTargetRenewal();

	if (SCENEID::SC_TUTORIAL_THREAD != m_iScene
		&& SCENEID::SC_TUTORIAL != m_iScene)
	{
		if (0.7f > GET_TIME)
			StageDoorGofor();
		NarrativeMoveing();
		if (SCENEID::SC_STAGE == m_iScene
			|| SCENEID::SC_STAGE2 == m_iScene)
		{
			if (FALSE == m_bStageWaveMoving)
				StagePlayerWaveCheck();
			else if (TRUE == m_bStageWaveMoving)
				CameraStageWaveMoving();
		}
		TargetRenewal();
	}

	/************************ (SongSong) ************************/
	/*  키가 생성되있는 스테이지만 적용하셈 (뻑나는 구간이니까 조심!)  */
	if (SCENEID::SC_STAGE == m_iScene
		|| SCENEID::SC_STAGE2 == m_iScene
		|| SCENEID::SC_STAGE3 == m_iScene)
	{
		m_pInfoSubject->Notify(DATA_KEY);
		StageAddKeyUpdate();
	}

	m_pInfoSubject->Notify(D3DTS_VIEW);
	m_pInfoSubject->Notify(D3DTS_PROJECTION);
	m_pInfoSubject->Notify(DATA_NARRATIVE_MOVEING);
	m_pInfoSubject->Notify(DATA_NARRATIVE_VIEW);
	m_pInfoSubject->Notify(DATA_SCENEID);
}

void CStaticCamera::LateUpdate(void)
{
}

void CStaticCamera::KeyInput(void)
{
	bool b = m_bNarrativeMoveing;

	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//	m_fAngle += D3DXToRadian(45.f) * GET_TIME;

	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//	m_fAngle -= D3DXToRadian(45.f) * GET_TIME;

	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	m_fDistance -= m_fSpeed * GET_TIME;

	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	m_fDistance += m_fSpeed * GET_TIME;


	if (m_pPlayer)
	{
		if (m_pKeyMgr->KeyDown(KEY_SHIFT) && false == m_bNarrativeMoveing && PLAYERSTATE::STATE_JUMP != m_pPlayer->Get_CurState()
			&& false == m_bDoorGofor)
		{
			m_bNarrativeMoveing = true;
			m_eNarrativeView = m_eNarrativeView == NARRATIVE_VIEW::FIRST_PERSON ? NARRATIVE_VIEW::DEFAULT : NARRATIVE_VIEW::FIRST_PERSON;

			/************************ (SongSong) ************************/
			/*  카메라 이동값 초기화  */
			m_bCubPointMove = false;
			m_vCubPointPower[0] = { 0.f, 0.f, 1.f };
			m_vCubPointPower[1] = { 20.f, 0.f, 25.f };

			if (NARRATIVE_VIEW::FIRST_PERSON == m_eNarrativeView)
			{
				m_vEye.x = m_pTargetInfo->m_vPos.x;
				m_vEye.y = m_pTargetInfo->m_vPos.y;
				SetCursorPos(WINCX / 2, WINCY / 2);
				Engine::Get_SoundMgr()->MyPlaySound(L"CameraZoomIn.wav", Engine::CHANNELID::CAMERA);
			}
			else
				Engine::Get_SoundMgr()->MyPlaySound(L"CameraZoomOut.wav", Engine::CHANNELID::CAMERA);

			m_pPlayer->Set_NextStateWalk();
		}
	}
	else
	{
		if (SCENEID::SC_LOGO == m_iScene)
			UpdateLogoCamera();
	}
}

void CStaticCamera::TargetRenewal(void)
{
	if (m_pTargetInfo && FALSE == m_bNarrativeMoveing && false == m_bDoorGofor && FALSE == m_pCameraObserver->GetKey()->GetIsGetKey()
		&& false == m_bStageWaveMoving)
	{
		D3DXVECTOR3 vDir;

		vDir = m_pTargetInfo->m_vPos - m_vEye;
		D3DXVec3Normalize(&vDir, &vDir);

		switch (m_eNarrativeView)
		{
		case NARRATIVE_VIEW::DEFAULT:
		{
			switch (m_eCameraZoomType)
			{
			case CAMERA_ZOOM_TYPE::ZOOM_VERY_NEAR:

				break;
			case CAMERA_ZOOM_TYPE::ZOOM_NEAR:
				m_vEye.x += vDir.x * 0.4f * GET_TIME;
				m_vEye.y += vDir.y * 0.6f  * GET_TIME;
				m_vEye.z = -7.f;
				m_vAt = m_pTargetInfo->m_vPos;
				break;
			case CAMERA_ZOOM_TYPE::ZOOM_DEFAULT:
				m_vEye.x += vDir.x * 0.5f * GET_TIME;
				m_vEye.y += vDir.y * 0.7f  * GET_TIME;
				m_vEye.z = -10.f;
				m_vAt = m_pTargetInfo->m_vPos;
				break;
			case CAMERA_ZOOM_TYPE::ZOOM_FAR:
				m_vEye.x += vDir.x * 0.4f * GET_TIME;
				m_vEye.y += vDir.y * 0.6f  * GET_TIME;
				m_vEye.z = -13.f;
				m_vAt = m_pTargetInfo->m_vPos;
				break;
			case CAMERA_ZOOM_TYPE::ZOOM_VERY_FAR:
				m_vAt.x = m_vEye.x = 58.9f;
				m_vAt.y = m_vEye.y = 21.8f;
				m_vEye.z = m_fZoomZ;
				//m_vAt = { 58.9f, 21.f, 0.f };
				break;
			}

			CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		}
		break;
		case NARRATIVE_VIEW::FIRST_PERSON:
		{
			if (!g_bFirstNar)
				g_bFirstNar = true;
			m_vEye = m_pTargetInfo->m_vDir * (-0.5f);
			D3DXVec3Normalize(&m_vEye, &m_vEye);

			//m_vEye *= m_fDistance;

			D3DXVECTOR3   vRight;
			memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);//

			m_fAngle = 0.f;

			D3DXMATRIX   matRotAxis;//
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);//
			D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);//

			m_vEye = m_pTargetInfo->m_vPos;
			m_vEye.y += 1.f;

			m_vAt = m_pTargetInfo->m_vPos;
			m_vAt += m_pTargetInfo->m_vDir;
			m_vAt.y += 1.f;


			if (TRUE == m_pPlayer->Get_ForCameraFirstDir())
			{
				m_vEye.x += m_pTargetInfo->m_vDir.x / m_pTargetInfo->m_vDir.x / 1.15f;
				m_vAt.x += m_pTargetInfo->m_vDir.x / m_pTargetInfo->m_vDir.x / 1.15f;
			}
			else
			{
				m_vEye.x -= m_pTargetInfo->m_vDir.x / m_pTargetInfo->m_vDir.x / 1.15f;
				m_vAt.x -= m_pTargetInfo->m_vDir.x / m_pTargetInfo->m_vDir.x / 1.15f;
			}

			CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		}
		break;
		}
	}
	else if (nullptr == m_pTargetInfo)
	{
		CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
	}

}

void CStaticCamera::TutoTargetRenewal(void)
{
	if (m_pTargetInfo)
	{
		if (37.f > m_pTargetInfo->m_vPos.x)
		{
			D3DXVECTOR3 vDir = m_pTargetInfo->m_vPos - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			//m_vEye.x += vDir.x * 18.f * GET_TIME;
			//m_vEye.y += vDir.y * 12.f  * GET_TIME;

			m_vEye.x += vDir.x * 21.f * GET_TIME;
			m_vEye.y += vDir.y * 15.f  * GET_TIME;

			m_vAt = m_pTargetInfo->m_vPos;

			//m_vEye.x = m_pTargetInfo->m_vPos.x;
			//m_vEye.y = m_pTargetInfo->m_vPos.y;

			CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		}
		else
		{
			D3DXVECTOR3 vDir = m_vAt - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vEye.x += vDir.x * 10.f * GET_TIME;
			m_vEye.y += vDir.y * 3.f  * GET_TIME;

			if (-13.25f < m_vEye.z)
				m_vEye.z -= 1.9f * GET_TIME;
			else
				m_vEye.z = -13.25f;

			CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
		}
	}
}

void CStaticCamera::NarrativeMoveing(void)
{
	if (nullptr == m_pTargetInfo || FALSE == m_bNarrativeMoveing)
		return;


	switch (m_eNarrativeView)
	{
	case NARRATIVE_VIEW::DEFAULT:
		/* 플레이어가 왼쪽 바라볼때 */
		if (TRUE == m_pPlayer->Get_ForCameraFirstDir())
			CameraDefaultDir(0);
		/* 플레이어가 오른쪽 바라볼때 */
		else if (FALSE == m_pPlayer->Get_ForCameraFirstDir())
			CameraDefaultDir(1);

		break;
	case NARRATIVE_VIEW::FIRST_PERSON:
		/* 플레이어가 왼쪽 바라볼때 */
		if (TRUE == m_pPlayer->Get_ForCameraFirstDir())
			CameraFirstPersonDir(0);
		/* 플레이어가 오른쪽 바라볼때 */
		else if (FALSE == m_pPlayer->Get_ForCameraFirstDir())
			CameraFirstPersonDir(1);

		break;
	}
	CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
}

void CStaticCamera::CameraDefaultDir(BYTE _byDir)
{
	bool bZMove = false;
	bool bXMove = false;

	m_bCubPointMove = true;
	m_vAt.x = m_pTargetInfo->m_vPos.x;

	/************************ (SongSong) ************************/
	/*  각 시점에 따른 Y값 변화떄문에 시점이동 하면서 자연스러운 이동  */
	if (m_pTargetInfo->m_vPos.y < m_vEye.y)
		m_vEye.y -= 0.5f * GET_TIME;
	else
		m_vEye.y = m_pTargetInfo->m_vPos.y;

	if (m_pTargetInfo->m_vPos.y < m_vAt.y)
		m_vAt.y -= 0.5f * GET_TIME;
	else
		m_vAt.y = m_pTargetInfo->m_vPos.y;

	switch (_byDir)
	{
		/************************ (SongSong) ************************/
		/*  플레이어 왼쪽방향  */
	case 0:
		// 커브포인트0 행동
		if (true == m_bCubPointMove)
		{
			/************************ (SongSong) ************************/
			/*  줌아웃  */
			// X 이동
			if (0.f < m_vCubPointPower[1].x)
			{
				m_vEye.x += m_vCubPointPower[1].x * GET_TIME;
				m_vCubPointPower[1].x -= 13.f * GET_TIME;
			}
			else
				bXMove = true;
			// Z 이동
			if (0.f < m_vCubPointPower[1].z)
			{
				if (m_pTargetInfo->m_vPos.x + 2.f < m_vEye.x)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 15.f * GET_TIME;
				}
			}
			else
				bZMove = true;

			/************************ (SongSong) ************************/
			/*  줌인  */
			if (true == bXMove)
			{
				if (m_pTargetInfo->m_vPos.x < m_vEye.x)
				{
					m_vEye.x += m_vCubPointPower[1].x * GET_TIME;
					m_vCubPointPower[1].x -= 30.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.x >= m_vEye.x)
						m_vEye.x = m_pTargetInfo->m_vPos.x;
				}
			}
			if (true == bZMove)
			{
				if (-10.f > m_vEye.z)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 30.f * GET_TIME;

					if (-10.f <= m_vEye.z)
					{
						m_vEye.z = -10.f;
					}
				}
			}
		}
		break;
		/************************ (SongSong) ************************/
		/*  플레이어 오른쪽방향  */
	case 1:
		// 커브포인트0 행동
		if (true == m_bCubPointMove)
		{
			/************************ (SongSong) ************************/
			/*  줌아웃  */
			// X 이동
			if (0.f < m_vCubPointPower[1].x)
			{
				m_vEye.x -= m_vCubPointPower[1].x * GET_TIME;
				m_vCubPointPower[1].x -= 13.f * GET_TIME;
			}
			else
				bXMove = true;
			// Z 이동
			if (0.f < m_vCubPointPower[1].z)
			{
				if (m_pTargetInfo->m_vPos.x - 2.f > m_vEye.x)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 15.f * GET_TIME;
				}
			}
			else
				bZMove = true;

			/************************ (SongSong) ************************/
			/*  줌인  */
			if (true == bXMove)
			{
				if (m_pTargetInfo->m_vPos.x > m_vEye.x)
				{
					m_vEye.x -= m_vCubPointPower[1].x * GET_TIME;
					m_vCubPointPower[1].x -= 30.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.x <= m_vEye.x)
						m_vEye.x = m_pTargetInfo->m_vPos.x;
				}
			}
			if (true == bZMove)
			{
				if (-10.f > m_vEye.z)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 30.f * GET_TIME;

					if (-10.f <= m_vEye.z)
					{
						m_vEye.z = -10.f;
					}
				}
			}
		}
		break;
	}
	/************************ (SongSong) ************************/
	/*  도착시 무빙 종료  */
	if (bXMove && bZMove)
	{
		if (-10.f == m_vEye.z && m_pTargetInfo->m_vPos.x == m_vEye.x)
			m_bNarrativeMoveing = false;
	}
}

void CStaticCamera::CameraFirstPersonDir(BYTE _byDir)
{
	bool bZMove = false;
	bool bXMove = false;

	/************************ (SongSong) ************************/
	/*  각 시점에 따른 Y값 변화떄문에 시점이동 하면서 자연스러운 이동  */
	if (m_pTargetInfo->m_vPos.y + 1.f > m_vEye.y)
		m_vEye.y += 0.5f * GET_TIME;
	else
		m_vEye.y = m_pTargetInfo->m_vPos.y + 1.f;

	if (m_pTargetInfo->m_vPos.y + 1.f > m_vAt.y)
		m_vAt.y += 0.5f * GET_TIME;
	else
		m_vAt.y = m_pTargetInfo->m_vPos.y + 1.f;

	// 커브포인트0 행동 (플레이어 줌인)
	if (false == m_bCubPointMove)
	{
		// Z 이동
		if (m_pTargetInfo->m_vPos.z - 2.f > m_vEye.z)
		{
			m_vEye.z += m_vCubPointPower[0].z * GET_TIME;
			m_vCubPointPower[0].z += 15.f * GET_TIME;

			if (m_pTargetInfo->m_vPos.z - 2.f <= m_vEye.z)
			{
				m_vEye.z = m_pTargetInfo->m_vPos.z - 2.f;
				m_bCubPointMove = true;
			}
		}
	}

	switch (_byDir)
	{
		/************************ (SongSong) ************************/
		/*  플레이어 왼쪽방향  */
	case 0:
		// 커브포인트1 행동
		if (true == m_bCubPointMove)
		{
			/************************ (SongSong) ************************/
			/*  줌아웃  */
			// X 이동
			if (0.f < m_vCubPointPower[1].x && -6.f > m_vEye.z)
			{
				m_vEye.x += m_vCubPointPower[1].x * GET_TIME;
				m_vCubPointPower[1].x -= 12.f * GET_TIME;
			}
			else
				bXMove = true;
			// Z 이동
			if (0.f < m_vCubPointPower[1].z)
			{
				m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
				m_vCubPointPower[1].z -= 21.f * GET_TIME;
			}
			else
				bZMove = true;

			/************************ (SongSong) ************************/
			/*  줌인  */
			if (true == bXMove)
			{
				if (m_pTargetInfo->m_vPos.x < m_vEye.x)
				{
					m_vEye.x += m_vCubPointPower[1].x * GET_TIME;
					m_vCubPointPower[1].x -= 48.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.x >= m_vEye.x)
						m_vEye.x = m_pTargetInfo->m_vPos.x;
				}
			}
			if (true == bZMove)
			{
				if (m_pTargetInfo->m_vPos.x > m_vEye.z)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 16.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.z <= m_vEye.z)
					{
						m_vEye.z = m_pTargetInfo->m_vPos.z;
						m_vAt.x -= 3.f;
					}
				}
			}
		}
		break;
		/************************ (SongSong) ************************/
		/*  플레이어 오른쪽방향  */
	case 1:
		// 커브포인트1 행동
		if (true == m_bCubPointMove)
		{
			/************************ (SongSong) ************************/
			/*  줌아웃  */
			// X 이동
			if (0.f < m_vCubPointPower[1].x && -6.f > m_vEye.z)
			{
				m_vEye.x -= m_vCubPointPower[1].x * GET_TIME;
				m_vCubPointPower[1].x -= 12.f * GET_TIME;
			}
			else
				bXMove = true;
			// Z 이동
			if (0.f < m_vCubPointPower[1].z)
			{
				m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
				m_vCubPointPower[1].z -= 21.f * GET_TIME;
			}
			else
				bZMove = true;

			/************************ (SongSong) ************************/
			/*  줌인  */
			if (true == bXMove)
			{
				if (m_pTargetInfo->m_vPos.x > m_vEye.x)
				{
					m_vEye.x -= m_vCubPointPower[1].x * GET_TIME;
					m_vCubPointPower[1].x -= 48.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.x <= m_vEye.x)
						m_vEye.x = m_pTargetInfo->m_vPos.x;
				}
			}
			if (true == bZMove)
			{
				if (m_pTargetInfo->m_vPos.x > m_vEye.z)
				{
					m_vEye.z -= m_vCubPointPower[1].z * GET_TIME;
					m_vCubPointPower[1].z -= 16.f * GET_TIME;

					if (m_pTargetInfo->m_vPos.z <= m_vEye.z)
					{
						m_vEye.z = m_pTargetInfo->m_vPos.z;
						m_vAt.x += 3.f;
					}
				}
			}
		}
		break;
	}
	/************************ (SongSong) ************************/
	/*  도착시 무빙 종료  */
	if (bXMove && bZMove)
	{
		if (m_pTargetInfo->m_vPos.z == m_vEye.z && m_pTargetInfo->m_vPos.x == m_vEye.x)
			m_bNarrativeMoveing = false;
	}
}

void CStaticCamera::StagePlayerWaveCheck()
{
	if (m_pTargetInfo)
	{
		switch (m_iScene)
		{
			/************************ (SongSong) ************************/
			/*  스테이지 1  */
		case SCENEID::SC_STAGE:

			/************************ (SongSong) ************************/
			/*  체크포인트3 줌인 해제  */
			if (3 == CCubeMgr::GetInstance()->GetSaveCount()
				&& TRUE == CCubeMgr::GetInstance()->Get_IsRender()
				/*|| 4 == m_iWave
				&& TRUE == CCubeMgr::GetInstance()->Get_IsRender()*/)
			{
				m_iWave = 3;
				m_bStageWaveMoving = false;
				m_bStageWavePoint = false;
				m_bStage1MovingPoint[2] = false;
				m_bStage1MovingPoint[3] = false;

				if (-10.f < m_vEye.z)
					m_vEye.z -= 2.f * GET_TIME;
				else
				{
					m_vEye.z = -10.f;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
				}
			}

			// 체크포인트 1
			if (false == m_bStage1MovingPoint[0] && 1 == m_iWave)
			{
				if (13.5f >= m_pTargetInfo->m_vPos.x && 12.5f <= m_pTargetInfo->m_vPos.x &&
					13.f >= m_pTargetInfo->m_vPos.y && 10.f <= m_pTargetInfo->m_vPos.y)
				{
					m_bStage1MovingPoint[0] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 2
			if (false == m_bStage1MovingPoint[1] && 2 == m_iWave)
			{
				if (25.f >= m_pTargetInfo->m_vPos.x && 23.f <= m_pTargetInfo->m_vPos.x &&
					28.7f >= m_pTargetInfo->m_vPos.y && 25.7f <= m_pTargetInfo->m_vPos.y)
				{
					m_bStage1MovingPoint[1] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 3
			if (false == m_bStage1MovingPoint[2] && 3 == m_iWave)
			{
				if (38.2f >= m_pTargetInfo->m_vPos.x && 36.2f <= m_pTargetInfo->m_vPos.x &&
					10.5f >= m_pTargetInfo->m_vPos.y && 7.5f <= m_pTargetInfo->m_vPos.y)
				{
					m_bStage1MovingPoint[2] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 4
			if (false == m_bStage1MovingPoint[3] && 4 == m_iWave)
			{
				if (38.f >= m_pTargetInfo->m_vPos.x && 36.f <= m_pTargetInfo->m_vPos.x &&
					20.5f >= m_pTargetInfo->m_vPos.y && 17.5f <= m_pTargetInfo->m_vPos.y)
				{
					m_bStage1MovingPoint[3] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			break;
			/************************ (SongSong) ************************/
			/*  스테이지 2  */
		case SCENEID::SC_STAGE2:

			/************************ (SongSong) ************************/
			/*  체크포인트3 줌인 해제  */
			if (3 == CCubeMgr::GetInstance()->GetSaveCount()
				&& TRUE == CCubeMgr::GetInstance()->Get_IsRender()
				/*|| 4 == m_iWave
				&& TRUE == CCubeMgr::GetInstance()->Get_IsRender()*/)
			{
				m_iWave = 3;
				m_bStageWaveMoving = false;
				m_bStageWavePoint = false;
				m_bStage2MovingPoint[2] = false;
				m_bStage2MovingPoint[3] = false;

				if (-10.f < m_vEye.z)
					m_vEye.z -= 0.5f * GET_TIME;
				else
				{
					m_vEye.z = -10.f;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
				}
			}

			// 체크포인트 1
			if (false == m_bStage2MovingPoint[0] && 1 == m_iWave)
			{
				if (12.f <= m_pTargetInfo->m_vPos.x && 14.f >= m_pTargetInfo->m_vPos.x &&
					5.5f <= m_pTargetInfo->m_vPos.y && 8.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[0] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 2
			if (false == m_bStage2MovingPoint[1] && 2 == m_iWave)
			{
				if (25.f <= m_pTargetInfo->m_vPos.x && 27.f >= m_pTargetInfo->m_vPos.x &&
					32.5f <= m_pTargetInfo->m_vPos.y && 35.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[1] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
					m_fMovingZ = 17.f;
				}
			}
			// 체크포인트 3
			if (false == m_bStage2MovingPoint[2] && 3 == m_iWave)
			{
				if (38.f <= m_pTargetInfo->m_vPos.x && 40.f >= m_pTargetInfo->m_vPos.x &&
					10.5f <= m_pTargetInfo->m_vPos.y && 13.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[2] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 4
			if (false == m_bStage2MovingPoint[3] && 4 == m_iWave)
			{
				if (47.f <= m_pTargetInfo->m_vPos.x && 49.f >= m_pTargetInfo->m_vPos.x &&
					5.5f <= m_pTargetInfo->m_vPos.y && 8.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[3] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 5
			if (false == m_bStage2MovingPoint[4] && 5 == m_iWave)
			{
				if (65.f <= m_pTargetInfo->m_vPos.x && 67.f >= m_pTargetInfo->m_vPos.x &&
					5.5f <= m_pTargetInfo->m_vPos.y && 8.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[4] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
				}
			}
			// 체크포인트 6
			if (false == m_bStage2MovingPoint[5] && 6 == m_iWave)
			{
				if (63.f <= m_pTargetInfo->m_vPos.x && 65.f >= m_pTargetInfo->m_vPos.x &&
					20.5f <= m_pTargetInfo->m_vPos.y && 23.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[5] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
					m_fMovingZ = 4.f;
				}
			}
			// 체크포인트 7
			if (false == m_bStage2MovingPoint[6] && 7 == m_iWave)
			{
				if (50.5f <= m_pTargetInfo->m_vPos.x && 52.5f >= m_pTargetInfo->m_vPos.x &&
					20.5f <= m_pTargetInfo->m_vPos.y && 23.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[6] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
					m_fMovingZ = 4.f;
				}
			}
			// 체크포인트 8
			if (false == m_bStage2MovingPoint[7] && 8 == m_iWave)
			{
				if (47.f <= m_pTargetInfo->m_vPos.x && 49.f >= m_pTargetInfo->m_vPos.x &&
					35.5f <= m_pTargetInfo->m_vPos.y && 38.5f >= m_pTargetInfo->m_vPos.y)
				{
					m_bStage2MovingPoint[7] = true;
					m_bStageWaveMoving = true;
					m_bStageWavePoint = false;
					m_fMovingZ = 9.f;
				}
			}
			break;
		}
	}
}

void CStaticCamera::CameraStageWaveMoving()
{
	D3DXVECTOR3 vPlayerPos = m_pTargetInfo->m_vPos;
	D3DXVECTOR3 vTargetPos, vDir, vDist;

	/************************ (SongSong) ************************/
	/*  체크포인트 갈때  */
	if (false == m_bStageWavePoint)
	{
		switch (m_iScene)
		{
			/************************ (SongSong) ************************/
			/*  스테이지 1  */
		case SCENEID::SC_STAGE:
			// 체크포인트 1
			if (true == m_bStage1MovingPoint[0] && 1 == m_iWave)
			{
				vTargetPos = { 9.6f, 28.f, 0.f };

				vDist = vTargetPos - m_vEye;
				if (m_vEye.z * -1.f + 0.1f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 4.5f * GET_TIME;
					m_vEye.y += vDir.y * 8.f  * GET_TIME;
				}
				else
				{
					m_bStageWavePoint = true;
					break;
				}

				vDist = vTargetPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 7.5f * GET_TIME;
					m_vAt.y += vDir.y * 10.7f * GET_TIME;
				}
			}
			// 체크포인트 2
			if (true == m_bStage1MovingPoint[1] && 2 == m_iWave)
			{
				D3DXVECTOR3 vTargetPos2 = { 38.f, vPlayerPos.y + 10.f, -17.f };
				vTargetPos = { 38.f, vPlayerPos.y, 0.f };

				if (-18.f < m_vEye.z)
					m_vEye.z -= 2.1f  * GET_TIME;
				else
					m_vEye.z = -18.f;

				vDist = vTargetPos2 - m_vEye;
				if (vTargetPos2.x - 0.51f > m_vEye.x)
				{
					vDir = vTargetPos2 - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 8.f * GET_TIME;
				}
				else
				{
					m_bStageWavePoint = true;
					break;
				}

				vDist = vTargetPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 12.05f * GET_TIME;
					m_vAt.y = vTargetPos.y;
				}
			}
			// 체크포인트 3
			if (true == m_bStage1MovingPoint[2] && 3 == m_iWave)
			{
				vDir = vPlayerPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				m_vEye.x += vDir.x * 4.f * GET_TIME;
				m_vEye.y += vDir.y * 4.f  * GET_TIME;
				if (-7.f > m_vEye.z)
					m_vEye.z += 3.f * GET_TIME;
				else
				{
					m_vEye.z = -7.f;
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					//m_bStage1MovingPoint[2] = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_NEAR;
					if (3 == m_iWave)
						++m_iWave;
				}
			}
			// 체크포인트 4
			if (true == m_bStage1MovingPoint[3] && 4 == m_iWave)
			{
				if (-10.f < m_vEye.z)
					m_vEye.z -= 2.f * GET_TIME;
				else
				{
					m_vEye.z = -10.f;
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					//m_bStage1MovingPoint[3] = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
				}
			}
			break;
			/************************ (SongSong) ************************/
			/*  스테이지 2  */
		case SCENEID::SC_STAGE2:
			// 체크포인트 1
			if (true == m_bStage2MovingPoint[0] && 1 == m_iWave)
			{
				vTargetPos = { 33.f, 21.f, 0.f };

				vDist = vTargetPos - m_vEye;
				if (m_vEye.z * -1.f + 0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 10.f * GET_TIME;
					m_vEye.y += vDir.y * 6.5f  * GET_TIME;
				}
				else
				{
					m_bStageWavePoint = true;
					break;
				}

				vDist = vTargetPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 12.7f * GET_TIME;
					m_vAt.y += vDir.y * 10.f * GET_TIME;
				}
			}
			// 체크포인트 2
			if (true == m_bStage2MovingPoint[1] && 2 == m_iWave)
			{
				vTargetPos = { 14.f, 24.f, 0.f };

				m_fMovingZ -= 7.f * GET_TIME;
				if (/*-28.f < m_vEye.z && */0.f < m_fMovingZ)
					m_vEye.z -= m_fMovingZ * GET_TIME;
				else
				{
					//m_vEye.z = -28.f;
					//m_bStageWavePoint = true;
				}

				vDir = vTargetPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vTargetPos.x < m_vEye.x)
					m_vAt.x = m_vEye.x += vDir.x * 17.f * GET_TIME;

				if (vTargetPos.y < m_vEye.y)
					m_vAt.y = m_vEye.y += vDir.y * 17.f  * GET_TIME;

				if (vTargetPos.x + 1.2f > m_vEye.x
					&& vTargetPos.y + 1.2f > m_vEye.y)
				{
					m_bStageWavePoint = true;
					m_fMovingZ = 19.f;
					break;
				}
			}
			// 체크포인트 3
			if (true == m_bStage2MovingPoint[2] && 3 == m_iWave)
			{
				if (-7.f > m_vEye.z)
					m_vEye.z += 3.f * GET_TIME;
				else
				{
					m_vEye.z = -7.f;
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					//m_bStage1MovingPoint[2] = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_NEAR;
					if (3 == m_iWave)
						++m_iWave;
				}
			}
			// 체크포인트 4
			if (true == m_bStage2MovingPoint[3] && 4 == m_iWave)
			{
				if (-10.f < m_vEye.z)
					m_vEye.z -= 2.f * GET_TIME;
				else
				{
					m_vEye.z = -10.f;
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					//m_bStage1MovingPoint[3] = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (4 == m_iWave)
						++m_iWave;
				}
			}
			// 체크포인트 5
			if (true == m_bStage2MovingPoint[4] && 5 == m_iWave)
			{
				vTargetPos = { 72.f, 24.f, 0.f };

				vDist = vTargetPos - m_vEye;
				if (m_vEye.z * -1.f + 0.15f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 7.f * GET_TIME;
					m_vEye.y += vDir.y * 8.5f  * GET_TIME;
				}
				else
				{
					m_bStageWavePoint = true;
					break;
				}

				vDist = vTargetPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vTargetPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 7.f * GET_TIME;
					m_vAt.y += vDir.y * 11.f * GET_TIME;
				}
			}
			// 체크포인트 6
			if (true == m_bStage2MovingPoint[5] && 6 == m_iWave)
			{
				vTargetPos = { 58.9f, 21.8f, 0.f };

				m_fMovingZ -= 2.65f * GET_TIME;
				if (/*-13.f < m_vEye.z ||*/ 0.f < m_fMovingZ)
					m_vEye.z -= m_fMovingZ * GET_TIME;
				/*else
				m_vEye.z = -13.f;*/

				vDir = vTargetPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vTargetPos.x < m_vEye.x)
					m_vAt.x = m_vEye.x += vDir.x * 17.f * GET_TIME;

				m_vAt.y = m_vEye.y += vDir.y * 17.f  * GET_TIME;

				if (vTargetPos.x + 0.05f > m_vEye.x
					/*&& vTargetPos.y + 0.1f > m_vEye.y*/)
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_VERY_FAR;
					m_fZoomZ = m_vEye.z;
					if (6 == m_iWave)
						++m_iWave;
					break;
				}
			}
			// 체크포인트 7
			if (true == m_bStage2MovingPoint[6] && 7 == m_iWave)
			{
				m_fMovingZ -= 3.f * GET_TIME;
				if (-10.f > m_vEye.z)
					m_vEye.z += m_fMovingZ * GET_TIME;
				else
					m_vEye.z = -10.f;

				vDir = vPlayerPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vPlayerPos.x < m_vEye.x)
					m_vAt.x = m_vEye.x += vDir.x * 20.f * GET_TIME;

				m_vAt.y = m_vEye.y += vDir.y * 20.f  * GET_TIME;

				if (vPlayerPos.x + 0.05f > m_vEye.x
					/*&& vTargetPos.y + 0.1f > m_vEye.y*/)
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (7 == m_iWave)
						++m_iWave;
					break;
				}
			}
			// 체크포인트 8
			if (true == m_bStage2MovingPoint[7] && 8 == m_iWave)
			{
				vTargetPos = { 57.f, 36.f, 0.f };

				m_fMovingZ -= 5.f * GET_TIME;
				if (/*-28.f < m_vEye.z && */0.f < m_fMovingZ)
					m_vEye.z -= m_fMovingZ * GET_TIME;
				else
				{
					//m_vEye.z = -28.f;
					//m_bStageWavePoint = true;
				}

				vDir = vTargetPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vTargetPos.x > m_vEye.x)
					m_vEye.x += vDir.x * 13.5f * GET_TIME;

				if (vTargetPos.y < m_vEye.y)
					m_vEye.y += vDir.y * 10.5f  * GET_TIME;

				if (vTargetPos.x - 0.75f < m_vEye.x
					&& vTargetPos.y + 0.75f > m_vEye.y)
				{
					m_bStageWavePoint = true;
					m_fMovingZ = 11.f;
					break;
				}

				vDir = vTargetPos - m_vAt;

				if (0.3f < D3DXVec3Length(&vDir))
				{
					D3DXVec3Normalize(&vDir, &vDir);
					m_vAt.x += vDir.x * 17.f * GET_TIME;
					m_vAt.y += vDir.y * 14.f  * GET_TIME;
				}

			}
			break;
		}
	}
	/************************ (SongSong) ************************/
	/*  원점 돌아올때  */
	else if (true == m_bStageWavePoint)
	{
		switch (*m_pCameraObserver->GetSceneID())
		{
			/************************ (SongSong) ************************/
			/*  스테이지 1  */
		case SCENEID::SC_STAGE:
			// 체크포인트 1
			if (true == m_bStage1MovingPoint[0] && 1 == m_iWave)
			{
				vDist = vPlayerPos - m_vEye;
				if (m_vEye.z * -1.f + 1.03f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 5.5f * GET_TIME;
					m_vEye.y += vDir.y * 9.f  * GET_TIME;
				}
				else
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (1 == m_iWave)
						++m_iWave;
					break;
				}

				vDist = vPlayerPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 7.5f * GET_TIME;
					m_vAt.y += vDir.y * 14.7f * GET_TIME;
				}
				else
					m_vAt = m_pTargetInfo->m_vPos;
			}
			// 체크포인트 2
			if (true == m_bStage1MovingPoint[1] && 2 == m_iWave)
			{
				vDist = vPlayerPos - m_vEye;

				if (-13.f > m_vEye.z)
					m_vEye.z += 3.f * GET_TIME;
				else
					m_vEye.z = -13.f;

				if (vPlayerPos.x + 0.4f < m_vEye.x)
				{
					vDir = vPlayerPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 16.f * GET_TIME;
				}
				else
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_FAR;
					if (2 == m_iWave)
						++m_iWave;
					break;
				}

				vDist = vPlayerPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 16.5f * GET_TIME;
				}
				else
					m_vAt = m_pTargetInfo->m_vPos;
			}
			break;
			/************************ (SongSong) ************************/
			/*  스테이지 2  */
		case SCENEID::SC_STAGE2:
			// 체크포인트 1
			if (true == m_bStage2MovingPoint[0] && 1 == m_iWave)
			{
				vDist = vPlayerPos - m_vEye;

				if (m_vEye.z * -1.f + 0.66f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 11.f * GET_TIME;
					m_vEye.y += vDir.y * 11.f  * GET_TIME;
				}
				else
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (1 == m_iWave)
						++m_iWave;
					break;
				}

				vDist = vPlayerPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 16.5f * GET_TIME;
					m_vAt.y += vDir.y * 8.f * GET_TIME;
				}
				else
					m_vAt = m_pTargetInfo->m_vPos;
			}
			// 체크포인트 2
			if (true == m_bStage2MovingPoint[1] && 2 == m_iWave)
			{
				m_fMovingZ -= 8.7f * GET_TIME;
				if (-10.f > m_vEye.z && 0.f < m_fMovingZ)
					m_vEye.z += m_fMovingZ * GET_TIME;
				else
					m_vEye.z = -10.f;

				vDir = vPlayerPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vPlayerPos.x > m_vEye.x)
					m_vAt.x = m_vEye.x += vDir.x * 22.f * GET_TIME;

				if (vPlayerPos.y > m_vEye.y)
					m_vAt.y = m_vEye.y += vDir.y * 22.f  * GET_TIME;

				if (vPlayerPos.x - 0.03f < m_vEye.x
					&& vPlayerPos.y - 0.03f < m_vEye.y)
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (2 == m_iWave)
						++m_iWave;
					break;
				}
			}
			// 체크포인트 5
			if (true == m_bStage2MovingPoint[4] && 5 == m_iWave)
			{
				vDist = vPlayerPos - m_vEye;
				if (m_vEye.z * -1.f + 1.04f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vEye;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vEye.x += vDir.x * 8.f * GET_TIME;
					m_vEye.y += vDir.y * 10.5f  * GET_TIME;
				}
				else
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (5 == m_iWave)
						++m_iWave;
					break;
				}

				vDist = vPlayerPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDist))
				{
					vDir = vPlayerPos - m_vAt;
					D3DXVec3Normalize(&vDir, &vDir);

					m_vAt.x += vDir.x * 8.f * GET_TIME;
					m_vAt.y += vDir.y * 15.f * GET_TIME;
				}
				else
					m_vAt = m_pTargetInfo->m_vPos;
			}
			// 체크포인트 8
			if (true == m_bStage2MovingPoint[7] && 8 == m_iWave)
			{
				m_fMovingZ -= 7.f * GET_TIME;
				if (-10.f > m_vEye.z && 0.f < m_fMovingZ)
					m_vEye.z += m_fMovingZ * GET_TIME;
				else
					m_vEye.z = -10.f;

				vDir = vPlayerPos - m_vEye;
				D3DXVec3Normalize(&vDir, &vDir);

				if (vPlayerPos.x < m_vEye.x)
					m_vEye.x += vDir.x * 14.f * GET_TIME;

				if (vPlayerPos.y > m_vEye.y)
					m_vEye.y += vDir.y * 10.f  * GET_TIME;

				if (vPlayerPos.x + 0.45f > m_vEye.x
					&& vPlayerPos.y - 0.45f < m_vEye.y)
				{
					m_bStageWaveMoving = false;
					m_bStageWavePoint = false;
					m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;
					if (8 == m_iWave)
						++m_iWave;
					break;
				}

				vDir = vPlayerPos - m_vAt;
				if (0.3f < D3DXVec3Length(&vDir))
				{
					D3DXVec3Normalize(&vDir, &vDir);
					m_vAt.x += vDir.x * 17.f * GET_TIME;
					m_vAt.y += vDir.y * 12.5f  * GET_TIME;
				}
			}
			break;
		}
	}

	CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::CTransform* pTargetInfo)
{
	CStaticCamera*   pInstance = new CStaticCamera(pGraphicDev);

	pInstance->SetTarget(pTargetInfo);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	pInstance->SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	return pInstance;
}

void CStaticCamera::SetTarget(const Engine::CTransform* pTargetInfo)
{
	if (pTargetInfo == nullptr)
	{
		m_vAt = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vEye = D3DXVECTOR3(0.f, 0.f, -10.f);
	}
	m_pTargetInfo = pTargetInfo;
}


void CStaticCamera::UpdateLogoCamera()
{
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
	{
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		pt.x -= WINCX / 2;
		pt.y -= WINCY / 2;

		m_vEye.x = pt.x / 150.f;
		m_vEye.y = -(pt.y / 150.f);

		if (fabsf(m_vEye.x) > 3.f)
		{
			if (m_vEye.x < 0)
				m_vEye.x = -3.f;
			else
				m_vEye.x = 3.f;
		}
		if (fabsf(m_vEye.y) > 3.f)
		{
			if (m_vEye.y < 0)
				m_vEye.y = -3.f;
			else
				m_vEye.y = 3.f;
		}
	}
	else
	{
		D3DXVECTOR3 vecToOrigin;
		ZeroMemory(&vecToOrigin, sizeof(D3DXVECTOR3));

		vecToOrigin.x = -m_vEye.x;
		vecToOrigin.y = -m_vEye.y;

		if (D3DXVec3Length(&vecToOrigin) < 0.1f)
			return;
		D3DXVec3Normalize(&vecToOrigin, &vecToOrigin);

		m_vEye += vecToOrigin * 5.f * GET_TIME;
	}
}

void CStaticCamera::BasicCameraDefaultInit()
{
	if (nullptr != m_pTargetInfo)
	{
		D3DXVECTOR3 vDir = { 0.f, 0.f, 10.f };
		m_vEye = vDir * (-1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);

		m_vEye *= m_fDistance;

		D3DXVECTOR3   vRight;
		memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);//

		m_fAngle = 0.f;

		D3DXMATRIX   matRotAxis;//
		D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);//
		D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);//

		m_vEye += m_pTargetInfo->m_vPos;
		m_vAt = m_pTargetInfo->m_vPos;

		if (/*SCENEID::SC_TUTORIAL == m_iScene
			|| */SCENEID::SC_TUTORIAL_THREAD == m_iScene)
		{
			m_vEye.x = 0.f;
			m_vAt.x = 0.f;
			m_vAt.x = 0.f;
			m_vAt.y = 0.f;
		}

		CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}

void CStaticCamera::StageDoorGofor()
{
	if (NARRATIVE_VIEW::FIRST_PERSON == m_eNarrativeView && false == m_bDoorGofor || SCENEID::SC_STAGE != m_iScene
		&& SCENEID::SC_STAGE2 != m_iScene && SCENEID::SC_STAGE3 != m_iScene)
		return;
	if (TRUE == m_bNarrativeMoveing || true == m_bStageWaveMoving)
		return;

	D3DXVECTOR3 vDoorPos = CCubeMgr::GetInstance()->GetLockDoorPos();
	D3DXVECTOR3 vDir;

	static bool bSoundStart = false;
	/************************ (SongSong) ************************/
	/*  디폴트 시점  */
	if (NARRATIVE_VIEW::DEFAULT == m_eNarrativeView)
	{
		/************************ (SongSong) ************************/
		/*  문쪽을 향해 갈때  */
		if (false == m_bDoorPoint)
		{
			if (SCENEID::SC_STAGE == m_iScene)
			{
				vDoorPos.x /= 1.55f;
				vDoorPos.y /= 1.42f;
			}
			else if (SCENEID::SC_STAGE2 == m_iScene)
			{
				vDoorPos.x /= 1.4f;
				vDoorPos.y /= 1.3f;
			}
			vDir = vDoorPos - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vEye.x += vDir.x * 20.f * GET_TIME;
			m_vEye.y += vDir.y * 20.f * GET_TIME;
			m_vAt.x += vDir.x * 20.f * GET_TIME;
			m_vAt.y += vDir.y * 20.f * GET_TIME;
			if (-45.f < m_vEye.z)
				m_vEye.z -= 15.f * GET_TIME;
			else
				m_vEye.z = -45.f;

			if (vDoorPos.x - 1.f <= m_vEye.x
				&& vDoorPos.y - 1.f <= m_vEye.y)
			{
				m_bDoorPoint = true;
			}

			bSoundStart = true;
		}
		/************************ (SongSong) ************************/
		/*  플레이어 위치로 돌아갈 때  */
		else if (true == m_bDoorPoint)
		{
			vDir = m_pTargetInfo->m_vPos - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vEye.x += vDir.x * 60.f * GET_TIME;
			m_vEye.y += vDir.y * 60.f * GET_TIME;
			m_vAt.x += vDir.x * 60.f * GET_TIME;
			m_vAt.y += vDir.y * 60.f * GET_TIME;

			/************************ (SongSong) ************************/
			/*  위치가 플레이어보다 오른쪽일떄  */
			if (m_pTargetInfo->m_vPos.x < m_vEye.x)
			{
				if (m_pTargetInfo->m_vPos.x + 20.f >= m_vEye.x
					&& m_pTargetInfo->m_vPos.y + 20.f >= m_vEye.y)
				{
					if (-10.f > m_vEye.z)
						m_vEye.z += 33.f * GET_TIME;
					else
						m_vEye.z = -10.f;
				}
				if (m_pTargetInfo->m_vPos.x + 0.025f >= m_vEye.x
					&& m_pTargetInfo->m_vPos.y + 0.025f >= m_vEye.y
					&& -10.f <= m_vEye.z)
				{
					m_bDoorGofor = false;
					m_vEye.x = m_pTargetInfo->m_vPos.x;
					m_vEye.y = m_pTargetInfo->m_vPos.y;
					m_vEye.z = -10.f;
					m_vAt = m_pTargetInfo->m_vPos;

					if (bSoundStart)
					{
						Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::BGM);
						switch (*CCameraObserver::GetInstance()->GetSceneID())
						{
						case SCENEID::SC_STAGE:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						case SCENEID::SC_STAGE2:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage2BGM.wav");
							break;
						case SCENEID::SC_STAGE3:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage3BGM.wav");
							break;
						case SCENEID::SC_END:
							//Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						default:
							break;
						}
						bSoundStart = false;
					}
					//m_pCameraObserver->GetPlayer()->Set_Respawn(FALSE);
				}
			}
			/************************ (SongSong) ************************/
			/*  위치가 플레이어보다 왼쪽일떄  */
			else if (m_pTargetInfo->m_vPos.x > m_vEye.x)
			{
				if (m_pTargetInfo->m_vPos.x - 20.f <= m_vEye.x
					&& m_pTargetInfo->m_vPos.y + 20.f >= m_vEye.y)
				{
					if (-10.f > m_vEye.z)
						m_vEye.z += 33.f * GET_TIME;
					else
						m_vEye.z = -10.f;
				}
				if (m_pTargetInfo->m_vPos.x - 0.025f <= m_vEye.x
					&& m_pTargetInfo->m_vPos.y - 0.025f <= m_vEye.y
					&& -10.f <= m_vEye.z)
				{
					m_bDoorGofor = false;
					m_vEye.x = m_pTargetInfo->m_vPos.x;
					m_vEye.y = m_pTargetInfo->m_vPos.y;
					m_vEye.z = -10.f;
					m_vAt = m_pTargetInfo->m_vPos;

					if (bSoundStart)
					{
						Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::BGM);
						switch (*CCameraObserver::GetInstance()->GetSceneID())
						{
						case SCENEID::SC_LOGO:
							Engine::Get_SoundMgr()->PlayBGM(L"LogoBGM.wav");
							break;
						case SCENEID::SC_TUTORIAL:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						case SCENEID::SC_TUTORIAL_THREAD:
							//Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						case SCENEID::SC_STAGE:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						case SCENEID::SC_STAGE2:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage2BGM.wav");
							break;
						case SCENEID::SC_STAGE3:
							Engine::Get_SoundMgr()->PlayBGM(L"Stage3BGM.wav");
							break;
						case SCENEID::SC_END:
							//Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
							break;
						default:
							break;
						}
						bSoundStart = false;
					}
					//m_pCameraObserver->GetPlayer()->Set_Respawn(FALSE);
				}
			}
		}
	}
	/************************ (SongSong) ************************/
	/*  1인칭 시점  */
	else if (NARRATIVE_VIEW::FIRST_PERSON == m_eNarrativeView)
	{
		/************************ (SongSong) ************************/
		/*  플레이어 위치로 돌아갈 때  */
		if (true == m_bDoorPoint)
		{
			vDir = m_pTargetInfo->m_vPos - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vEye.x += vDir.x * 5.0f * GET_TIME;
			m_vEye.y += vDir.y * 5.0f * GET_TIME;
			m_vEye.z += vDir.z * 5.0f * GET_TIME;

			/************************ (SongSong) ************************/
			/*  위치가 플레이어보다 오른쪽일떄  */
			if (m_pTargetInfo->m_vPos.x < m_vEye.x)
			{
				if (m_pTargetInfo->m_vPos.x + 0.025f >= m_vEye.x
					&& m_pTargetInfo->m_vPos.y + 0.025f >= m_vEye.y)
				{
					m_bDoorGofor = false;
					m_vEye.x = m_pTargetInfo->m_vPos.x;
					m_vEye.y = m_pTargetInfo->m_vPos.y;
					//m_vEye.z = -10.f;
					m_vEye = m_pTargetInfo->m_vPos;
					//m_pCameraObserver->GetPlayer()->Set_Respawn(FALSE);
				}
			}
			/************************ (SongSong) ************************/
			/*  위치가 플레이어보다 왼쪽일떄  */
			else if (m_pTargetInfo->m_vPos.x > m_vEye.x)
			{
				if (m_pTargetInfo->m_vPos.x - 0.025f <= m_vEye.x
					&& m_pTargetInfo->m_vPos.y - 0.025f <= m_vEye.y)
				{
					m_bDoorGofor = false;
					m_vEye.x = m_pTargetInfo->m_vPos.x;
					m_vEye.y = m_pTargetInfo->m_vPos.y;
					//m_vEye.z = -10.f;
					m_vEye = m_pTargetInfo->m_vPos;
					//m_pCameraObserver->GetPlayer()->Set_Respawn(FALSE);
				}
			}
		}
	}
	CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
}


void CStaticCamera::StageAddKeyUpdate()
{
	if (TRUE == m_pCameraObserver->GetKey()->GetIsGetKey())
	{
		m_vEye = { 0.f, -0.5f, -4.f };

		D3DXVECTOR3   vUp;
		memcpy(&vUp, &m_pCameraObserver->GetKey()->GetInfoCom()->m_matWorld.m[1][0], sizeof(float) * 3);//

		m_fAngle += 2.f * GET_TIME;
		D3DXMATRIX   matRotAxis;//
		D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fAngle);
		D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

		m_vEye += m_pCameraObserver->GetKey()->Get_Pos();
		m_vAt = m_pCameraObserver->GetKey()->Get_Pos();

		/************************ (SongSong) ************************/
		/*  2.5초뒤에 끝  */
		static float fTime = 0.f;
		fTime += 1.f *GET_TIME;
		if (3.f < fTime)
		{
			m_vEye = m_pTargetInfo->m_vPos;
			m_vEye.z = -10.f;
			m_vAt = m_pTargetInfo->m_vPos;

			m_pCameraObserver->GetKey()->SetIsGetKey(FALSE);
			m_pCameraObserver->GetKey()->SetDead(TRUE);
			fTime = 0.f;
		}

		CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}