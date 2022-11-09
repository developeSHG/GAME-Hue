#ifndef Engine_Math_h__
#define Engine_Math_h__

namespace Engine
{
	namespace Math
	{
		static float MyVec3Length(const D3DXVECTOR3* pVec)
		{
			return sqrtf(pVec->x * pVec->x + pVec->y * pVec->y + pVec->z * pVec->z);
		}

		static D3DXVECTOR3* MyVec3Normalize(D3DXVECTOR3* pOut, const D3DXVECTOR3* pVec)
		{
			float fVecLength = MyVec3Length(pVec);
			*(pOut) /= fVecLength;

			return pOut;
		}

		static float MyVec3Dot(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
		{
			return (pV1->x * pV2->x) + (pV1->y * pV2->y) + (pV1->z * pV2->z);
		}

		static D3DXVECTOR3* MyVec3Cross(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2)
		{
			pOut->x = pV1->y * pV2->z - pV1->z * pV2->y;
			pOut->y = pV1->z * pV2->x - pV1->x * pV2->z;
			pOut->z = pV1->x * pV2->y - pV1->y * pV2->x;

			//D3DXVec3Cross(pOut, pV1, pV2);
			return pOut;
		}
		static D3DXVECTOR3* MyVec3TransformCoord(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM)
		{
			D3DXVECTOR4 vec4; vec4.x = pV->x; vec4.y = pV->y; vec4.z = pV->z; vec4.w = 1.f;

			pOut->x = vec4.x * pM->m[0][0] + vec4.y * pM->m[1][0] + vec4.z * pM->m[2][0] + vec4.w * pM->m[3][0];
			pOut->y = vec4.x * pM->m[0][1] + vec4.y * pM->m[1][1] + vec4.z * pM->m[2][1] + vec4.w * pM->m[3][1];
			pOut->z = vec4.x * pM->m[0][2] + vec4.y * pM->m[1][2] + vec4.z * pM->m[2][2] + vec4.w * pM->m[3][2];

			return pOut;
		}
		static D3DXVECTOR3* MyVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM)
		{
			D3DXVECTOR4 vec4; vec4.x = pV->x; vec4.y = pV->y; vec4.z = pV->z; vec4.w = 0.f;

			pOut->x = vec4.x * pM->m[0][0] + vec4.y * pM->m[1][0] + vec4.z * pM->m[2][0] + vec4.w * pM->m[3][0];
			pOut->y = vec4.x * pM->m[0][1] + vec4.y * pM->m[1][1] + vec4.z * pM->m[2][1] + vec4.w * pM->m[3][1];
			pOut->z = vec4.x * pM->m[0][2] + vec4.y * pM->m[1][2] + vec4.z * pM->m[2][2] + vec4.w * pM->m[3][2];

			return pOut;
		}
		//За·Д
		static D3DXMATRIX* MyMatrixIdentity(D3DXMATRIX *pOut)
		{
			ZeroMemory(pOut, sizeof(D3DXMATRIX));
			pOut->m[0][0] = 1.f;
			pOut->m[1][1] = 1.f;
			pOut->m[2][2] = 1.f;
			pOut->m[3][3] = 1.f;

			return pOut;
		}

		static D3DXMATRIX* MyMatrixTranspose(D3DXMATRIX *pOut, const D3DXMATRIX *pM)
		{
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					pOut->m[i][j] = pM->m[j][i];
			return pOut;
		}

		static D3DXMATRIX* MyMatrixTranslation(D3DXMATRIX *pOut, float x, float y, float z)
		{
			MyMatrixIdentity(pOut);
			pOut->m[3][0] = x;
			pOut->m[3][1] = y;
			pOut->m[3][2] = z;
			pOut->m[3][3] = 1.f;

			return pOut;
		}

		static D3DXMATRIX* MyMatrixRotationX(D3DXMATRIX *pOut, float fAngle)
		{
			MyMatrixIdentity(pOut);
			pOut->m[1][1] = cosf(fAngle);
			pOut->m[1][2] = sinf(fAngle);
			pOut->m[2][1] = -sinf(fAngle);
			pOut->m[2][2] = cosf(fAngle);

			return pOut;
		}
		static D3DXMATRIX* MyMatrixRotationY(D3DXMATRIX *pOut, float fAngle)
		{
			MyMatrixIdentity(pOut);
			pOut->m[0][0] = cosf(fAngle);
			pOut->m[0][2] = -sinf(fAngle);
			pOut->m[2][0] = sinf(fAngle);
			pOut->m[2][2] = cosf(fAngle);

			return pOut;
		}
		static D3DXMATRIX* MyMatrixRotationZ(D3DXMATRIX *pOut, float fAngle)
		{
			MyMatrixIdentity(pOut);
			pOut->m[0][0] = cosf(fAngle);
			pOut->m[0][1] = sinf(fAngle);
			pOut->m[1][0] = -sinf(fAngle);
			pOut->m[1][1] = cosf(fAngle);

			return pOut;
		}

		static D3DXMATRIX* MyMatrixScaling(D3DXMATRIX *pOut, float sx, float sy, float sz)
		{
			MyMatrixIdentity(pOut);
			pOut->m[0][0] = sx;
			pOut->m[1][1] = sy;
			pOut->m[2][2] = sz;
			return pOut;
		}
		static D3DXMATRIX* MyMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pLook,
			const D3DXVECTOR3* pUp)
		{
			MyMatrixIdentity(pOut);
			D3DXVECTOR3 vecLook = *pLook - *pEye;
			MyVec3Normalize(&vecLook, &vecLook);
			//right up look

			pOut->m[2][0] = vecLook.x;	pOut->m[2][1] = vecLook.y;	pOut->m[2][2] = vecLook.z;

			D3DXVECTOR3 right;
			MyVec3Cross(&right, pUp, &vecLook);
			MyVec3Normalize(&right, &right);

			pOut->m[0][0] = right.x;	pOut->m[0][1] = right.y;	pOut->m[0][2] = right.z;

			D3DXVECTOR3 up;
			MyVec3Cross(&up, &vecLook, &right);
			MyVec3Normalize(&up, &up);

			pOut->m[1][0] = up.x;		pOut->m[1][1] = up.y;		pOut->m[1][2] = up.z;

			pOut->m[3][0] = pEye->x;	pOut->m[3][1] = pEye->y;	pOut->m[3][2] = pEye->z;

			D3DXMatrixInverse(pOut, 0, pOut);

			return pOut;
		}
		static D3DXMATRIX* MyMatrixPerspectiveFovLH(D3DXMATRIX *pOut, const float fovy, const float aspect,
			const float zn, const float zf)
		{
			ZeroMemory(pOut, sizeof(D3DXMATRIX));
			pOut->m[0][0] = (1.f / tanf(fovy / 2)) * (1.f / aspect);
			pOut->m[1][1] = 1.f / tanf(fovy / 2);
			pOut->m[2][2] = zf / (zf - zn);
			pOut->m[3][2] = -((zf * zn) / (zf - zn));
			pOut->m[2][3] = 1.f;

			return pOut;
		}
	}
}


#endif // Engine_Math_h__