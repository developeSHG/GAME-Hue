#ifndef Fire_h__
#define Fire_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CFire : public Engine::CGameObject
{
public:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CFire(void);

public:
	void				SetSpawnPos(FIRESPAWNPOS ePos);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	int					FireUpdate();
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
private:

	void				UpdateLeftDown();
	void				UpdateLeftUp();
	void				UpdateRightDown();
	void				UpdateRightUp();

private:
	void				SetTransform(void);

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fvecAngle = 0.f;
	float						m_fRotAngle = 45.f;
	FIRESPAWNPOS				m_eSpawnPos = FIRESPAWNPOS::FIRESPAWN_END;

	bool						m_bSwitch = false;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CFire*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Fire_h__