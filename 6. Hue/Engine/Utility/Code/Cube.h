#ifndef Cube_h__
#define Cube_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;

class ENGINE_DLL CCube : public CGameObject
{
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_INFO& _eCubeInfo);
public:
	virtual ~CCube(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);

	void				FirstMovingUpdate();
	void						SwapPos();
private:
	virtual void		Release(void);


protected:
	virtual HRESULT		AddComponent(void);

	bool				m_bIsRotMinus;
	bool				m_bIsMovingCube;
	D3DXVECTOR3			m_vecOriginPos;
public:
	void				SetMovingCube();
	void				SetOriginPos();
private:
	
	bool				m_bIsMove;
	bool				m_bIsRotX;
	bool				m_bIsRotY;
	bool				m_bIsRotZ;
	// Get
public:
	void						Set_CubeColor(Engine::CUBE_COLOR eColor) { m_tCubeInfo.tColor = eColor; }
	const CUBE_INFO&			Get_CubeInfo() const { return m_tCubeInfo; }
	const Engine::CUBE_COLOR&	Get_CubeColor() const { return m_tCubeInfo.tColor; }
	Engine::VTXTEX*				Get_Vertex() { return m_pVertex; }
	Engine::VTXTEX*				Get_ConvertVertex() { return m_pConvertVtx; }
	Engine::CTransform*			Get_InfoCom() { return m_pInfoCom; }

protected:
	Engine::CVIBuffer*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pInfoCom = nullptr;
	Engine::CResourceMgr*	m_pResourceMgr = nullptr;

	Engine::VTXTEX*			m_pVertex = nullptr;
	Engine::VTXTEX*			m_pConvertVtx = nullptr;
	DWORD					m_dwVtxCnt = 0;


	bool					m_bIsSpike;
	D3DXVECTOR3				m_vecRight;
	D3DXVECTOR3				m_vecMoveDir;
	bool					m_bNoAngleMore;
	float					m_fDirAngle = 45.f;
protected:
	CUBE_INFO					m_tCubeInfo;
	D3DMATERIAL9				m_tMtrl;
	D3DCOLOR					m_tColor;

public:
	void SetMaterial(D3DXCOLOR tColor);

public:
	static CCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_INFO& _eCubeInfo);

};

END
#endif // Cube_h__
