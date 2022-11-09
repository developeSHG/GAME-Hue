#ifndef ColorHP_h__
#define ColorHP_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"
#include "BackGround.h"
BEGIN(Engine)

class CUvTex;
END;

class CCubeLaser;
class CLaserEffect;
class CColorHP : public Engine::CGameObject
{
public:
	explicit CColorHP(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CColorHP(void);


private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);

	void				MinusCount();
	void				PlusCount();
	void				MaxCount() { m_iCurChange = m_iMaxChange; }
private:
	virtual void		Release(void);


private:
	void				SetTransform(void);
public:
	// Ãß°¡µÊ
	Engine::CTransform*	Get_InfoCom() { return m_pInfoCom; }
private:
	Engine::CUvTex*		m_pBufferUvTex = nullptr;
	Engine::CVIBuffer*	m_pBufferCom = nullptr;
	Engine::CTransform* m_pInfoCom = nullptr;
	Engine::CTexture*	m_pTextureCom = nullptr;


	int					m_iMaxChange;
	int					m_iCurChange;
	D3DMATERIAL9				m_tMtrl;

	D3DXVECTOR3					m_vecOirgin;

	float			m_fColorR = 0.f;
	float			m_fColorG = 0.f;
	float			m_fColorB = 0.f;

	void			ColorToYellow();
	void			ColorToGreen();
	void			ColorToBlue();
	void			ColorToRed();

	CBackGround::CURCOLORID		m_eCurColorID = CBackGround::CURCOLORID::CURCOLORID_RED;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CColorHP*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//ColorHP_h__