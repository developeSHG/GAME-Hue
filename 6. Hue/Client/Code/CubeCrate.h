#ifndef CubeCrate_h__
#define CubeCrate_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"


class CButtonX;
class CCubeCrate : public Engine::CCube
{
public:
	explicit CCubeCrate(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeCrate(void);

private:
	virtual HRESULT Initialize(void);
public:
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Render(void);
	virtual void RenderInsideWorld(void);
private:
	virtual void Release(void);

private:
	void SetTransform(void);


private:
	float m_fLoadingTime = 0.f;
	bool m_bIsLoad = false;
public:
	// Ãß°¡µÊ
	void SetCollision(CHECK::SIDE eSide, bool bOn) { m_bIsCollisionSide[eSide] = bOn; }
	bool* GetCollision() { return m_bIsCollisionSide; }
	void SetCrateFall(bool bOn) { m_bCrateFall = bOn; }
	bool GetCrateFall() { return m_bCrateFall; }
	void SetOwned(bool bOn) { m_bIsOwned = bOn; }
	bool GetIsOwned() { return m_bIsOwned; }
private:
	CCameraObserver* m_pCameraObserver = nullptr;

	CButtonX* m_pButtonX;
public:
	virtual HRESULT AddComponent(void);
public:
	static CCubeCrate* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);

private:
	bool m_bIsCollisionSide[CHECK::SIDE::SIDE_END];
	bool m_bCrateFall = false;
	bool m_bIsOwned = false;
};
#endif //CubeCrate_h__