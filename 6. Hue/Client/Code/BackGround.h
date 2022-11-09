#ifndef BackGround_h__
#define BackGround_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer;
END


class CBackObject;
class CBackLight;
class CFire;
class CBackGround : public Engine::CGameObject
{
public:
	enum CURCOLORID { CURCOLORID_RED, CURCOLORID_YELLOW, CURCOLORID_GREEN, CURCOLORID_BLUE, CURCOLOR_END };

private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackGround(void);

public:
	virtual void	Update(void) override;
	virtual void	LateUpdate(void);
	virtual void	Render(void) override;

	void			SetTransform(void);



private:
	void	Release(void);
	virtual HRESULT Initialize(void) override;
	HRESULT			AddComponent(void);

	void			MakeFireParticle(void);

	float			m_fMakeTime = 0.f;

private:
	void			ColorToYellow();
	void			ColorToGreen();
	void			ColorToBlue();
	void			ColorToRed();

	CURCOLORID		m_eCurColorID = CURCOLORID::CURCOLORID_RED;
	float			m_fColorR = 0.f;
	float			m_fColorG = 0.f;
	float			m_fColorB = 0.f;

public:
	static CBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::INFO				m_tInfo;
	float						m_fAngle = 0.f;
	float						m_fFrameCnt = 0.f;
	float						m_fFrameMax = 37.f;

	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;


private:
	CBackObject*				m_pBackObject = nullptr;
	CBackLight*					m_pBackLight = nullptr;
	list<CFire*>				m_ListFire;
private:
	D3DMATERIAL9				m_tMtrl;
};

#endif // BackGround_h__
