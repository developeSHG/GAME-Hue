#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Include.h"
#include "Logo.h"
#include "TutorialThread.h"
#include "Tutorial.h"
#include "Stage.h"
#include "Stage2.h"
#include "Stage3.h"
class CSceneSelector
{
public:
	explicit CSceneSelector(SCENEID eScene)
		: m_eScene(eScene)
	{
	}

public:
	HRESULT		operator()(Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pGraphicDev)
	{
		switch (m_eScene)
		{
		case  SC_LOGO:
			*ppScene = CLogo::Create(pGraphicDev);
			break;

		case  SC_TUTORIAL_THREAD:
			*ppScene = CTutorialThread::Create(pGraphicDev);
			break;

		case  SC_TUTORIAL:
			*ppScene = CTutorial::Create(pGraphicDev);
			break;

		case SC_STAGE:
			*ppScene = CStage::Create(pGraphicDev);
			break;
		case SC_STAGE2:
			*ppScene = CStage2::Create(pGraphicDev);
			break;
		case SC_STAGE3:
			*ppScene = CStage3::Create(pGraphicDev);
			break;
		}

		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}

private:
	SCENEID			m_eScene;

};



#endif // SceneSelector_h__
