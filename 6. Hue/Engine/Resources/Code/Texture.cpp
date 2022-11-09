#include "Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
	, m_vecTexture(rhs.m_vecTexture)
	, m_dwContainerSize(rhs.m_dwContainerSize)
{

}

CTexture::~CTexture(void)
{
	Release();
}

void CTexture::SetTexture(const DWORD & iIndex)
{
	if (iIndex >= m_dwContainerSize)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CResources * CTexture::Clone(void)
{
	(*m_pwRefCnt)++;

	return new CTexture(*this);
}

HRESULT CTexture::Load_Texture(TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt)
{
	IDirect3DBaseTexture9*		pTexture = nullptr;

	m_vecTexture.reserve(wCnt);

	TCHAR	szFullPath[MAX_PATH] = L"";

	for (size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, wstrFilePath.c_str(), i);
		HRESULT	hr = NULL;

		switch (eTexType)
		{
		case TEX_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TEX_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		FAILED_CHECK_MSG(hr, szFullPath);

		m_vecTexture.push_back(pTexture);

	}

	m_dwContainerSize = m_vecTexture.size();


	return S_OK;
}

void CTexture::Release(void)
{
	if (0 == (*m_pwRefCnt))
	{
		for (size_t i = 0; i < m_vecTexture.size(); ++i)
			Safe_Release(m_vecTexture[i]);

		m_vecTexture.clear();

		CResources::Release();
	}
	else
		(*m_pwRefCnt)--;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt)
{
	CTexture*	pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Load_Texture(eTexType, wstrFilePath, wCnt)))
		Safe_Delete(pInstance);

	return pInstance;
}
