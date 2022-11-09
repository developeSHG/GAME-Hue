#ifndef Obj_h__
#define Obj_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Transform.h"
BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;

class ENGINE_DLL CObj : public CGameObject
{
protected:
	explicit CObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO& _eCubeInfo);
public:
	virtual ~CObj(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);


protected:
	virtual HRESULT		AddComponent(void);

	// Get
public:
	const OBJ_INFO&				Get_CubeInfo() const { return m_tObjInfo; }
	const Engine::CUBE_COLOR&	Get_CubeColor() const { return m_tObjInfo.tColor; }
	Engine::VTXTEX*				Get_Vertex() { return m_pVertex; }
	Engine::VTXTEX*				Get_ConvertVertex() { return m_pConvertVtx; }
	Engine::CTransform*			Get_InfoCom() { return m_pInfoCom; }

	void Set_ObjPos(D3DXVECTOR3& vPos) { m_tObjInfo.vPos = vPos; m_pInfoCom->m_vPos = m_tObjInfo.vPos; }
	void Set_ObjScale(D3DXVECTOR3& vScale) { m_tObjInfo.vScale = vScale; m_pInfoCom->m_vScale = m_tObjInfo.vScale; }
	void Set_ObjKey(int iKey) { m_tObjInfo.iKey = iKey; }

	void Set_CubeColor(Engine::CUBE_COLOR eColor) { m_tObjInfo.tColor = eColor; }
protected:
	Engine::CVIBuffer*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pInfoCom = nullptr;
	Engine::CResourceMgr*	m_pResourceMgr = nullptr;

	Engine::VTXTEX*			m_pVertex = nullptr;
	Engine::VTXTEX*			m_pConvertVtx = nullptr;
	DWORD					m_dwVtxCnt = 0;

protected:
	OBJ_INFO					m_tObjInfo;
	D3DMATERIAL9				m_tMtrl;
	D3DCOLOR					m_tColor;

public:
	void SetMaterial(D3DXCOLOR tColor);

public:
	static CObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_INFO& _eCubeInfo);

};

END
#endif // Cube_h__
