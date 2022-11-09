#include "Obj.h"
#include "Export_Function.h"
USING(Engine)

Engine::CObj::CObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO& _eCubeInfo)
	: Engine::CGameObject(pGraphicDev)
{
	m_tObjInfo = _eCubeInfo;
}

Engine::CObj::~CObj(void)
{
	Release();
}

HRESULT Engine::CObj::Initialize(void)
{

	return S_OK;
}

void Engine::CObj::Update(void)
{
}

void CObj::LateUpdate(void)
{
}

void Engine::CObj::Render(void)
{
}

void CObj::RenderInsideWorld(void)
{
}

void Engine::CObj::Release(void)
{
	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVtx);
}

HRESULT CObj::AddComponent(void)
{
	return S_OK;
}

void CObj::SetMaterial(D3DXCOLOR tColor)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	m_tMtrl.Ambient = tColor;
	m_tMtrl.Diffuse = tColor;
	m_tMtrl.Specular = tColor;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

CObj* Engine::CObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO& _eCubeInfo)
{
	CObj* pInstance = new CObj(pGraphicDev, _eCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);


	return pInstance;
}
