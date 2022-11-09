#ifndef Struct_h__
#define Struct_h__


typedef struct tagFrame
{
	float fCurFrame;
	int	  iMaxFrame;
	float fFrameSpeed;

}FRAME_INFO;

enum PARTI_ANGLETYPE { PARTI_ANGLE_X, PARTI_ANGLE_Y, PARTI_ANGLE_Z, PARTI_ANGLE_END };

typedef struct tagParticleInfo
{
	D3DXMATRIX		matWorld;
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vScale;
	D3DXVECTOR3		vDir;
	float			fVelocity;
	float			fAngle[PARTI_ANGLETYPE::PARTI_ANGLE_END];

}PARTICLE_INFO;

typedef struct tagCollSphere
{
	D3DXVECTOR3		vCenterPos;		// �� �߽���
	float			fScale;			// ũ��
	float			fRadius;		// ������
}COLL_SPHERE;


#endif