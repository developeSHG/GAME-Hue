#ifndef Obj_Basic_h__
#define Obj_Basic_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Obj.h"

class CLightEffect;
class CObjBasic : public Engine::CObj
{
public:
	explicit CObjBasic(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CObjBasic(void);


	virtual	void		ResetObject(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	virtual int			ObjectUpdate(void);
private:
	virtual void		Release(void);

	void				MakeFireEffect(void);
	void				MakeTopFireEffect(void);
	void				MakeLeftLanternEffect(void);
	void				MakeRightLanternEffect(void);

public:
	D3DXVECTOR3			Get_Pos() { return m_pInfoCom->m_vPos; }
	void				SetDecoID(Engine::OBJDECOID eID) { m_eDecoID = eID; }
private:
	void				SetTransform(void);

protected:
	D3DXVECTOR3			m_vecOriginPos;
	bool				m_bIsInteraction;
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	Engine::OBJDECOID	m_eDecoID;
	list<CLightEffect*>	m_listLightEffect;

	float				m_fMakeEffectTime;


	float				m_fSoundTime;
	bool				m_bSound;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CObjBasic*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//Obj_Basic_h__