#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CResources;
class ENGINE_DLL CResourceMgr
{

	DECLARE_SINGLETON(CResourceMgr)
private:
	CResourceMgr();
	~CResourceMgr();

public:

	void CopyToClient(RESOURCETYPE eResourceType, const wstring& wstrResourceKey, void* pVertex);
	void SendToEngine(RESOURCETYPE eResourceType, const wstring& wstrResourceKey, void* pVertex);

public:
	HRESULT		Add_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
		RESOURCETYPE eResourceType,
		BUFFERID eBufferType,
		const wstring& wstrResourceKey,
		const WORD& wCntX = 1,
		const WORD& wCntZ = 1,
		const WORD& wItv = 1);

	HRESULT		Add_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
		RESOURCETYPE eResourceType,
		TEXTURETYPE eTexType,
		const wstring& wstrResourceKey,
		const wstring& wstrFilePath,
		const WORD& wCnt);

	HRESULT		Delete_Buffer(RESOURCETYPE eResourceType,
		const wstring& wstrResourceKey);

	void		Render(const wstring& wstrResourceKey);
	void		SetTexture(WORD wIndex, const wstring& wstrTextureKey, WORD wCnt);

	CResources*	Clone_Buffer(RESOURCETYPE eResourceType, const wstring& wstrResourceKey);
	CResources*	Clone_Tex(RESOURCETYPE eResourceType, const wstring& wstrResourceKey);
	CResources* Clone_UVBuffer();

private:
	void		Release(void);

private:
	map<wstring, CResources*>		m_mapResource[RESOURCE_END];
	map<wstring, CResources*>		m_mapTexture[RESOURCE_END];
	CResources*						m_pResource = nullptr;
};

END

#endif // ResourceMgr_h__
