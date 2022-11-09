#include "stdafx.h"
//#include "ColorHP.h"
//#include "Include.h"
//#include "Export_Function.h"
//#include "CubeMgr.h"
//#include "UvTex.h"
//#include "Player.h"
//USING(Engine)
//
//CColorHP::CColorHP(LPDIRECT3DDEVICE9 pGraphicDev)
//	: CGameObject(pGraphicDev)
//{
//}
//
//CColorHP::~CColorHP(void)
//{
//	Release();
//}
//
//HRESULT CColorHP::Initialize(void)
//{
//	m_iCurChange = m_iMaxChange = 35;
//	m_fColorR = 0.f;
//	m_fColorG = 0.f;
//	m_fColorB = 0.f;
//	m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_RED;
//	m_pTexName = L"Texture_ColorHP";
//	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
//
//	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
//	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
//	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
//	m_tMtrl.Emissive = BLACK;
//	m_tMtrl.Power = 5.f;
//
//	m_pInfoCom->m_vPos.x = -(float)WINCX + 150.f;
//	m_pInfoCom->m_vPos.y = 0.f;
//	m_pInfoCom->m_vPos.z = 0.f;
//
//	m_vecOirgin = m_pInfoCom->m_vPos;
//
//	m_pInfoCom->m_vScale.x = 55.2f;
//	m_pInfoCom->m_vScale.y = 140.2f;
//	m_pInfoCom->m_vScale.z = 100.5f;
//	return S_OK;
//}
//
//void CColorHP::Update(void)
//{
//	if (m_iCurChange < 0 && !CCubeMgr::GetInstance()->GetPlayerDie())
//	{
//		CCubeMgr::GetInstance()->SetPlayerDie(true);
//		CCubeMgr::GetInstance()->GetPlayer()->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
//
//		Engine::Get_SoundMgr()->StopAll();
//		Engine::Get_SoundMgr()->MyPlaySound(L"DeadBySkull.wav", Engine::CHANNELID::EFFECT);
//	}
//
//	if (CCubeMgr::GetInstance()->GetResetBattery())
//	{
//		m_iCurChange = m_iMaxChange;
//		CCubeMgr::GetInstance()->SetResetBattery(false);
//	}
//
//	switch (m_eCurColorID)
//	{
//	case CBackGround::CURCOLORID::CURCOLORID_RED:
//		ColorToYellow();
//		break;
//	case CBackGround::CURCOLORID::CURCOLORID_YELLOW:
//		ColorToGreen();
//		break;
//	case CBackGround::CURCOLORID::CURCOLORID_GREEN:
//		ColorToBlue();
//		break;
//	case CBackGround::CURCOLORID::CURCOLORID_BLUE:
//		ColorToRed();
//		break;
//	case CBackGround::CURCOLORID::CURCOLOR_END:
//		break;
//	default:
//		break;
//	}
//	SetTransform();
//	m_pBufferUvTex->Update();
//
//}
//
//void CColorHP::LateUpdate(void)
//{
//}
//
//void CColorHP::Render(void)
//{
//	m_pInfoCom->m_vPos.y = m_vecOirgin.y - 30.f - ((m_iMaxChange - m_iCurChange)) * m_iMaxChange / 11.5f;
//	m_pInfoCom->m_vPos.z = 0.1f;
//	m_pInfoCom->m_vScale.x = 43.f;
//	m_pInfoCom->m_vScale.y = 108.f * ((float)m_iCurChange / m_iMaxChange);
//	m_pInfoCom->m_vScale.z = 100.5f;
//
//	SetTransform();
//	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
//
//
//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
//
//
//	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pTextureCom->SetTexture(2);
//	m_pBufferUvTex->Render();
//
//
//	m_pInfoCom->m_vPos = m_vecOirgin;
//	m_pInfoCom->m_vPos.z = 0.f;
//	m_pInfoCom->m_vScale.x = 55.2f;
//	m_pInfoCom->m_vScale.y = 140.2f;
//	m_pInfoCom->m_vScale.z = 100.5f;
//
//	SetTransform();
//	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
//
//
//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
//
//	m_pTextureCom->SetTexture(1);
//	m_pBufferCom->Render();
//
//}
//
//void CColorHP::MinusCount()
//{
//	if (SCENEID::SC_TUTORIAL != *CCameraObserver::GetInstance()->GetSceneID())
//	{
//		if (m_iCurChange >= 0) 
//			m_iCurChange -= 1; 
//	}
//}
//
//void CColorHP::PlusCount()
//{
//	m_iCurChange += 15;
//	if (m_iCurChange > m_iMaxChange)
//		m_iCurChange = m_iMaxChange;
//}
//
//void CColorHP::Release(void)
//{
//	Engine::Safe_Delete(m_pBufferUvTex);
//
//}
//
//void CColorHP::SetTransform(void)
//{
//	D3DXMATRIX matTrans, matScale, matRot;
//	D3DXMatrixIdentity(&matTrans);
//	D3DXMatrixIdentity(&matScale);
//	D3DXMatrixTranslation(&matTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z);
//	D3DXMatrixScaling(&matScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y, m_pInfoCom->m_vScale.z);
//	D3DXMatrixRotationZ(&matRot, m_pInfoCom->m_fAngle[ANGLE_Z]);
//	m_pInfoCom->m_matWorld = matScale * matRot * matTrans;
//}
//
//void CColorHP::ColorToYellow()
//{
//	if (m_fColorG - 255 > 0.f && m_fColorG != 0.f)
//	{
//		m_fColorG = 255.f;
//		m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_YELLOW;
//		m_fColorR = 255.f;
//		return;
//	}
//	m_fColorG += GET_TIME* 100.f;
//
//
//	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
//	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
//	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
//	m_tMtrl.Emissive = BLACK;
//	m_tMtrl.Power = 5.f;
//}
//
//void CColorHP::ColorToGreen()
//{
//	if ((int)m_fColorR <= 0 && m_fColorG != 0.f)
//	{
//		m_fColorG = 255.f;
//		m_fColorR = 0.f;
//		m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_GREEN;
//		return;
//	}
//	m_fColorR -= GET_TIME* 100.f;
//
//	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
//	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
//	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
//	m_tMtrl.Emissive = BLACK;
//	m_tMtrl.Power = 5.f;
//}
//
//void CColorHP::ColorToBlue()
//{
//	if (m_fColorB - 255 > 0.f && m_fColorB != 0.f)
//	{
//		m_fColorG = 255.f;
//		m_fColorB = 255.f;
//		m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_BLUE;
//		m_fColorR = 0.f;
//		m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		return;
//	}
//	m_fColorB += GET_TIME* 100.f;
//
//	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
//	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
//	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
//	m_tMtrl.Emissive = BLACK;
//	m_tMtrl.Power = 5.f;
//}
//
//void CColorHP::ColorToRed()
//{
//	if (m_fColorR - 255 > 0.f && m_fColorR != 0.f)
//	{
//		m_fColorG = 0.f;
//		m_fColorB = 0.f;
//		m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_RED;
//		m_fColorR = 255.f;
//		m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//		return;
//	}
//	m_fColorR += GET_TIME* 100.f;
//	m_fColorG -= GET_TIME* 100.f;
//	m_fColorB -= GET_TIME* 100.f;
//
//	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
//	m_tMtrl.Emissive = BLACK;
//	m_tMtrl.Power = 5.f;
//}
//
//HRESULT CColorHP::AddComponent(void)
//{
//	m_pBufferName = L"Buffer_UVTex";
//	Engine::CComponent*		pComponent = nullptr;
//
//	// Buffer
//	pComponent = Engine::Get_ResourceMgr()->Clone_UVBuffer();
//	//m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
//	m_pBufferUvTex = dynamic_cast<Engine::CUvTex*>(pComponent);
//	//m_mapComponent.emplace(L"Buffer", m_pBufferUvTex);
//
//	pComponent = Engine::Get_ResourceMgr()->Clone_Buffer(Engine::RESOURCE_STATIC, L"Buffer_RcMouse");
//	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
//	m_mapComponent.emplace(L"Buffer", m_pBufferCom);
//	// Texture
//	pComponent = Engine::Get_ResourceMgr()->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
//	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
//	m_mapComponent.emplace(L"Texture", m_pTextureCom);
//
//	// Transform
//	pComponent = Engine::CTransform::Create(g_vLook);
//	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	m_mapComponent.emplace(L"Transform", m_pInfoCom);
//
//	return S_OK;
//}
//
//CColorHP * CColorHP::Create(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	CColorHP* pInstance = new CColorHP(pGraphicDev);
//
//	if (FAILED(pInstance->Initialize()))
//		Safe_Delete(pInstance);
//
//	return pInstance;
//}
