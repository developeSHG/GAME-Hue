#include "stdafx.h"
#include "CubeMgr.h"

#include "Export_Function.h"
#include "enum.h"
/************************ (SongSong) ************************/
/*  Cube GameObject  */
#include "SkyBox.h"
#include "CubeDeco.h"
#include "CubeBrick.h"
#include "CubeCrate.h"
#include "CubePlatform.h"
#include "CubeSkull.h"
#include "StaticCamera.h"
#include "Battery.h"
/* Obstacle Object */
#include "ObjBasic.h"
#include "Key.h"
#include "Door.h"
#include "ButtonX.h"
#include "Player.h"
#include "CubeRope.h"
#include "CubeLaser.h"
#include "CubeSpikes.h"
#include "CubeStairs.h"
#include "CubeRock.h"
#include "CubeBalloon.h"
#include "SavePoint.h"
#include "InsideCircle.h"
#include "ColorCircle.h"

IMPLEMENT_SINGLETON(CCubeMgr)

CCubeMgr::CCubeMgr(void)
	: m_pResourceMgr(Engine::Get_ResourceMgr())
{
	m_bIsStageChange = false;
	m_bResetBattery = false;
	m_bInitOriginPos = false;
	m_bIsRender = false;
	m_bIsResize = false;
	m_bGetBattery = false;
	m_bIsPlayerDie = false;
	m_pCameraObserver = CCameraObserver::GetInstance();
	m_ePlayerColColor = m_eCurCubeColor = Engine::CUBE_COLOR::CUBE_COLOR_END;

	m_iSaveCount = 0;
}

CCubeMgr::~CCubeMgr(void)
{
	Release();
}

