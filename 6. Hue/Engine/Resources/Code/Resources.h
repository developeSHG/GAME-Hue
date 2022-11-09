#ifndef Resources_h__
#define Resources_h__

#include "Engine_Include.h"
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResources(const CResources& rhs);
public:
	virtual ~CResources(void);

	virtual void			Update(void);
	virtual void			Render(void) PURE;
	virtual CResources*		Clone(void) PURE;
	virtual void			Release(void);
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	WORD*					m_pwRefCnt;
};
END



#endif // Resources_h__
