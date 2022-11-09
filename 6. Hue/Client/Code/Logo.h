#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
BEGIN(Engine)

class CManagement;
class CResourceMgr;
END

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLogo(void);

public:
	virtual HRESULT		Initialize(void) override;
	virtual void		Update(void) override;
	virtual void		LateUpdate(void) override;
	virtual void		Render(void) override;
	virtual void		Release(void) override;

private:
	HRESULT				Add_Environment(void) {};
	HRESULT				Add_GameObject(void);
	HRESULT				Add_UI(void) {};

private:
	HRESULT				BufferForLogo(void);
	HRESULT				TextureForLogo(void);
	HRESULT				BufferForTutorialThread(void);
	HRESULT				TextureForTutorialThread(void);
	HRESULT				LoadPlayerTexture(void);
	HRESULT				LoadPlayerMaskTexture(void);

private:
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;


public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

#endif // Logo_h__