void CCubeMgr::Release(void)
{
	for (auto mapiter : m_mapTutorialCubeList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapTutorialCubeList.clear();

	for (auto mapiter : m_mapStage1CubeList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage1CubeList.clear();

	for (auto mapiter : m_mapStage2CubeList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage2CubeList.clear();

	for (auto mapiter : m_mapStage3CubeList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage3CubeList.clear();

	///////////////////////////////////////////////////////////////////

	for (auto mapiter : m_mapTutorialObjList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapTutorialObjList.clear();

	for (auto mapiter : m_mapStage1ObjList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage1ObjList.clear();

	for (auto mapiter : m_mapStage2ObjList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage2ObjList.clear();

	for (auto mapiter : m_mapStage3ObjList)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapStage3ObjList.clear();

	/////////////////////////////////////////////////////

	for (auto mapiter : m_mapAllStageParticle)
	{
		for (auto iter : mapiter.second)
		{
			Engine::Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapAllStageParticle.clear();
}

HRESULT CCubeMgr::Load_Tutorial(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;

	FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Tutorial.dat", SCENEID::SC_TUTORIAL), E_FAIL);
	FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Tutorial.objdat", SCENEID::SC_TUTORIAL), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage1.dat", SCENEID::SC_TUTORIAL), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage1.objdat", SCENEID::SC_TUTORIAL), E_FAIL);
	return S_OK;
}

HRESULT CCubeMgr::Load_Stage(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;

	//ResourceForStage(_pGraphicDev);

	FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage1.dat", SCENEID::SC_STAGE), E_FAIL);
	FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage1.objdat", SCENEID::SC_STAGE), E_FAIL);
	FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage2.dat", SCENEID::SC_STAGE2), E_FAIL);
	FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage2.objdat", SCENEID::SC_STAGE2), E_FAIL);
	CreateStage3Cube();
	CreateStage3Object();
	//FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage3.dat"), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage3.objdat"), E_FAIL);

	CreateParticle();

	return S_OK;
}

HRESULT CCubeMgr::Load_Stage2(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	//FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage2.dat"), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage2.objdat"), E_FAIL);

	return S_OK;
}

HRESULT CCubeMgr::Load_Stage3(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	//FAILED_CHECK_RETURN(Load_Cube(_pGraphicDev, L"../../Data/Cube_Stage3.dat"), E_FAIL);
	//FAILED_CHECK_RETURN(Load_Obj(_pGraphicDev, L"../../Data/Obj_Stage3.objdat"), E_FAIL);

	return S_OK;
}

HRESULT CCubeMgr::Load_Cube(LPDIRECT3DDEVICE9 _pGraphicDev, const TCHAR* _szPathFile, SCENEID eID)
{
	HANDLE hFile = CreateFile(_szPathFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;
	TCHAR szCubeType[MIN_STR];
	Engine::CUBE_INFO tCubeInfo;

	/************************ (SongSong) ************************/
	/*  Cube 로드생성  */
	float x = 0.f;
	float y = 0.f;
	while (TRUE)
	{
		ReadFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
		ReadFile(hFile, &tCubeInfo, sizeof(Engine::CUBE_INFO), &dwByte, nullptr);

		tCubeInfo.vPos.x += x;
		tCubeInfo.vPos.y += y;
		//x += 0.0001f;
		//y += 0.0001f;

		if (0 == dwByte)
			break;

		CUBEID eCubeId;

		if (0 == lstrcmp(L"CubeDeco", szCubeType))
			eCubeId = CUBEID::CUBE_DECO;
		else if (0 == lstrcmp(L"CubeBrick", szCubeType))
			eCubeId = CUBEID::CUBE_BRICK;
		else if (0 == lstrcmp(L"CubeCrate", szCubeType))
			eCubeId = CUBEID::CUBE_CRATE;
		else if (0 == lstrcmp(L"CubePlatform", szCubeType))
			eCubeId = CUBEID::CUBE_PLATFORM;
		else if (0 == lstrcmp(L"CubeSkull", szCubeType))
			eCubeId = CUBEID::CUBE_SKULL;
		else if (0 == lstrcmp(L"CubeRope", szCubeType))
			eCubeId = CUBEID::CUBE_ROPE;
		else if (0 == lstrcmp(L"CubeLaser", szCubeType))
			eCubeId = CUBEID::CUBE_LASER;
		else if (0 == lstrcmp(L"CubeSpikes", szCubeType))
			eCubeId = CUBEID::CUBE_SPIKES;
		else if (0 == lstrcmp(L"CubeStairs", szCubeType))
			eCubeId = CUBEID::CUBE_STAIRS;
		else if (0 == lstrcmp(L"CubeRock", szCubeType))
			eCubeId = CUBEID::CUBE_ROCK;
		else if (0 == lstrcmp(L"CubeBalloon", szCubeType))
			eCubeId = CUBEID::CUBE_BALLOON;

		Engine::CCube*   pCube = nullptr;

		if (SC_TUTORIAL == eID)
		{
			tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
		}

		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(_pGraphicDev, tCubeInfo);
			break;
		}

		if (SC_TUTORIAL != eID && eCubeId != CUBE_LASER)
			pCube->SetMovingCube();

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}

		switch (eID)
		{
		case SC_TUTORIAL:
		{
			auto& iter = m_mapTutorialCubeList.find(eCubeId);

			if (iter == m_mapTutorialCubeList.end())
				m_mapTutorialCubeList[eCubeId] = list<Engine::CCube*>();

			m_mapTutorialCubeList[eCubeId].push_back(pCube);
		}
		break;
		case SC_STAGE:
		{
			auto& iter = m_mapStage1CubeList.find(eCubeId);

			if (iter == m_mapStage1CubeList.end())
				m_mapStage1CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage1CubeList[eCubeId].push_back(pCube);
		}
		break;
		case SC_STAGE2:
		{
			auto& iter = m_mapStage2CubeList.find(eCubeId);

			if (iter == m_mapStage2CubeList.end())
				m_mapStage2CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage2CubeList[eCubeId].push_back(pCube);
		}
		break;
		case SC_STAGE3:
		{
			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);
		}
		break;
		}

		/************************ (SongSong) ************************/
		/*  애들눕히기  */
		//pCube->SwapPos();
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CCubeMgr::Load_Obj(LPDIRECT3DDEVICE9 _pGraphicDev, const TCHAR * _szPathFile, SCENEID eID)
{
	HANDLE hFile = CreateFile(_szPathFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("Obj Load Failed"), L"System Message", MB_OK);
		return E_FAIL;
	}

	DWORD dwByte = 0;
	TCHAR szCubeType[MIN_STR];
	Engine::OBJ_INFO tObjInfo;

	/************************ (SongSong) ************************/
	/*  Cube 로드생성  */
	float x = 0.f;
	float y = 0.f;
	while (TRUE)
	{

		ReadFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
		ReadFile(hFile, &tObjInfo, sizeof(Engine::OBJ_INFO), &dwByte, nullptr);

		tObjInfo.vPos.x += x;
		tObjInfo.vPos.y += y;


		//x += 0.0001f;
		//y += 0.0001f;

		if (0 == dwByte)
			break;

		OBJID eObjId;

		if (0 == lstrcmp(L"Object_Door", szCubeType))
			eObjId = OBJID::OBJ_DOOR;
		else if (0 == lstrcmp(L"Object_DoorLock", szCubeType))
			eObjId = OBJID::OBJ_DOORLOCK;
		else if (0 == lstrcmp(L"Object_Key", szCubeType))
			eObjId = OBJID::OBJ_KEY;
		else if (0 == lstrcmp(L"Object_Axe", szCubeType))
			eObjId = OBJID::OBJ_BATTERY;
		else if (0 == lstrcmp(L"Object_Boat", szCubeType))
			eObjId = OBJID::OBJ_SAVEPOINT;
		else
			eObjId = OBJID::OBJ_DECO;

		CObjBasic*   pObj = nullptr;

		switch (eObjId)
		{
		case OBJID::OBJ_DECO:
			pObj = CObjBasic::Create(_pGraphicDev, tObjInfo);
			if (0 == lstrcmp(L"Object_Chandlier", szCubeType))
				pObj->SetDecoID(Engine::OBJDECOID::OBJDECO_TOPFIRE);
			else if (0 == lstrcmp(L"Object_FirePot", szCubeType))
				pObj->SetDecoID(Engine::OBJDECOID::OBJDECO_FIREPOT);
			else if (0 == lstrcmp(L"Object_Lantern", szCubeType))
			{
				if (tObjInfo.tTex.iIndex == 0)
					pObj->SetDecoID(Engine::OBJDECOID::OBJDECO_LEFTLANTERN);
				else
					pObj->SetDecoID(Engine::OBJDECOID::OBJDECO_RIGHTLANTERN);
			}
			else
				pObj->SetDecoID(Engine::OBJDECOID::OBJDECOID_END);

			break;
		case OBJID::OBJ_DOOR:
			pObj = CDoor::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_DOORLOCK:
			pObj = CDoor::Create(m_pGraphicDev, tObjInfo);
			dynamic_cast<CDoor*>(pObj)->Set_Lock();
			dynamic_cast<CDoor*>(pObj)->SetEndDoor();
			break;
		case OBJID::OBJ_KEY:
			pObj = CKey::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_BATTERY:

			pObj = CBattery::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_SAVEPOINT:

			pObj = CSavePoint::Create(m_pGraphicDev, tObjInfo);
			break;
		}
		int eColor = rand() % 8;

		switch (eColor)
		{
		case 0:
			pObj->SetMaterial(RED);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_RED);
			break;
		case 1:
			pObj->SetMaterial(ORANGE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_ORANGE);
			break;
		case 2:
			pObj->SetMaterial(GREEN);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_GREEN);
			break;
		case 3:
			pObj->SetMaterial(YELLOW);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_YELLOW);
			break;
		case 4:
			pObj->SetMaterial(BLUE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_BLUE);
			break;
		case 5:
			pObj->SetMaterial(SKYBLUE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_SKYBLUE);
			break;
		case 6:
			pObj->SetMaterial(PINK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_PINK);
			break;
		case 7:
			pObj->SetMaterial(PURPLE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_PURPLE);
			break;
		case 8:
			pObj->SetMaterial(BLACK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
			break;
		}

		if (eObjId == OBJID::OBJ_DECO || eObjId == OBJID::OBJ_SAVEPOINT)
		{
			pObj->SetMaterial(BLACK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
		}

		switch (eID)
		{
		case SC_TUTORIAL:
		{
			auto& iter = m_mapTutorialObjList.find(eObjId);

			if (iter == m_mapTutorialObjList.end())
				m_mapTutorialObjList[eObjId] = list<CObjBasic*>();

			m_mapTutorialObjList[eObjId].push_back(pObj);
		}
		break;
		case SC_STAGE:
		{
			auto& iter = m_mapStage1ObjList.find(eObjId);

			if (iter == m_mapStage1ObjList.end())
				m_mapStage1ObjList[eObjId] = list<CObjBasic*>();

			m_mapStage1ObjList[eObjId].push_back(pObj);
		}
		break;
		case SC_STAGE2:
		{
			auto& iter = m_mapStage2ObjList.find(eObjId);

			if (iter == m_mapStage2ObjList.end())
				m_mapStage2ObjList[eObjId] = list<CObjBasic*>();

			m_mapStage2ObjList[eObjId].push_back(pObj);
		}
		break;
		case SC_STAGE3:
		{
			auto& iter = m_mapStage3ObjList.find(eObjId);

			if (iter == m_mapStage3ObjList.end())
				m_mapStage3ObjList[eObjId] = list<CObjBasic*>();

			m_mapStage3ObjList[eObjId].push_back(pObj);
		}
		break;
		case SC_END:
			break;
		default:
			break;
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

void CCubeMgr::RenderInsideWorld()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXMATRIX    mView, mProj;
	D3DXMATRIX    mIdentity;
	D3DXMatrixIdentity(&mIdentity);

	D3DXMatrixLookAtLH(&mView, &m_pCameraObserver->GetStaticCamera()->Get_Eye(),
		&m_pCameraObserver->GetStaticCamera()->Get_At(), &m_pCameraObserver->GetStaticCamera()->Get_Up());
	m_View = mView;
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, m_pCameraObserver->GetProj());

	if (SCENEID::SC_TUTORIAL != *m_pCameraObserver->GetSceneID())
	{
		for (auto& MyMap : m_mapAllStageParticle)
		{
			for (auto& MyParticle : MyMap.second)
				MyParticle->RenderInsideWorld();
		}
	}
	else if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
	{
		static bool bCheck = false;
		if (Engine::CUBE_COLOR::CUBE_COLOR_END != CCubeMgr::GetInstance()->Get_CurCubeColor()
			&& 35.f > m_pCameraObserver->GetColorCircle()->Get_InsideCircle()->Get_Scale())
			bCheck = true;
		if (true == bCheck)
		{
			for (auto& MyMap : m_mapAllStageParticle)
			{
				for (auto& MyParticle : MyMap.second)
				{
					MyParticle->RenderInsideWorld();
					break;
				}
				break;
			}
		}
	}

	// Cube
	for (auto& MyMap : *m_mapCurStageCube)
	{
		for (auto& MyCube : MyMap.second)
			MyCube->RenderInsideWorld();
	}
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, m_pCameraObserver->GetProj());

	// Obj
	for (auto& MyMap : *m_mapCurStageObj)
	{
		for (auto& MyObj : MyMap.second)
			MyObj->RenderInsideWorld();
	}


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CCubeMgr::RenderOutsideWorld()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);


	D3DXMATRIX    mView, mProj;
	D3DXMATRIX    mIdentity;
	D3DXMatrixIdentity(&mIdentity);

	D3DXMatrixLookAtLH(&mView, &m_pCameraObserver->GetStaticCamera()->Get_Eye(),
		&m_pCameraObserver->GetStaticCamera()->Get_At(), &m_pCameraObserver->GetStaticCamera()->Get_Up());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, CCameraObserver::GetInstance()->GetProj());

	// Particle
	if (SCENEID::SC_TUTORIAL != *m_pCameraObserver->GetSceneID())
	{
		for (auto& MyMap : m_mapAllStageParticle)
		{
			for (auto& MyParticle : MyMap.second)
				MyParticle->Render();
		}
	}
	else if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
	{
		if (Engine::CUBE_COLOR::CUBE_COLOR_END != CCubeMgr::GetInstance()->Get_CurCubeColor())
		{
			for (auto& MyMap : m_mapAllStageParticle)
			{
				for (auto& MyParticle : MyMap.second)
				{
					MyParticle->Render();
					break;
				}
				break;
			}
		}
	}

	// Cube
	for (auto& MyMap : *m_mapCurStageCube)
	{
		for (auto& MyCube : MyMap.second)
		{
			if (dynamic_cast<CCubeBalloon*>(MyCube) != nullptr)
				MyCube->Render();
			else if (MyCube->Get_CubeColor() != m_eCurCubeColor
				|| MyCube->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
				MyCube->Render();
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXMatrixLookAtLH(&mView, &m_pCameraObserver->GetStaticCamera()->Get_Eye(),
		&m_pCameraObserver->GetStaticCamera()->Get_At(), &m_pCameraObserver->GetStaticCamera()->Get_Up());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
	//   m_pGraphicDev->SetTransform(D3DTS_PROJECTION, CCameraObserver::GetInstance()->GetProj());

	for (auto& MyMap : *m_mapCurStageObj)
	{
		for (auto& MyObj : MyMap.second)
		{
			if (MyObj->Get_CubeColor() != m_eCurCubeColor
				|| MyObj->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
				MyObj->Render();
		}
	}


	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mIdentity);


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CCubeMgr::RenderViewportCube()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);


	//RenderObject();
	if (SCENEID::SC_TUTORIAL != *m_pCameraObserver->GetSceneID())
	{
		for (auto& MyMap : m_mapAllStageParticle)
		{
			for (auto& MyParticle : MyMap.second)
				MyParticle->Render();
		}
	}
	for (auto& MyMap : *m_mapCurStageCube)
	{
		for (auto& MyCube : MyMap.second)
		{
			if (MyCube->Get_CubeColor() != m_eCurCubeColor
				|| MyCube->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
				MyCube->Render();
		}
	}
	for (auto& MyMap : *m_mapCurStageObj)
	{
		for (auto& MyObj : MyMap.second)
		{
			/*if (MyObj->Get_CubeColor() != m_eCurCubeColor
			|| MyObj->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)*/
			MyObj->Render();
		}
	}

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//void CCubeMgr::RenderObject()
//{
//   for (auto& MyMap : m_mapAllStageParticle)
//   {
//      for (auto& MyParticle : MyMap.second)
//         MyParticle->Render();
//   }
//}

void CCubeMgr::Update(void)
{
	for (auto& MyMap : m_mapAllStageParticle)
	{
		for (auto& MyParticle : MyMap.second)
			MyParticle->Update();
	}
	for (auto& MyMap : *m_mapCurStageCube)
	{
		for (auto& MyCube : MyMap.second)
			MyCube->Update();
	}
	for (auto& MyObjMap : *m_mapCurStageObj)
	{
		auto iter_begin = MyObjMap.second.begin();
		auto iter_end = MyObjMap.second.end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->ObjectUpdate();

			if (0 == iEvent)
			{
				delete (*iter_begin);
				iter_begin = MyObjMap.second.erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}

	m_Proj = *m_pCameraObserver->GetProj();
	m_View = *m_pCameraObserver->GetView();
}

void CCubeMgr::LateUpdate(void)
{
}

D3DXVECTOR3 CCubeMgr::GetDoorPos()
{
	if (!m_bGetSavePos)
	{
		auto& iter = m_mapCurStageObj->find(OBJID::OBJ_DOOR);
		return iter->second.front()->Get_Pos();
	}
	else
	{
		return m_vecSavePos;
	}

}

D3DXVECTOR3 CCubeMgr::GetLockDoorPos()
{
	auto& iter = m_mapCurStageObj->find(OBJID::OBJ_DOORLOCK);

	return iter->second.front()->Get_Pos();
}


void CCubeMgr::CreateParticle(void)
{
	auto& iter = m_mapAllStageParticle.find(OBJECTNAME::SKYBOX);

	if (iter == m_mapAllStageParticle.end())
		m_mapAllStageParticle[OBJECTNAME::SKYBOX] = list<Engine::CGameObject*>();

	Engine::CGameObject*      pGameObject = nullptr;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	m_mapAllStageParticle[OBJECTNAME::SKYBOX].emplace_back(pGameObject);

	pGameObject = CButtonX::Create(m_pGraphicDev);
	m_mapAllStageParticle[OBJECTNAME::BUTTON_X].emplace_back(pGameObject);
}

const list<Engine::CCube*>&   CCubeMgr::Get_ListCubeType(CUBEID _eCubeID)
{
	auto& iter = m_mapCurStageCube->find(_eCubeID);
	return iter->second;
}

const list<CObjBasic*>& CCubeMgr::Get_ListObjType(OBJID _eObjID)
{
	auto& iter = m_mapCurStageObj->find(_eObjID);
	return iter->second;
}

void CCubeMgr::Set_CurStageCube(SCENEID _eSceneID)
{
	/************************ (SongSong) ************************/
	/*  기존에 있었던 애들 지우기  */
	//for (auto mapiter : *m_mapCurStageCube)
	//{
	//   for (auto iter : mapiter.second)
	//   {
	//      Engine::Safe_Delete(iter);
	//   }
	//   mapiter.second.clear();
	//}
	//m_mapCurStageCube->clear();

	//for (auto mapiter : *m_mapCurStageObj)
	//{
	//   for (auto iter : mapiter.second)
	//   {
	//      Engine::Safe_Delete(iter);
	//   }
	//   mapiter.second.clear();
	//}
	//m_mapCurStageObj->clear();

	switch (_eSceneID)
	{
	case SC_TUTORIAL:
		m_mapCurStageCube = &m_mapTutorialCubeList;
		m_mapCurStageObj = &m_mapTutorialObjList;

		for (auto& MyMap : *m_mapCurStageCube)
		{
			for (auto& MyCube : MyMap.second)
			{
				MyCube->Get_InfoCom()->m_vPos.x += 300.f;
				MyCube->Get_InfoCom()->m_vPos.y -= 300.f;
			}
		}
		for (auto& MyMap : *m_mapCurStageObj)
		{
			for (auto& MyObj : MyMap.second)
			{
				MyObj->Get_InfoCom()->m_vPos.x += 300.f;
				MyObj->Get_InfoCom()->m_vPos.y -= 300.f;
			}
		}

		break;
	case SC_STAGE:
		m_mapCurStageCube = &m_mapStage1CubeList;
		m_mapCurStageObj = &m_mapStage1ObjList;
		break;
	case SC_STAGE2:
		m_mapCurStageCube = &m_mapStage2CubeList;
		m_mapCurStageObj = &m_mapStage2ObjList;
		break;
	case SC_STAGE3:
		m_mapCurStageCube = &m_mapStage3CubeList;
		m_mapCurStageObj = &m_mapStage3ObjList;
		break;
	}

	auto& iter = m_mapCurStageObj->find(OBJID::OBJ_DOOR);

	float fDoorX = iter->second.front()->Get_Pos().x;
	float fDoorY = iter->second.front()->Get_Pos().y;

	//플레이어 포스 바꾸기

	NULL_CHECK(m_mapCurStageCube);
}

void CCubeMgr::ResetCubePos()
{
	SCENEID eID = *CCameraObserver::GetInstance()->GetSceneID();

	if (eID == SCENEID::SC_LOGO || eID == SCENEID::SC_TUTORIAL || eID == SCENEID::SC_TUTORIAL_THREAD)
		return;

	//그냥 큐브 원래 위치로 모두 이동
	for (auto& MyMap : *m_mapCurStageCube)
	{
		for (auto& MyCube : MyMap.second)
		{
			if (dynamic_cast<CCubeBalloon*>(MyCube))
			{
				dynamic_cast<CCubeBalloon*>(MyCube)->SetBalloonOriginPos();
			}
			else if (dynamic_cast<CCubeLaser*>(MyCube))
				continue;
			else
			{
				MyCube->SetOriginPos();
			}
		}
	}

	//오브젝트들 리셋(먹어서 죽은 것들도 있으므로)

	for (auto& MyMap : *m_mapCurStageObj)
	{
		for (auto& MyObj : MyMap.second)
		{
			MyObj->ResetObject();
		}
	}

	//switch (eID)
	//{
	//case SC_LOGO:
	//	break;
	//case SC_TUTORIAL_THREAD:
	//	break;
	//case SC_TUTORIAL:
	//	break;
	//case SC_STAGE:

	//	for (auto& MyMap : m_mapStage1ObjList)
	//	{
	//		for (auto& MyObj : MyMap.second)
	//		{
	//			Engine::Safe_Delete(MyObj);
	//		}
	//		MyMap.second.clear();
	//	}
	//	m_mapStage1ObjList.clear();

	//	Load_Obj(m_pGraphicDev, L"../../Data/Obj_Stage1.objdat", eID);
	//	m_mapCurStageObj = &m_mapStage1ObjList;
	//	break;
	//case SC_STAGE2:
	//	for (auto& MyMap : m_mapStage2ObjList)
	//	{
	//		for (auto& MyObj : MyMap.second)
	//		{
	//			Engine::Safe_Delete(MyObj);
	//		}
	//		MyMap.second.clear();
	//	}
	//	m_mapStage2ObjList.clear();

	//	Load_Obj(m_pGraphicDev, L"../../Data/Obj_Stage2.objdat", eID);
	//	m_mapCurStageObj = &m_mapStage2ObjList;
	//	break;
	//case SC_STAGE3:
	//	for (auto& MyMap : m_mapStage3ObjList)
	//	{
	//		for (auto& MyObj : MyMap.second)
	//		{
	//			Engine::Safe_Delete(MyObj);
	//		}
	//		MyMap.second.clear();
	//	}
	//	m_mapStage3ObjList.clear();

	//	Load_Obj(m_pGraphicDev, L"../../Data/Obj_Stage3.objdat", eID);
	//	m_mapCurStageObj = &m_mapStage3ObjList;
	//	break;
	//case SC_END:
	//	break;
	//default:
	//	break;
	//}

}

void CCubeMgr::CreateStage3Cube(void)
{
	Engine::CUBE_INFO tCubeInfo;
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 0.f;
	tCubeInfo.vPos.y = 0.f;
	tCubeInfo.vPos.z = 0.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;
	for (int i = 0; i < 10; ++i)
	{
		tCubeInfo.vPos.x = 1.7f * i;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = 1.7 * j;
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}


	for (int i = 0; i < 2; ++i)
	{
		tCubeInfo.vPos.x = 1.7f * i + 11.5f * 1.7f;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = 1.7 * j;
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = 1.7f * i + 15.5f * 1.7f;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = 1.7 * j;
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	//첫번쨰 칼라큐브
	for (int i = 0; i < 1; ++i)
	{
		lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
		tCubeInfo.tTex.iIndex = 1;

		tCubeInfo.vPos.x = 1.7f * i + 14.3f * 1.7f;
		tCubeInfo.vPos.y = 3.f;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = 1.7 * j;
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_BRICK;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_RED;
			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}
	//첫번째 오른쪽 밑 블럭
	for (int i = 0; i < 3; ++i)
	{
		lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
		tCubeInfo.tTex.iIndex = 0;

		tCubeInfo.vPos.x = 1.7f * i + 30.f;
		tCubeInfo.vPos.y = -5.f;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = -3.f + ((-1.7f) *(j + 1));
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;
			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	//오른쪽 대각선 블럭 앞 처음 브릭큐브
	for (int i = 0; i < 2; ++i)
	{
		lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
		tCubeInfo.tTex.iIndex = 1;

		tCubeInfo.vPos.x = 1.7f * i + 38.f;
		tCubeInfo.vPos.y = -3.8f;
		for (int j = 0; j < 2; ++j)
		{
			tCubeInfo.vPos.z = -1.f + ((-1.7f) *(j + 1));
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_SKYBLUE;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_BRICK;
			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	// 앞 두번째 플랫폼큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	tCubeInfo.tPlatformInfo.fDist = 8.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 6.0f;
	tCubeInfo.vPos.x = 1.7f + 44.5f;
	tCubeInfo.vPos.y = -2.8f;
	tCubeInfo.vScale.x = 2.2f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 2.2f;

	tCubeInfo.vPos.z = 2.f + ((-1.7f));
	//x += 0.0001f;
	//y += 0.0001f;

	CUBEID eCubeId;
	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_PINK;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	Engine::CCube*   pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}

	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}


	//오른쪽 대각선 블럭 앞 세번째 브릭큐브
	for (int i = 0; i < 2; ++i)
	{
		lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
		tCubeInfo.tTex.iIndex = 1;

		tCubeInfo.vPos.x = 1.7f * i + 60.5f;
		tCubeInfo.vPos.y = -1.8f;
		for (int j = 0; j < 2; ++j)
		{
			tCubeInfo.vPos.z = 5.f + ((-1.7f) *(j + 1));
			//x += 0.0001f;
			//y += 0.0001f;

			CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_GREEN;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_BRICK;
			Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	// 브릭큐브 앞 네번째 위아래로 플랫폼큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_DOWN;
	tCubeInfo.tPlatformInfo.fDist = 6.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 6.0f;
	tCubeInfo.vPos.x = 1.7f + 66.5f;
	tCubeInfo.vPos.y = -2.0f;
	tCubeInfo.vScale.x = 1.5f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 3.2f;

	tCubeInfo.vPos.z = 5.f + ((-1.7f));
	//x += 0.0001f;
	//y += 0.0001f;

	
	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_PURPLE;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}

	// 위아래 플랫폼 큐드 다음 다섯번째  앞뒤로 플랫폼큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	tCubeInfo.tPlatformInfo.fDist = 7.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 7.0f;
	tCubeInfo.vPos.x = 1.7f + 74.5f;
	tCubeInfo.vPos.y = -9.0f;
	tCubeInfo.vScale.x = 4.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.0f;

	tCubeInfo.vPos.z = 8.f + ((-1.7f));
	//x += 0.0001f;
	//y += 0.0001f;


	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_BLUE;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}

	// 위아래 플랫폼 큐드 다음 다섯번째  앞뒤로 플랫폼큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_LEFT;
	tCubeInfo.tPlatformInfo.fDist = 7.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 7.0f;
	tCubeInfo.vPos.x = 1.7f + 81.5f;
	tCubeInfo.vPos.y = -9.0f;
	tCubeInfo.vScale.x = 4.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.0f;

	tCubeInfo.vPos.z = 2.f + ((-1.7f));
	//x += 0.0001f;
	//y += 0.0001f;


	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_YELLOW;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}

	// 쌍둥이 앞뒤 플랫폼 큐브 다음 여섯번째  앞뒤로 플랫폼큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	tCubeInfo.tPlatformInfo.fDist = 7.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 5.0f;
	tCubeInfo.vPos.x = 1.7f + 90.5f;
	tCubeInfo.vPos.y = -9.0f;
	tCubeInfo.vScale.x = 4.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.0f;

	tCubeInfo.vPos.z = -1.0f + ((-1.7f));
	//x += 0.0001f;
	//y += 0.0001f;


	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_RED;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}

	// 앞뒤로 플랫폼 큐브 밑 일곱번째 검정색 데코 큐브
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 0.f;
	tCubeInfo.vPos.y = -13.f;
	tCubeInfo.vPos.z = 0.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.f;
	for (int i = 0; i < 23; ++i)
	{
		tCubeInfo.vPos.x = 93.5f - 1.7f * i;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = 1.7 * j - 13.0f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	// 7번째 데코 큐브 멀리  8번째 레이저 큐브 3개
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeObstacle_Laser");
	tCubeInfo.tTex.iIndex = 1;
	tCubeInfo.vPos.x = 0.f;
	tCubeInfo.vPos.y = -11.f;
	tCubeInfo.vPos.z = 0.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_LEFT;
	tCubeInfo.tPlatformInfo.fDist = 1.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 1.0f;
	
	//tCubeInfo.vPos.x = 54.4f;
	tCubeInfo.vPos.x = 56.4f;

	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.z = 1.7 * j - 13.0f;

		if(j == 0)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_YELLOW;
		if (j == 1)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_PINK;
		if (j == 2)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_SKYBLUE;

		eCubeId = CUBEID::CUBE_LASER;
		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}
		pCube->SetMaterial(BLACK);

		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}



	// 레이저 큐브 뒤 9번째 검정색 데코 큐브 3개
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 56.4f;
	tCubeInfo.vPos.y = -11.f;
	tCubeInfo.vPos.z = 0.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.f;

	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.z = 1.7 * j - 13.0f;
		tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
		//tCubeInfo.
		eCubeId = CUBEID::CUBE_DECO;
		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}
		pCube->SetMaterial(BLACK);

		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);

	}
	
	/*
		// 레이저 큐브 뒤 9번째 검정색 데코 큐브 3개
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 0.f;
	tCubeInfo.vPos.y = -13.f;
	tCubeInfo.vPos.z = 0.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.f;

	tCubeInfo.vPos.x = 93.5f;
	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.z = 1.7 * j - 13.0f;
	*/


	// 장애물 10번 브릭큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
	tCubeInfo.tTex.iIndex = 1;

	tCubeInfo.vPos.x = 93.5f;
	tCubeInfo.vPos.y = -11.3f;
	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.z = (-4.5f) * (j + 1) - 4.5f;
		//x += 0.0001f;
		//y += 0.0001f;

		if (j == 0)
			tCubeInfo.vPos.x = 83.5;
		else if(j==1)
			tCubeInfo.vPos.x = 90.5;
		else if (j == 2)
		{
			tCubeInfo.vPos.x = 71.5;
			tCubeInfo.vPos.z = -11.25f;
		}

		CUBEID eCubeId;
		if (j == 0)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_GREEN;
		else if (j == 1)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_YELLOW;
		else if (j == 2)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_ORANGE;
		//tCubeInfo.
		eCubeId = CUBEID::CUBE_BRICK;
		Engine::CCube*   pCube = nullptr;

		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}

		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);

	}
	

	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 68.f;
	tCubeInfo.vPos.y = -16.f;
	tCubeInfo.vPos.z = -18.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;

	// 열쇠로 향하는 검정색 데코들

	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = 68.f + 1.7f * i;
		for (int j = 0; j < 4; ++j)
		{
			tCubeInfo.vPos.z = -1.7 * j - 18.f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 60.f;
	tCubeInfo.vPos.y = -23.f;
	tCubeInfo.vPos.z = -25.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;


	//열쇠가 있는 검정색 데코
	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = 60.f + 1.7f * i;
		for (int j = 0; j < 4; ++j)
		{
			tCubeInfo.vPos.z = -1.7 * j - 30.f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	tCubeInfo.tPlatformInfo.fDist = 20.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 10.0f;
	tCubeInfo.vPos.x = 30.f;
	tCubeInfo.vPos.y = -22.0f;
	tCubeInfo.vPos.z = -34.0f;
	tCubeInfo.vScale.x = 5.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 1.0f;



	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_GREEN;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}



	//열쇠 다음 플랫폼 다음 데코
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 20.f;
	tCubeInfo.vPos.y = -25.f;
	tCubeInfo.vPos.z = -25.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;


	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = 20.f + 1.7f * i;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = -1.7 * j - 25.f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}

	//열쇠 다음 플랫폼 다음 데코 다음 플랫폼
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_DOWN;
	tCubeInfo.tPlatformInfo.fDist = 10.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 5.0f;
	tCubeInfo.vPos.x = 20.f;
	tCubeInfo.vPos.y = -27.0f;
	tCubeInfo.vPos.z = -18.0f;
	tCubeInfo.vScale.x = 2.5f;
	tCubeInfo.vScale.y = 1.5f;
	tCubeInfo.vScale.z = 1.0f;



	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_PINK;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}


	tCubeInfo.vPos.x = 20.f;
	tCubeInfo.vPos.y = -27.0f;
	tCubeInfo.vPos.z = -18.0f;

	//열쇠 다음 플랫폼 다음 데코
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 17.f;
	tCubeInfo.vPos.y = -37.f;
	tCubeInfo.vPos.z = -17.f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;


	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = 12.f + 1.7f * i;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = -1.7 * j - 15.f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}


	//플랫폼
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	tCubeInfo.tPlatformInfo.fDist = 50.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 12.0f;
	tCubeInfo.vPos.x = -46.f;
	tCubeInfo.vPos.y = -37.0f;
	tCubeInfo.vPos.z = -17.0f;
	tCubeInfo.vScale.x = 3.f;
	tCubeInfo.vScale.y = 1.5f;
	tCubeInfo.vScale.z = 1.0f;



	tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_BLUE;
	//tCubeInfo.
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	switch (eCubeId)
	{
	case CUBEID::CUBE_DECO:
		pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BRICK:
		pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_CRATE:
		pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_PLATFORM:
		pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SKULL:
		pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROPE:
		pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_LASER:
		pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_SPIKES:
		pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_STAIRS:
		pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_ROCK:
		pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
		break;
	case CUBEID::CUBE_BALLOON:
		pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
		break;
	}

	switch (tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(BLACK);
		break;
	}
	{
		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);
	}

	


	// 장애물 10번 브릭큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
	tCubeInfo.tTex.iIndex = 1;
	tCubeInfo.vPos.x = -46.0f;
	tCubeInfo.vPos.y = -34.5f;
	tCubeInfo.vPos.z = -17.0f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;

	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.x = (-8.5f) * (j + 1) - 22.0f;

		CUBEID eCubeId;
		if (j == 0)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_GREEN;
		else if (j == 1)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_YELLOW;
		else if (j == 2)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_ORANGE;
		//tCubeInfo.
		eCubeId = CUBEID::CUBE_BRICK;
		Engine::CCube*   pCube = nullptr;

		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}

		auto& iter = m_mapStage3CubeList.find(eCubeId);

		if (iter == m_mapStage3CubeList.end())
			m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

		m_mapStage3CubeList[eCubeId].push_back(pCube);

	}

	// 몰라
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = -50.0f;
	tCubeInfo.vPos.y = -37.5f;
	tCubeInfo.vPos.z = -17.0f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;


	for (int i = 0; i < 3; ++i)
	{
		tCubeInfo.vPos.x = -50.0f - 1.7f * i;
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = +1.7 * j - 17.f;
			//x += 0.0001f;
			//y += 0.0001f;

			//CUBEID eCubeId;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			//tCubeInfo.
			eCubeId = CUBEID::CUBE_DECO;

			//Engine::CCube*   pCube = nullptr;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);

			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);

		}
	}


	/*
	// 장애물 10번 브릭큐브
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
	tCubeInfo.tTex.iIndex = 1;
	tCubeInfo.vPos.x = -46.0f;
	tCubeInfo.vPos.y = -34.5f;
	tCubeInfo.vPos.z = -17.0f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;

	for (int j = 0; j < 3; ++j)
	{
		tCubeInfo.vPos.x = (-8.5f) * (j + 1) - 22.0f;

		CUBEID eCubeId;
		if (j == 0)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_GREEN;
		else if (j == 1)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_YELLOW;
		else if (j == 2)
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_ORANGE;
		//tCubeInfo.
	*/


	// 마지막 전전 플랫폼
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");
	tCubeInfo.tTex.iIndex = 1;
	//tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_RIGHT;
	//tCubeInfo.tPlatformInfo.fDist = 5.0f;
	//tCubeInfo.tPlatformInfo.fSpeed = 7.0f;
	tCubeInfo.vPos.x = -46.f;
	tCubeInfo.vPos.y = -37.0f;
	tCubeInfo.vPos.z = -17.0f;
	tCubeInfo.vScale.x = 3.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 3.0f;

	eCubeId = CUBEID::CUBE_BRICK;
	pCube = nullptr;

	for (int i = 0; i < 5; ++i)
	{
		//0 
		if (i == 0)
		{
			tCubeInfo.vPos.x = -58.f;
			tCubeInfo.vPos.y = -37.0f;
			tCubeInfo.vPos.z = (6.0f* 1 - 14.0f);
		}
		//1
		else if (i == 1)
		{
			tCubeInfo.vPos.x = -64.f;
			tCubeInfo.vPos.y = -37.0f + (1.0f);
			tCubeInfo.vPos.z = (6.0f*(2.0f)) - 14.0f;
		}
		//2
	/*	else if (i == 2)
		{
			tCubeInfo.vPos.x = -70.f;
		}*/
		//3
		else if (i == 2)
		{
			tCubeInfo.vPos.x = -66.f;
			tCubeInfo.vPos.y = -37.0f + (3.0f);
			tCubeInfo.vPos.z = (6.0f*(4.0f)) - 14.0f;
		}
		//4
	/*	else if (i == 4)
		{
			tCubeInfo.vPos.x = -61.f;
		}*/
		//5
		else if (i == 3)
		{
			tCubeInfo.vPos.x = -67.f;
			tCubeInfo.vPos.y = -37.0f + (5.0f);
			tCubeInfo.vPos.z = (6.0f*(6.0f)) - 14.0f;
		}
		//6
		else if (i == 4)
		{
			tCubeInfo.vPos.x = -64.f;
			tCubeInfo.vPos.y = -37.0f + (6.0f);
			tCubeInfo.vPos.z = (6.0f*(7.0f)) - 14.0f;
		}
		/*else if (i == 7)
		{
			tCubeInfo.vPos.x = -58.f;
		}*/
		//else if (i == 8)
		//{
		//	tCubeInfo.vPos.x = -53.f;
		//	tCubeInfo.vPos.y = -30.0f - (1.0f);

		//}
		//else if (i == 9)
		//{
		//	tCubeInfo.vPos.x = -47.f;
		//	tCubeInfo.vPos.y = -30.0f - (2.0f);

		//}
		//else if (i == 10)
		//{
		//	tCubeInfo.vPos.x = -51.f;
		//	tCubeInfo.vPos.y = -30.0f - (3.0f);
		//	tCubeInfo.vPos.z = 56.0f - 14.0f*1;


		//}
		//8 (48.0f) - 14.0f

		//tCubeInfo.vPos.y = -37.0f + 7.0f;
	/*	else if (i == 5)
		{
			tCubeInfo.vPos.x = -55.f;
			tCubeInfo.vPos.y = -37.0f + 6.0f;
			tCubeInfo.vPos.z = (6.0f*(8.0f)) - 14.0f;
		}*/

		/*else if (i == 6)
		{

			tCubeInfo.vPos.x = -48.f;
			tCubeInfo.vPos.y = -37.0f - (2.0f);
			tCubeInfo.vPos.z =  (48.0f) - 10.0f;;
		}*/


		//tCubeInfo.vPos.x = -58.f;
		//tCubeInfo.vPos.z = (4.0f*(i+1) ) - 14.0f;
		//tCubeInfo.vPos.y = -37.0f + (1.0f * i);
		//tCubeInfo.tPlatformInfo.fDist = 5.0f - (float)i;

		int iRandColor = (rand() % 8);
		tCubeInfo.tColor = (Engine::CUBE_COLOR)iRandColor;

		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}
		{
			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);
		}
	}

	// 마지막 전 플랫폼
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeColor_Platform");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR::CUBE_LEFT;
	tCubeInfo.tPlatformInfo.fDist = 5.0f;
	tCubeInfo.tPlatformInfo.fSpeed = 4.0f;
	tCubeInfo.vScale.x = 2.0f;
	tCubeInfo.vScale.y = 1.0f;
	tCubeInfo.vScale.z = 3.0f;

	//tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_BLUE;
	eCubeId = CUBEID::CUBE_PLATFORM;
	pCube = nullptr;

	for (int i = 0; i < 3; ++i)
	{
		int iRandColor = (rand() % 8);
		tCubeInfo.tColor = (Engine::CUBE_COLOR)iRandColor;
		// 2
		if (i == 0)
		{
			tCubeInfo.vPos.x = -70.f;
			tCubeInfo.vPos.y = -37.0f + (2.0f);
			tCubeInfo.vPos.z = (16.0f) - 14.0f;
		}
		// 4 
		else if (i == 1)
		{
			tCubeInfo.vPos.x = -61.f;
			tCubeInfo.vPos.y = -37.0f + (4.0f);
			tCubeInfo.vPos.z = (30.0f) - 14.0f;
		}
		// 7
		else if (i == 2)
		{
			tCubeInfo.vPos.x = -58.f;
			tCubeInfo.vPos.y = -37.0f + 4.0f;
			tCubeInfo.vPos.z = (48.0f) - 14.0f;
		}
		tCubeInfo.tPlatformInfo.fDist = 5.0f + (float)i;

		switch (eCubeId)
		{
		case CUBEID::CUBE_DECO:
			pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BRICK:
			pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_CRATE:
			pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_PLATFORM:
			pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SKULL:
			pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROPE:
			pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_LASER:
			pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_SPIKES:
			pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_STAIRS:
			pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_ROCK:
			pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
			break;
		case CUBEID::CUBE_BALLOON:
			pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
			break;
		}

		switch (tCubeInfo.tColor)
		{
		case Engine::CUBE_COLOR::CUBE_RED:
			pCube->SetMaterial(RED);
			break;
		case Engine::CUBE_COLOR::CUBE_ORANGE:
			pCube->SetMaterial(ORANGE);
			break;
		case Engine::CUBE_COLOR::CUBE_GREEN:
			pCube->SetMaterial(GREEN);
			break;
		case Engine::CUBE_COLOR::CUBE_YELLOW:
			pCube->SetMaterial(YELLOW);
			break;
		case Engine::CUBE_COLOR::CUBE_BLUE:
			pCube->SetMaterial(BLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_SKYBLUE:
			pCube->SetMaterial(SKYBLUE);
			break;
		case Engine::CUBE_COLOR::CUBE_PINK:
			pCube->SetMaterial(PINK);
			break;
		case Engine::CUBE_COLOR::CUBE_PURPLE:
			pCube->SetMaterial(PURPLE);
			break;
		case Engine::CUBE_COLOR::CUBE_COLOR_END:
			pCube->SetMaterial(BLACK);
			break;
		}
		{
			auto& iter = m_mapStage3CubeList.find(eCubeId);

			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);
		}
	}

	// 마지막 데코
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = -50.0f;
	tCubeInfo.vPos.y = -37.5f;
	tCubeInfo.vPos.z = 32.0f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;

	for (int i = 0; i < 25; ++i)
	{
		tCubeInfo.vPos.x = - 50.0f + (1.7f * i);
		for (int j = 0; j < 3; ++j)
		{
			tCubeInfo.vPos.z = (1.7 * j) + 32.0f;
			tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
			eCubeId = CUBEID::CUBE_DECO;

			switch (eCubeId)
			{
			case CUBEID::CUBE_DECO:
				pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BRICK:
				pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_CRATE:
				pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_PLATFORM:
				pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SKULL:
				pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROPE:
				pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_LASER:
				pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_SPIKES:
				pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_STAIRS:
				pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_ROCK:
				pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
				break;
			case CUBEID::CUBE_BALLOON:
				pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
				break;
			}

			switch (tCubeInfo.tColor)
			{
			case Engine::CUBE_COLOR::CUBE_RED:
				pCube->SetMaterial(RED);
				break;
			case Engine::CUBE_COLOR::CUBE_ORANGE:
				pCube->SetMaterial(ORANGE);
				break;
			case Engine::CUBE_COLOR::CUBE_GREEN:
				pCube->SetMaterial(GREEN);
				break;
			case Engine::CUBE_COLOR::CUBE_YELLOW:
				pCube->SetMaterial(YELLOW);
				break;
			case Engine::CUBE_COLOR::CUBE_BLUE:
				pCube->SetMaterial(BLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_SKYBLUE:
				pCube->SetMaterial(SKYBLUE);
				break;
			case Engine::CUBE_COLOR::CUBE_PINK:
				pCube->SetMaterial(PINK);
				break;
			case Engine::CUBE_COLOR::CUBE_PURPLE:
				pCube->SetMaterial(PURPLE);
				break;
			case Engine::CUBE_COLOR::CUBE_COLOR_END:
				pCube->SetMaterial(BLACK);
				break;
			}
			pCube->SetMaterial(BLACK);
			auto& iter = m_mapStage3CubeList.find(eCubeId);
			if (iter == m_mapStage3CubeList.end())
				m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

			m_mapStage3CubeList[eCubeId].push_back(pCube);
		}
	}





	// 마지막 마지막 데코 
	tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(tCubeInfo.tTex.szTex, L"Texture_CubeDeco_Aztec");
	tCubeInfo.tTex.iIndex = 0;
	tCubeInfo.vPos.x = 15.0f;
	tCubeInfo.vPos.y = -37.5f;
	tCubeInfo.vPos.z = 32.0f;
	tCubeInfo.vScale.x = 1.f;
	tCubeInfo.vScale.y = 1.f;
	tCubeInfo.vScale.z = 1.f;

	for (int i = 0; i < 15; ++i)
	{
		//9먼저                                             
		tCubeInfo.vPos.y = (1.7 * i) - 37.5f;

		for (int j = 0; j < 15; ++j)
		{
			if ((j == 1 && i == 4) || (j == 2 && i == 4) || (j == 3 && i == 4) || (j == 4 && i == 4) || (j == 5 && i == 4) || (j == 5 && i == 5) || (j == 5 && i == 6) || (j == 5 && i == 7)
				|| (j == 5 && i == 8) || (j == 5 && i == 9) || (j == 5 && i == 10) || (j == 5 && i == 11) || (j == 5 && i == 12) || (j == 5 && i == 13) || (j == 4 && i == 13)
				|| (j == 3 && i == 13) || (j == 2 && i == 13) || (j == 1 && i == 13) || (j == 1 && i == 12) || (j == 1 && i == 11) || (j == 1 && i == 10) || (j == 1 && i == 9) || (j == 2 && i == 9)
				|| (j == 3 && i == 9) || (j == 4 && i == 9)
				|| (j == 14 && i == 4) || (j == 14 && i == 5) || (j == 14 && i == 6) || (j == 14 && i == 7) || (j == 14 && i == 8) || (j == 14 && i == 9) || (j == 14 && i == 10)
				|| (j == 14 && i == 11) || (j == 14 && i == 12) || (j == 14 && i == 13) || (j == 13 && i == 13) || (j == 12 && i == 13) || (j == 11 && i == 13)
				|| (j == 10 && i == 13) || (j == 10 && i == 12) || (j == 10 && i == 11) || (j == 10 && i == 10) || (j == 10 && i == 9))

			{
				tCubeInfo.vPos.z = 48.8 - 1.7 * j;
				tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
				eCubeId = CUBEID::CUBE_DECO;

				switch (eCubeId)
				{
				case CUBEID::CUBE_DECO:
					pCube = CCubeDeco::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_BRICK:
					pCube = CCubeBrick::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_CRATE:
					pCube = CCubeCrate::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_PLATFORM:
					pCube = CCubePlatform::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_SKULL:
					pCube = CCubeSkull::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_ROPE:
					pCube = CCubeRope::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_LASER:
					pCube = CCubeLaser::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_SPIKES:
					pCube = CCubeSpikes::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_STAIRS:
					pCube = CCubeStairs::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_ROCK:
					pCube = CCubeRock::Create(m_pGraphicDev, tCubeInfo);
					break;
				case CUBEID::CUBE_BALLOON:
					pCube = CCubeBalloon::Create(m_pGraphicDev, tCubeInfo);
					break;
				}

				switch (tCubeInfo.tColor)
				{
				case Engine::CUBE_COLOR::CUBE_RED:
					pCube->SetMaterial(RED);
					break;
				case Engine::CUBE_COLOR::CUBE_ORANGE:
					pCube->SetMaterial(ORANGE);
					break;
				case Engine::CUBE_COLOR::CUBE_GREEN:
					pCube->SetMaterial(GREEN);
					break;
				case Engine::CUBE_COLOR::CUBE_YELLOW:
					pCube->SetMaterial(YELLOW);
					break;
				case Engine::CUBE_COLOR::CUBE_BLUE:
					pCube->SetMaterial(BLUE);
					break;
				case Engine::CUBE_COLOR::CUBE_SKYBLUE:
					pCube->SetMaterial(SKYBLUE);
					break;
				case Engine::CUBE_COLOR::CUBE_PINK:
					pCube->SetMaterial(PINK);
					break;
				case Engine::CUBE_COLOR::CUBE_PURPLE:
					pCube->SetMaterial(PURPLE);
					break;
				case Engine::CUBE_COLOR::CUBE_COLOR_END:
					pCube->SetMaterial(BLACK);
					break;
				}
				pCube->SetMaterial(BLACK);
				dynamic_cast<CCubeDeco*>(pCube)->SetEndingCube();
				auto& iter = m_mapStage3CubeList.find(eCubeId);
				if (iter == m_mapStage3CubeList.end())
					m_mapStage3CubeList[eCubeId] = list<Engine::CCube*>();

				m_mapStage3CubeList[eCubeId].push_back(pCube);
			}
		}
	}

}

