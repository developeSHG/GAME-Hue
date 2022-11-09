#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"
#include "Include.h"

class CCubeMgr;
BEGIN(Engine)
class CGameObject;

END
class CCubeCrate;
class CCubeSkull;
class CCubeBrick;
class CCubeBalloon;
class CLaser;
class CCubeRock;
class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr(void);
	~CCollisionMgr(void);
private:
	void		Release(void);
public:
	static bool IsCollisionCrateToCube(CCubeCrate* pPlayer);
	static bool IsCollisionPlayerToCube(CPlayer* pPlayer, bool *pPlayerProperties, bool* bCollSide, bool& bInteraction, bool& bIsLeftOn, bool& bIsRightOn, bool& bIsHang, bool& bLopeEnd, CHECK::SIDE&	eSide, PLAYERSTATE& eState, bool IsOnGrass);
	static bool IsCollisionSkullToCube(CCubeSkull* pObject);
	static bool IsCollisionRockToCube(CCubeRock* pObject);

	static BALLOON_COLCHECK IsCollisionBalloonToCube(CCubeBalloon* pBalloon, CCubeBrick* pCubeBrick);
	static int IsCollisionRightLeftLaserToCube(CLaser*	pLaser);
	static int IsCollisionUpDownLaserToCube(CLaser*	pLaser);
	static int IsCollisionLaserToPlayer(CLaser* pLaser, CPlayer* pPlayer, bool bIsDown);
	static int IsCollisionLaserToPlayerStage3(CLaser* pLaser, CPlayer* pPlayer, bool bIsDown);
	static int IsCollisionBalloonCubeToPlayer(CCubeBrick* pBrick, CPlayer* pPlayer, BALLOON_STATE& eState);
public:
	static bool	ABC(D3DXVECTOR3& _vMinMainObj, D3DXVECTOR3& _vMaxMainObj, D3DXVECTOR3& _vMinSubObj, D3DXVECTOR3& _MaxSubObj);
	static bool	CollisionCheckSphere(float& fOut, D3DXVECTOR3 vCenter1, float fRadius1, D3DXVECTOR3 vCenter2, float fRadius2);
private:
};
#endif // CollisionMgr_h__

