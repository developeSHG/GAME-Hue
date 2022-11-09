#include "stdafx.h"
#include "InsideCircle.h"
#include "CameraObserver.h"
#include "Export_Function.h"
#include "CubeMgr.h"
#include "Cube.h"
#include "Snow.h"
USING(Engine)

CInsideCircle::CInsideCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CInsideCircle::~CInsideCircle()
{
	Release();
}

HRESULT CInsideCircle::Initialize()
{

	m_tMtrl.Ambient = RED;
	m_tMtrl.Diffuse = RED;
	m_tMtrl.Specular = RED;
	m_tMtrl.Emissive = WHITE;
	m_tMtrl.Power = 5.f;


	m_pBufferName = L"Buffer_CircleCol";
	m_pTexName = L"WhiteBack";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_dwVtxCnt = 42;
	m_pVertex = new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);


	return S_OK;
}

HRESULT CInsideCircle::AddComponent(void)
{
	Engine::CComponent*      pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);

	// Transform
	pComponent = m_pInfoCom = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	// Texture
	pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"Texture", m_pTextureCom);

	return S_OK;
}

void CInsideCircle::SetTransform(void)
{
	D3DXMATRIX      matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 0.f, 10.f);

	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);

		if (m_pConvertVtx[i].vPos.z < 1.f)
			m_pConvertVtx[i].vPos.z = 1.f;

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &matProj);
	}
}

void CInsideCircle::BiggerUpdate(void)
{
	m_fScale += 450.f * GET_CONSTANTTIME;

	if (m_fScale >= 1500.f)
		m_fScale = 1500.f;


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, 1.f),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);


	SetTransform();

}

void CInsideCircle::SmallerUpdate(void)
{
	m_fScale -= 480.f * GET_CONSTANTTIME;

	if (m_fScale <= 10.f)
		m_fScale = 10.f;


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, 1.f),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CInsideCircle::Update(void)
{

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, 1.f),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CInsideCircle::LateUpdate(void)
{
}

void CInsideCircle::Render(void)
{

	if (m_fScale >= 15.f)
	{
		m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetMaterial(&m_tMtrl);
		m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
		m_pTextureCom->SetTexture(0);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pBufferCom->Render_TriFans();

		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, true);               //���ٽ� Ȱ��ȭ
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);         //���ٽ� �׽�Ʈ �׻� ����
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILREF, 0x1);                  //����, ���ٽ� ��� ������ 0x1 ����
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);      //���ٽ� ���н� ���ٽ� �� �ȹٲ۴�
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);   //z, ���ٽ� ������ �� �ٲ۴�.(������ ������ 0x1��)

		m_pBufferCom->Render_TriFans();
		m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);                                                                  //���� �ݻ�Ǵ� ť�긦 z�� Ȱ��ȭ, �ȼ����ۿ��� �׸��� ���ٽǿ��� 0x1�� ���̰� �ǹǷ� �� �� -> ���ٽ� �׽�Ʈ�� �ϴ°�
		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);               //���ٽ� ���� 0x1�϶��� ���ٽ� �׽�Ʈ ���

																														//if(SCENEID::SC_TUTORIAL == *CCameraObserver::GetInstance()->GetSceneID())
																														//   Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
																														//else
		Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.f, 0);                     //ť��� �ſ� �ڿ� �׷����� �Ⱥ��̰� �ǹǷ� z���۸� 1�� �ٽ��ʱ�ȭ��


		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		////�׸���////////////////////////////////////////////////////////////////////////////////////////////////////
		D3DXMATRIX    mIdentity;
		D3DXMatrixIdentity(&mIdentity);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mIdentity);
		//m_pHand->Render();

		/************************ (SongSong) ************************/
		/*  ���������ε� ���������� �ִ� ��ü���� �����ϱ����ؼ��� �ٽ� ����İ� ������ �����ڽ��� ������ �ѷ��ֱ����� �ٽ� ������  */
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &CCubeMgr::GetInstance()->m_View);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &CCubeMgr::GetInstance()->m_Proj);

		m_pPlayer->Render();
		if (SCENEID::SC_TUTORIAL == *CCameraObserver::GetInstance()->GetSceneID())
		{
			CCubeMgr::GetInstance()->RenderInsideWorld();

			for (auto& iter : *CCameraObserver::GetInstance()->GetlistTutoObject())
			{
				iter->RenderInsideWorld();
			}
		}
		if (SCENEID::SC_STAGE == *CCameraObserver::GetInstance()->GetSceneID()
			|| SCENEID::SC_STAGE3 == *CCameraObserver::GetInstance()->GetSceneID())
		{
			m_pSnow->RenderInsideWorld();
		}
		else if (SCENEID::SC_STAGE2 == *CCameraObserver::GetInstance()->GetSceneID())
			m_pRain->RenderInsideWorld();
		if (SCENEID::SC_TUTORIAL != *CCameraObserver::GetInstance()->GetSceneID())
		{
			CCubeMgr::GetInstance()->RenderInsideWorld();
		}


		Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, false);

	}

}

void CInsideCircle::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}


CInsideCircle* CInsideCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInsideCircle*   pInstance = new CInsideCircle(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}