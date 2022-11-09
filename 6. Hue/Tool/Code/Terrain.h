#ifndef Terrain_h__
#define Terrain_h__

#include "Engine_Include.h"
#include "GameObject.h"
class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CTerrain();

public:
	HRESULT Initialize(int vtxX,int vtxZ, int vtxItv);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, int vtxX, int vtxZ, int vtxItv);
};
#endif