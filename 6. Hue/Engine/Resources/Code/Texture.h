#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"
#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);

public:
	virtual ~CTexture(void);

public:
	virtual void Render(void) {}
	void SetTexture(const DWORD& iIndex);
	virtual CResources*		Clone(void);

private:
	HRESULT Load_Texture(TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt);
	virtual void Release(void);
	
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	DWORD								m_dwContainerSize;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt);
};

END


#endif // Texture_h__
