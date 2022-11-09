#include "stdafx.h"
#include "TutoCamera.h"

#include "Export_Function.h"
#include "CameraObserver.h"
#include "Include.h"
#include "Transform.h"
#include "KeyMgr.h"
#include "Player.h"
#include "Door.h"
#include "Key.h"

USING(Engine)

CTutoCamera::CTutoCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
	, m_pInfoSubject(Engine::Get_InfoSubject())
	, m_pKeyMgr(Engine::Get_KeyMgr())
	, m_pCameraObserver(CCameraObserver::GetInstance())
{

}

CTutoCamera::~CTutoCamera(void)
{
}

HRESULT CTutoCamera::Initialize(void)
{
	Engine::Get_InfoSubject()->AddData(D3DTS_VIEW, &m_matView);
	Engine::Get_InfoSubject()->AddData(D3DTS_PROJECTION, &m_matProj);
	Engine::Get_InfoSubject()->AddData(DATA_NARRATIVE_MOVEING, &m_bNarrativeMoveing);
	Engine::Get_InfoSubject()->AddData(DATA_NARRATIVE_VIEW, &m_eNarrativeView);
	Engine::Get_InfoSubject()->AddData(DATA_TUTOCAMERA, this);

	BasicCameraDefaultInit();

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	
	return S_OK;
}

void CTutoCamera::Update(void)
{
	TargetRenewal();

	m_pInfoSubject->Notify(D3DTS_VIEW);
	m_pInfoSubject->Notify(D3DTS_PROJECTION);
	m_pInfoSubject->Notify(DATA_NARRATIVE_MOVEING);
	m_pInfoSubject->Notify(DATA_NARRATIVE_VIEW);
}

void CTutoCamera::LateUpdate(void)
{
}

void CTutoCamera::TargetRenewal(void)
{
	if (m_pTargetInfo)
	{
		//D3DXVECTOR3 vDir = { 0.f, 0.f, 10.f };
		//m_vEye = vDir * (-1.f);
		//D3DXVec3Normalize(&m_vEye, &m_vEye);

		//m_vEye *= m_fDistance;

		//D3DXVECTOR3	vRight;
		//memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);//

		//m_fAngle = 0.f;

		//D3DXMATRIX	matRotAxis;//
		//D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);//
		//D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);//

		//m_vEye += m_pTargetInfo->m_vPos;
		//m_vAt = m_pTargetInfo->m_vPos;

		D3DXVECTOR3 vDir = m_pTargetInfo->m_vPos - m_vEye;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vEye.x += vDir.x * 20.f * GET_TIME;
		m_vEye.y += vDir.y * 4.f  * GET_TIME;

		m_vAt = m_pTargetInfo->m_vPos;

		//m_vEye.x = m_pTargetInfo->m_vPos.x;
		//m_vEye.y = m_pTargetInfo->m_vPos.y;
		
		CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}

CTutoCamera* CTutoCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::CTransform* pTargetInfo)
{
	CTutoCamera*	pInstance = new CTutoCamera(pGraphicDev);

	pInstance->SetTarget(pTargetInfo);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	pInstance->SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	return pInstance;
}

void CTutoCamera::SetTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

void CTutoCamera::BasicCameraDefaultInit()
{
	if (nullptr != m_pTargetInfo)
	{
		D3DXVECTOR3 vDir = { 0.f, 0.f, 10.f };
		m_vEye = vDir * (-1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);

		m_vEye *= m_fDistance;

		D3DXVECTOR3	vRight;
		memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);//

		m_fAngle = 0.f;

		D3DXMATRIX	matRotAxis;//
		D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);//
		D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);//

		m_vEye += m_pTargetInfo->m_vPos;
		m_vAt = m_pTargetInfo->m_vPos;

		m_vEye.x = 0.f;
		m_vAt.x = 0.f;
		m_vAt.x = 0.f;
		m_vAt.y = 0.f;
		
		CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}