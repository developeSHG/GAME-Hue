#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

BEGIN(Engine)

class CManagement;
class CResourceMgr;

END

class CPlayer;
class CTerrain;
class CColorCircle;

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CStage(void);

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
	//�ӽ�
	CColorCircle*				m_pColorCircle = nullptr;
	CPlayer*					m_pPlayer = nullptr;
	

private:
	D3DVIEWPORT9				m_SubViewPort;
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool						m_bIsRenderViewport;
	bool						m_bIsResizeViewport;
	bool						m_bIsMoveUp;
	bool						m_bIsEnter;
private:
};

#endif // Stage_h__
