#include "Cube.h"
#include "Export_Function.h"
#include "Transform.h"
USING(Engine)

Engine::CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_INFO& _eCubeInfo)
	: Engine::CGameObject(pGraphicDev)
	, m_tCubeInfo(_eCubeInfo)
{
	m_tCubeInfo = _eCubeInfo; 
	m_bIsMovingCube = false;
	m_vecOriginPos = m_tCubeInfo.vPos;
	m_bIsMove = true;

	m_bIsRotMinus = false;
	m_bIsRotX = true;
	m_bIsRotY = true;
	m_bIsRotZ = true;

	m_bIsSpike = false;
}

Engine::CCube::~CCube(void)
{
	Release();
}

HRESULT Engine::CCube::Initialize(void)
{

	return S_OK;
}

void Engine::CCube::Update(void)
{
}

void CCube::LateUpdate(void)
{
}

void Engine::CCube::Render(void)
{
}

void CCube::RenderInsideWorld(void)
{
}

void CCube::SwapPos()
{
	float fTemp = m_pInfoCom->m_vPos.y;
	m_pInfoCom->m_vPos.y = m_pInfoCom->m_vPos.z;
	m_pInfoCom->m_vPos.z = fTemp;
}

void Engine::CCube::Release(void)
{
	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVtx);
}

HRESULT CCube::AddComponent(void)
{
	return S_OK;
}

void CCube::SetMovingCube()
{
	int iRotRandom = rand() % 2;
	if (iRotRandom == 0)
		m_bIsRotMinus = true;
	m_bIsMovingCube = true;

	/*if (m_vecOriginPos.x < 25.f)
		m_pInfoCom->m_vPos.x = -25.f;
	else
		m_pInfoCom->m_vPos.x =  75.f;*/
	m_pInfoCom->m_vPos.x = 30.f;

	int iZ = rand() % 25;
	m_pInfoCom->m_vPos.z = -30.f - iZ;
	int iRand = rand() % 2;
	if (iRand == 0)
		m_pInfoCom->m_vPos.z *= -1.f;

	if (m_pInfoCom->m_vPos.z > 0)
	{
		int iRandX = rand() % 110;
		m_pInfoCom->m_vPos.x = iRandX - 30.f;

		int iRandY = rand() % 200;			
		m_pInfoCom->m_vPos.y = iRandY - 100.f;
	}
	else
	{
		int iRandX = rand() % 10 + 10;
		m_pInfoCom->m_vPos.x = iRandX;

		int iRandY = rand() % 10 + 10;			// 20 ~ 40
		m_pInfoCom->m_vPos.y = iRandY;
	}
	m_pInfoCom->m_vDir = m_vecOriginPos - m_pInfoCom->m_vPos;
	D3DXVec3Normalize(&m_pInfoCom->m_vDir, &m_pInfoCom->m_vDir);

}

void CCube::SetOriginPos()
{
	m_pInfoCom->m_vPos = m_vecOriginPos;
}

void CCube::FirstMovingUpdate()
{
	float fDistX = m_vecOriginPos.x - m_pInfoCom->m_vPos.x;
	float fDistY = m_vecOriginPos.y - m_pInfoCom->m_vPos.y;
	float fDistZ = m_vecOriginPos.z - m_pInfoCom->m_vPos.z;

	if (m_bIsSpike && sqrtf(fDistX * fDistX + fDistY * fDistY + fDistZ * fDistZ) < 3.f)
	{
		m_pInfoCom->m_vPos = m_vecOriginPos;
		m_bIsMove = false;
	}
	else if (!m_bIsSpike && sqrtf(fDistX * fDistX + fDistY * fDistY + fDistZ * fDistZ) < 1.5f)
	{
		m_pInfoCom->m_vPos = m_vecOriginPos;
		m_bIsMove = false;
	}

	if (Engine::Get_TimeMgr()->GetTime() < 0.7f && m_bIsMove)
	{
		m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 20.f * Engine::Get_TimeMgr()->GetTime();
		if (!m_bIsRotMinus)
		{
			m_pInfoCom->m_fAngle[ANGLE_X] += D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
			m_pInfoCom->m_fAngle[ANGLE_Y] += D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
		}
		else
		{
			m_pInfoCom->m_fAngle[ANGLE_X] -= D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
			m_pInfoCom->m_fAngle[ANGLE_Y] -= D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetTime();
		}
	}
	else if (!m_bIsMove)
	{
		if (!m_bIsRotMinus)
		{
			m_pInfoCom->m_fAngle[ANGLE_X] += D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_X]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_X] = 0.f;
				m_bIsRotX = false;
			}
			m_pInfoCom->m_fAngle[ANGLE_Y] += D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Y]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_Y] = 0.f;
				m_bIsRotY = false;
			}
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Z]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_Z] = 0.f;
				m_bIsRotZ = false;
			}
		}
		else
		{
			m_pInfoCom->m_fAngle[ANGLE_X] -= D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_X]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_X] = 0.f;
				m_bIsRotX = false;
			}
			m_pInfoCom->m_fAngle[ANGLE_Y] -= D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Y]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_Y] = 0.f;
				m_bIsRotY = false;
			}
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(60.f) * Engine::Get_TimeMgr()->GetTime();
			if ((int)D3DXToDegree(m_pInfoCom->m_fAngle[ANGLE_Z]) % 360 <= 5.f)
			{
				m_pInfoCom->m_fAngle[ANGLE_Z] = 0.f;
				m_bIsRotZ = false;
			}
		}
	}

	if (!m_bIsRotX && !m_bIsRotY && !m_bIsRotZ)
		m_bIsMovingCube = false;
	
}

void CCube::SetMaterial(D3DXCOLOR tColor)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	m_tMtrl.Ambient = tColor;
	m_tMtrl.Diffuse = tColor;
	m_tMtrl.Specular = tColor;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

CCube* Engine::CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_INFO& _eCubeInfo)
{
	CCube* pInstance = new CCube(pGraphicDev, _eCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);


	return pInstance;
}
