#include "ResourceMgr.h"
#include "VIBuffer.h"
#include "TriCol.h"
#include "RcCol.h"
#include "TerrainCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "Texture.h"
#include "CubeCol.h"
#include "CubeTex.h"
#include "RcyTex.h"
#include "CircleCol.h"
#include "UvTex.h"
#include "RcMouse.h"
USING(Engine)

IMPLEMENT_SINGLETON(CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
{
}

Engine::CResourceMgr::~CResourceMgr(void)
{
	Release();
}

void CResourceMgr::CopyToClient(RESOURCETYPE eResourceType, const wstring & wstrResourceKey, void * pVertex)
{
	auto& iter = m_mapResource[eResourceType].find(wstrResourceKey);
	if (iter == m_mapResource[eResourceType].end())
		TAGMSG_BOX(wstrResourceKey.c_str(), L"Can't Find");

	dynamic_cast<CVIBuffer*>(iter->second)->CopyToClient(pVertex);
}

void CResourceMgr::SendToEngine(RESOURCETYPE eResourceType, const wstring & wstrResourceKey, void * pVertex)
{
	auto& iter = m_mapResource[eResourceType].find(wstrResourceKey);
	if (iter == m_mapResource[eResourceType].end())
		TAGMSG_BOX(wstrResourceKey.c_str(), L"Can't Find");

	dynamic_cast<CVIBuffer*>(iter->second)->SendToEngine(pVertex);
}


HRESULT Engine::CResourceMgr::Add_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
	RESOURCETYPE eResourceType,
	BUFFERID eBufferType,
	const wstring& wstrResourceKey,
	const WORD& wCntX /*= 1*/,
	const WORD& wCntZ /*= 1*/,
	const WORD& wItv /*= 1*/)
{
	auto	iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if (iter != m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	CResources*		pResource = nullptr;

	switch (eBufferType)
	{
	case BUFFER_TRICOL:
		pResource = CTriCol::Create(pGraphicDev);
		break;

	case BUFFER_RCCOL:
		pResource = CRcCol::Create(pGraphicDev);
		break;

	case BUFFER_RCTEX:
		pResource = CRcTex::Create(pGraphicDev);
		break;

	case BUFFER_RCYTEX:
		pResource = CRcyTex::Create(pGraphicDev);
		break;

	case BUFFER_TERRAINCOL:
		pResource = CTerrainCol::Create(pGraphicDev, wCntX, wCntZ, wItv);
		break;

	case BUFFER_TERRAINTEX:
		pResource = CTerrainTex::Create(pGraphicDev, wCntX, wCntZ, wItv);
		break;

	case BUFFER_CUBECOL:
		pResource = CCubeCol::Create(pGraphicDev);
		break;

	case BUFFER_CUBE_NORMAL:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_NORMAL);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_FRONT:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_FRONT);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_CullB:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_CullB);		//여기서 인자값 주기.
		break;


	case BUFFER_CIRCLECOL:
		pResource = CCircleCol::Create(pGraphicDev);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_CullL:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_CullL);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_CullR:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_CullR);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_CullLR:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_CullLR);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_CullBackAndB:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_CullBackAndB);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_Player:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_Player);		//여기서 인자값 주기.
		break;

	case BUFFER_CUBE_SkyBox:
		pResource = CCubeTex::Create(pGraphicDev, CUBE_CLIPPING_TYPE::CUBE_SkyBox);		//여기서 인자값 주기.
		break;

	case BUFFER_UVTEX:
		pResource = CUvTex::Create(pGraphicDev);		//여기서 인자값 주기.
		break;

	case BUFFER_RCMOUSE:
		pResource = CRcMouse::Create(pGraphicDev);		//여기서 인자값 주기.
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[eResourceType].emplace(wstrResourceKey, pResource);

	return S_OK;
}


HRESULT Engine::CResourceMgr::Add_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	RESOURCETYPE eResourceType,
	TEXTURETYPE eTexType,
	const wstring& wstrResourceKey,
	const wstring& wstrFilePath,
	const WORD& wCnt)
{
	auto	iter = m_mapTexture[eResourceType].find(wstrResourceKey);

	if (iter != m_mapTexture[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"Texture Already Loaded");
		return E_FAIL;
	}

	CResources*	pResources = CTexture::Create(pGraphicDev, eTexType, wstrFilePath, wCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_mapTexture[eResourceType].emplace(wstrResourceKey, pResources);

	return S_OK;
}

HRESULT CResourceMgr::Delete_Buffer(RESOURCETYPE eResourceType, const wstring & wstrResourceKey)
{
	auto	iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if (iter == m_mapResource[eResourceType].end())
		return E_FAIL;

	Safe_Delete(iter->second);
	m_mapResource[eResourceType].erase(wstrResourceKey);

	return S_OK;
}


void Engine::CResourceMgr::Render(const wstring& wstrResourceKey)
{
	auto		iter = m_mapResource[RESOURCETYPE::RESOURCE_STATIC].find(wstrResourceKey);

	if (iter == m_mapResource[RESOURCETYPE::RESOURCE_STATIC].end())
		return;

	iter->second->Render();
}

void CResourceMgr::SetTexture(WORD wIndex, const wstring & wstrTextureKey, WORD wCnt)
{
	auto	iter = m_mapTexture[wIndex].find(wstrTextureKey);

	if (iter == m_mapTexture[wIndex].end())
		return;

	dynamic_cast<CTexture*>(iter->second)->SetTexture(wCnt);
}

CResources* CResourceMgr::Clone_Buffer(RESOURCETYPE eResourceType, const wstring & wstrResourceKey)
{
	auto	iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if (iter == m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"복사 실패");
		return nullptr;
	}

	return iter->second->Clone();
}

CResources* CResourceMgr::Clone_Tex(RESOURCETYPE eResourceType, const wstring & wstrResourceKey)
{
	auto	iter = m_mapTexture[eResourceType].find(wstrResourceKey);

	if (iter == m_mapTexture[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"복사 실패");
		return nullptr;
	}

	return iter->second->Clone();
}

CResources * CResourceMgr::Clone_UVBuffer()
{
	auto	iter = m_mapResource[RESOURCE_STATIC].find(L"Buffer_UVTex");

	if (iter == m_mapResource[RESOURCE_STATIC].end())
	{
		TAGMSG_BOX(L"Texture_UvTEX", L"복사 실패");
		return nullptr;
	}

	return dynamic_cast<CUvTex*>(iter->second)->CloneUV();
}


void Engine::CResourceMgr::Release(void)
{
	for (size_t i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_mapResource[i].begin(), m_mapResource[i].end(), CDeleteMap());
		m_mapResource[i].clear();
	}


	for (size_t i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_mapTexture[i].begin(), m_mapTexture[i].end(), CDeleteMap());
		m_mapTexture[i].clear();
	}
}

