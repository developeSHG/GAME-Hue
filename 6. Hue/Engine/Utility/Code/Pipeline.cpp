#include "Pipeline.h"

USING(Engine)

Engine::CPipeline::CPipeline(void)
{

}

Engine::CPipeline::~CPipeline(void)
{

}

void Engine::CPipeline::MakeTransformMatrix(D3DXMATRIX* pMatrix,
											const D3DXVECTOR3* pRight,
											const D3DXVECTOR3* pUp,
											const D3DXVECTOR3* pLook,
											const D3DXVECTOR3* pPos)
{
	memcpy(&pMatrix->m[0][0], pRight, sizeof(float) * 3);
	memcpy(&pMatrix->m[1][0], pUp, sizeof(float) * 3);
	memcpy(&pMatrix->m[2][0], pLook, sizeof(float) * 3);
	memcpy(&pMatrix->m[3][0], pPos, sizeof(float) * 3);
}

void Engine::CPipeline::MakeWorldMatrix(D3DXMATRIX* pMatrix,
											const D3DXVECTOR3* pScale,
											const float* pAngle,
											const D3DXVECTOR3* pPos)
{
	D3DXMatrixIdentity(pMatrix);

	D3DXVECTOR3 vRight	= D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3 vUp		= D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 vLook	= D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3 vPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	// 크기 변환
	vRight	*= pScale->x;
	vUp		*= pScale->y;
	vLook	*= pScale->z;

	// 회전 변환

	CMathMgr::MyRotationX(&vRight, &vRight, pAngle[ANGLE_X]);
	CMathMgr::MyRotationX(&vUp, &vUp, pAngle[ANGLE_X]);
	CMathMgr::MyRotationX(&vLook, &vLook, pAngle[ANGLE_X]);

	CMathMgr::MyRotationY(&vRight, &vRight, pAngle[ANGLE_Y]);
	CMathMgr::MyRotationY(&vUp, &vUp, pAngle[ANGLE_Y]);
	CMathMgr::MyRotationY(&vLook, &vLook, pAngle[ANGLE_Y]);

	CMathMgr::MyRotationZ(&vRight, &vRight, pAngle[ANGLE_Z]);
	CMathMgr::MyRotationZ(&vUp, &vUp, pAngle[ANGLE_Z]);
	CMathMgr::MyRotationZ(&vLook, &vLook, pAngle[ANGLE_Z]);

	// 위치 변환
	vPos = *pPos;

	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);
}

void Engine::CPipeline::MakeViewSpaceMatrix(D3DXMATRIX* pMatrix,
											const D3DXVECTOR3* pEye,
											const D3DXVECTOR3* pAt,
											const D3DXVECTOR3* pUp)
{
	D3DXMatrixIdentity(pMatrix);

	D3DXVECTOR3 vRight = D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 vLook = D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	// Look
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	// Right
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// Up
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	// Pos
	vPos = *pEye;

	// 카메라의 월드행렬
	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);

	// 역행렬을 구해서 뷰스페이스 변환 행렬을 완성
	D3DXMatrixInverse(pMatrix, 0, pMatrix);
}

void Engine::CPipeline::MakeProjectionMatrix(D3DXMATRIX* pMatrix,
	const float& fFovY,
	const float& fAspect,
	const float& fNear,
	const float& fFar)
{
	D3DXMatrixIdentity(pMatrix);

	pMatrix->_11 = (1.f / tanf(fFovY / 2.f)) / fAspect;
	pMatrix->_22 = 1.f / tanf(fFovY / 2.f);

	pMatrix->_33 = fFar / (fFar - fNear);
	pMatrix->_43 = (-fNear * fFar) / (fFar - fNear);

	pMatrix->_34 = 1.f;
	pMatrix->_44 = 0.f;
}

void Engine::CPipeline::MyTransformCoord(D3DXVECTOR3* pOut,
	const D3DXVECTOR3* pVector,
	const D3DXMATRIX* pMatrix)
{
	D3DXVECTOR4	vVector(pVector->x, pVector->y, pVector->z, 1.f);

	pOut->x = vVector.x * pMatrix->_11 + vVector.y * pMatrix->_21 + vVector.z * pMatrix->_31 + vVector.w * pMatrix->_41;
	pOut->y = vVector.x * pMatrix->_12 + vVector.y * pMatrix->_22 + vVector.z * pMatrix->_32 + vVector.w * pMatrix->_42;
	pOut->z = vVector.x * pMatrix->_13 + vVector.y * pMatrix->_23 + vVector.z * pMatrix->_33 + vVector.w * pMatrix->_43;

	float fZ = vVector.x * pMatrix->_14 + vVector.y * pMatrix->_24 + vVector.z * pMatrix->_34 + vVector.w * pMatrix->_44;

	pOut->x /= fZ;
	pOut->y /= fZ;
	pOut->z /= fZ;
}

void CPipeline::MyTransformCoordReverseX(D3DXVECTOR3 * pOut, const D3DXVECTOR3 * pVector, const D3DXMATRIX * pMatrix)
{
	D3DXVECTOR4	vVector(pVector->x, pVector->y, pVector->z, 1.f);

	pOut->x = vVector.x * pMatrix->_11 + vVector.y * pMatrix->_21 + vVector.z * pMatrix->_31 + vVector.w * pMatrix->_41;
	pOut->y = vVector.x * pMatrix->_12 + vVector.y * pMatrix->_22 + vVector.z * pMatrix->_32 + vVector.w * pMatrix->_42;
	pOut->z = vVector.x * pMatrix->_13 + vVector.y * pMatrix->_23 + vVector.z * pMatrix->_33 + vVector.w * pMatrix->_43;

	float fZ = vVector.x * pMatrix->_14 + vVector.y * pMatrix->_24 + vVector.z * pMatrix->_34 + vVector.w * pMatrix->_44;

	pOut->x /= fZ;
	pOut->y /= fZ;
	pOut->z /= fZ;
}

bool CPipeline::CubeCollisionCheck(D3DXVECTOR3 * vMin1, D3DXVECTOR3 * vMax1, D3DXVECTOR3 * vMin2, D3DXVECTOR3 * vMax2)
{
	if (vMin1->x <= vMax2->x && vMax1->x >= vMin2->x &&
		vMin1->y <= vMax2->y && vMax1->y >= vMin2->y &&
		vMin1->z <= vMax2->z && vMax1->z >= vMin2->z)
		return true;
	return false;
}
