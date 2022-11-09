#include "stdafx.h"
#include "Mouse.h"
#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CMouse::~CMouse(void)
{
	Release();
}


HRESULT CMouse::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_RcMouse";
	m_pTexName = L"Texture_Mouse";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pInfoCom->m_vScale = D3DXVECTOR3(25.f, 25.f, 10.f);
	return S_OK;
}

void CMouse::Update(void)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x -= (WINCX / 2);
	pt.y -= (WINCY / 2);
	pt.y *= -2;
	pt.x *= 2;
	m_pInfoCom->m_vPos.x = (float)pt.x;
	m_pInfoCom->m_vPos.y = (float)pt.y;
	m_pInfoCom->m_vPos.z = 0.f;

	/************************ (SongSong) ************************/
	/*  1인칭 도착했을 시 화면이 그대로보여주기 위해서 초기화  */
	if (NARRATIVE_VIEW::FIRST_PERSON == *CCameraObserver::GetInstance()->GetNarrativeView()
		&& TRUE == *CCameraObserver::GetInstance()->GetNarrativeMoving())
	{
		SetCursorPos(WINCX / 2, WINCY / 2);
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		m_pInfoCom->m_vPos.x = (float)pt.x;
		m_pInfoCom->m_vPos.y = (float)pt.y;
		m_pInfoCom->m_vPos.z = 0.f;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	//SetTransform();
}


void CMouse::LateUpdate(void)
{

}

void CMouse::Render(void)
{
	/*D3DXVECTOR3 vPos = { -(WINCX / 2 * 2.f), -(WINCY / 2) * -2.f, 0.f };
	m_pInfoCom->m_vScale = D3DXVECTOR3(25.f, 25.f, 10.f);*/

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}

void CMouse::FirstPersonRender(void)
{
	////m_pInfoCom->m_vPos.x = WINCX/2;
	////m_pInfoCom->m_vPos.y = WINCY/2;
	////m_pInfoCom->m_vPos.z = 0.f;
	///*pt.x -= (WINCX / 2);
	//pt.y -= (WINCY / 2);
	//pt.y *= -2.f;
	//pt.x *= 2.f;*/
	//D3DXVECTOR3 vPos = { -(WINCX / 2 * 2.f), -(WINCY / 2) * -2.f, 0.f };
	//D3DXVECTOR3 vScale = { 40.f, 30.f, 15.f };

	//Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
	//	&m_pInfoCom->m_vScale,
	//	m_pInfoCom->m_fAngle,
	//	&m_pInfoCom->m_vPos);

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	//m_pTextureCom->SetTexture(2);
	//m_pBufferCom->Render();
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}

void CMouse::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CMouse::Render_FirstMouse()
{
	/*D3DXVECTOR3 vPos = { -(WINCX / 2 * 2.f), -(WINCY / 2) * -2.f, 0.f };
	m_pInfoCom->m_vScale = D3DXVECTOR3(55.f, 55.f, 10.f);

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);*/

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}


void CMouse::SetTransform(void)
{
	const D3DXMATRIX*		matView = CCameraObserver::GetInstance()->GetView();
	const D3DXMATRIX*		matProj = CCameraObserver::GetInstance()->GetProj();


	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matView);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matProj);
	}

}

HRESULT CMouse::AddComponent(void)
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

CMouse * CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse* pInstance = new CMouse(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
