#ifndef Pipeline_h__
#define Pipeline_h__

#include "Engine_Include.h"
#include "MathMgr.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline
{
private:
	CPipeline(void);
	~CPipeline(void);

public:
	static void	MakeTransformMatrix(D3DXMATRIX* pMatrix,
									const D3DXVECTOR3* pRight, 
									const D3DXVECTOR3* pUp, 
									const D3DXVECTOR3* pLook, 
									const D3DXVECTOR3* pPos);

	static void MakeWorldMatrix(D3DXMATRIX* pMatrix, 
									const D3DXVECTOR3* pScale, 
									const float* pAngle, 
									const D3DXVECTOR3* pPos);

	static void MakeViewSpaceMatrix(D3DXMATRIX* pMatrix, 
									const D3DXVECTOR3* pEye,
									const D3DXVECTOR3* pAt, 
									const D3DXVECTOR3* pUp);

	static void MakeProjectionMatrix(D3DXMATRIX* pMatrix,
									const float& fFovY,
									const float& fAspect,
									const float& fNear,
									const float& fFar);

	static void MyTransformCoord(D3DXVECTOR3* pOut,
									const D3DXVECTOR3* pVector,
									const D3DXMATRIX* pMatrix);

	static void MyTransformCoordReverseX(D3DXVECTOR3* pOut,
									const D3DXVECTOR3* pVector,
									const D3DXMATRIX* pMatrix);

	static bool CubeCollisionCheck(D3DXVECTOR3* vMin1,
								D3DXVECTOR3* vMax1,
								D3DXVECTOR3* vMin2,
								D3DXVECTOR3* vMax2);
};

END
#endif // Pipeline_h__
