#include "stdafx.h"
#include "SkyBox.h"

USING(Engine)

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev, m_tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CSkyBox::~CSkyBox(void)
{
}

HRESULT CSkyBox::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;


	m_pBufferName = L"Buffer_Cube_SkyBox";
	m_pTexName = L"Texture_SkyBox";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform 컴포넌트  */
	m_pInfoCom->m_vPos = { 0.f, 0.f, 0.f };
	m_pInfoCom->m_vScale = { 130.f, 130.f, 130.f };

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CSkyBox::Update(void)
{
	if (GET_TIME < 7.f)
		m_pInfoCom->m_fAngle[Engine::ANGLETYPE::ANGLE_Y] += D3DXToRadian(5.0f) * 1.f * GET_TIME;


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

void CSkyBox::PreRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CSkyBox::Render(void)
{
	PreRender();

	m_pGraphicDev->SetMaterial(&m_tMtrl);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pTextureCom->SetTexture(DWORD(CCubeMgr::GetInstance()->Get_CurCubeColor()));
	m_pBufferCom->Render_NoIndices();

	PostRender();
}

void CSkyBox::RenderInsideWorld(void)
{
	PreRender();

	m_pGraphicDev->SetMaterial(&m_tMtrl);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pTextureCom->SetTexture(DWORD(8));
	m_pBufferCom->Render_NoIndices();

	PostRender();
}

void CSkyBox::PostRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkyBox::Release(void)
{
}

void CSkyBox::SetTransform(void)
{
	D3DXMATRIX matTrans, matScale;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixTranslation(&matTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z);
	D3DXMatrixScaling(&matScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y, m_pInfoCom->m_vScale.z);
	m_pInfoCom->m_matWorld = matScale * matTrans;
}

HRESULT CSkyBox::AddComponent(void)
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

CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
