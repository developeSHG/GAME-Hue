#ifndef MathMgr_h__
#define MathMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CMathMgr
{
private:
	CMathMgr(void);
	~CMathMgr(void);

public:
	static void MyRotationX(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationY(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationZ(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);

};

END
#endif // MathMgr_h__
