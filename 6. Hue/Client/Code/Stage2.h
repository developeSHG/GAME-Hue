#ifndef Stage2_h__
#define Stage2_h__

#include "Scene.h"

BEGIN(Engine)

class CManagement;
class CResourceMgr;

END

class CPlayer;
class CTerrain;

class CStage2 : public Engine::CScene
{
private:
	explicit CStage2(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CStage2(void);

public:
	virtual HRESULT Initialize(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Add_Environment(void);
	HRESULT		Add_GameObject(void);
	HRESULT		Add_UI(void);

private:
	void		UpdateViewportPos(void);

private:
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

private:
	//юс╫ц
	CPlayer*					m_pPlayer = nullptr;

private:
	D3DVIEWPORT9				m_SubViewPort;
public:
	static CStage2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool						m_bIsRenderViewport;
	bool						m_bIsResizeViewport;
	bool						m_bIsMoveUp;
	bool						m_bIsEnter;
private:
};

#endif // Stage2_h__