void CCubeMgr::CreateStage3Object(void)
{

	Engine::OBJ_INFO tObjInfo;
	OBJID eObjId;
	for (int i = 0; i < 4; ++i)
	{

		if (i == 0)		//그냥도어
		{
			eObjId = OBJID::OBJ_DOOR;
			tObjInfo.vPos.x = 1.f;
			tObjInfo.vPos.y = 2.2f;
			tObjInfo.vPos.z = 3.f;

			CUBEID eCubeId;
			lstrcpy(tObjInfo.tTex.szTex, L"Texture_Object_Door");
			tObjInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
			tObjInfo.tTex.iIndex = 0;
		}
		else if (i == 1)	//락도어
		{
			eObjId = OBJID::OBJ_DOORLOCK;
			tObjInfo.vPos.x = 65.f;
			tObjInfo.vPos.y = -17.f;
			tObjInfo.vPos.z = -30.f;

			CUBEID eCubeId;
			lstrcpy(tObjInfo.tTex.szTex, L"Texture_Object_DoorLock");
			tObjInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
			tObjInfo.tTex.iIndex = 0;
		}
		else if (i == 2)		//키
		{
			eObjId = OBJID::OBJ_SAVEPOINT;
			tObjInfo.vPos.x = 62.f;
			tObjInfo.vPos.y = -20.f;
			tObjInfo.vPos.z = -35.f;

			/*tObjInfo.vPos.x = 0.f;
			tObjInfo.vPos.y = -0;
			tObjInfo.vPos.z = -5.f;*/


			CUBEID eCubeId;
			lstrcpy(tObjInfo.tTex.szTex, L"Texture_Object_SavePoint");
			tObjInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
			tObjInfo.tTex.iIndex = 0;
		}
		else if (i == 3)		//어려운 
		{
			eObjId = OBJID::OBJ_SAVEPOINT;
			tObjInfo.vPos.x = -51.f;
			tObjInfo.vPos.y = -35.f;
			tObjInfo.vPos.z =  -15.5f;
			/*tObjInfo.vPos.x = 0.f;
			tObjInfo.vPos.y = -0;
			tObjInfo.vPos.z = -5.f;*/


			CUBEID eCubeId;
			lstrcpy(tObjInfo.tTex.szTex, L"Texture_Object_SavePoint");
			tObjInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
			tObjInfo.tTex.iIndex = 0;
		}
		



		CObjBasic*   pObj = nullptr;

		switch (eObjId)
		{
		case OBJID::OBJ_DOOR:
			pObj = CDoor::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_DOORLOCK:
			pObj = CDoor::Create(m_pGraphicDev, tObjInfo);
			dynamic_cast<CDoor*>(pObj)->Set_Lock();
			dynamic_cast<CDoor*>(pObj)->SetEndDoor();
			break;
		case OBJID::OBJ_KEY:
			pObj = CKey::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_BATTERY:

			pObj = CBattery::Create(m_pGraphicDev, tObjInfo);
			break;
		case OBJID::OBJ_SAVEPOINT:
			pObj = CSavePoint::Create(m_pGraphicDev, tObjInfo);
			break;
		}
		int eColor = rand() % 8;

		switch (eColor)
		{
		case 0:
			pObj->SetMaterial(RED);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_RED);
			break;
		case 1:
			pObj->SetMaterial(ORANGE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_ORANGE);
			break;
		case 2:
			pObj->SetMaterial(GREEN);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_GREEN);
			break;
		case 3:
			pObj->SetMaterial(YELLOW);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_YELLOW);
			break;
		case 4:
			pObj->SetMaterial(BLUE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_BLUE);
			break;
		case 5:
			pObj->SetMaterial(SKYBLUE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_SKYBLUE);
			break;
		case 6:
			pObj->SetMaterial(PINK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_PINK);
			break;
		case 7:
			pObj->SetMaterial(PURPLE);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_PURPLE);
			break;
		case 8:
			pObj->SetMaterial(BLACK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
			break;
		}

		if (eObjId == OBJID::OBJ_DECO || eObjId == OBJID::OBJ_SAVEPOINT)
		{
			pObj->SetMaterial(BLACK);
			pObj->Set_CubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
		}


		auto& iter = m_mapStage3ObjList.find(eObjId);

		if (iter == m_mapStage3ObjList.end())
			m_mapStage3ObjList[eObjId] = list<CObjBasic*>();

		m_mapStage3ObjList[eObjId].push_back(pObj);
	}
}

void CCubeMgr::UnlockDoor()
{
	auto& iter = m_mapCurStageObj->find(OBJID::OBJ_DOORLOCK);

	dynamic_cast<CDoor*>(iter->second.front())->SetDoorLock(false);
}